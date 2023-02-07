#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/IGW3DStringCollection.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"

#include "Geoweb3dCore/LayerParameters.h"
#include "GeoWeb3dCore/SystemExports.h"

// A simple example of navigation control.
#include "../Common/NavigationHelper.h"

#include <set>
#include <map>
#include <deque>
#include <iostream>
#include <tchar.h>
#include <ctime>
#include <chrono>

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment( lib, "GW3DCommon.lib" )

#define SHAPEFILE_MODE 0
#if SHAPEFILE_MODE == 0
    #define GEOPACKAGE_MODE 1
#else 
    #define GEOPACKAGE_MODE 0
#endif

void my_fatal_function( const char* msg );
void my_info_function( const char* msg );
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Helper function for randomizing properties
///           of a features's property collection.                             </summary>
/// 
/// <param name="property_collection"> Cloned default property collection.     </param>
/// 
/// <returns> The passed in property_collection in a randomized state.         </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DPropertyCollectionPtr RandomizeProperties( Geoweb3d::IGW3DPropertyCollectionPtr property_collection )
{
    // Randomizes the draped_line_rep's cloned property collection.
    auto definition_collection = property_collection->get_DefinitionCollection();
    property_collection->put_Property( definition_collection->get_IndexByName( "RED" ),   static_cast< float >( rand() ) / static_cast< float >( RAND_MAX ) );
    property_collection->put_Property( definition_collection->get_IndexByName( "GREEN" ), static_cast< float >( rand() ) / static_cast< float >( RAND_MAX ) );
    property_collection->put_Property( definition_collection->get_IndexByName( "BLUE" ),  static_cast< float >( rand() ) / static_cast< float >( RAND_MAX ) );
    property_collection->put_Property( definition_collection->get_IndexByName( "LINE_WIDTH" ), rand() % 10 + 1 );
    property_collection->put_Property( definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 0.0 );
    return property_collection;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Helper class for bundling information about an intersection hit together. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct FeatureInfo
{
	long	id;
	double	intersection_distance;
    Geoweb3d::IGW3DVectorRepresentationDriverWPtr rep_driver;
    Geoweb3d::IGW3DVectorLayerWPtr                rep_vector_layer;
    Geoweb3d::IGW3DVectorRepresentationWPtr       rep_vector;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Constructor for the feature of a layer hit in a 
    ///           representation and its distance form the camera in meters. </summary>
    /// 
    /// <param name="object_id"> Feature ID, unique to the layer.            </param>
    /// 
    /// <param name="distance"> Distance the object_id was from the 
    ///                         camera at the time of intersection.          </param>
    /// 
    /// <returns> A constructed FeatureInfo with a valid feature ID 
    ///           and distance from camera.                                  </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	FeatureInfo ( long object_id, double distance = 0.0 )
		: id                   ( object_id )
        , intersection_distance( distance )
	{ }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Less than operator overload for std::set hashing.               </summary>
    /// 
    /// <param name="rhs"> FeatureInfo to compare against 'this'.                 </param>    
    /// 
    /// <returns> If the rhs FeatureInfo should be considered "less than" 'this'. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	bool operator <( const FeatureInfo& rhs ) const
	{
        bool less_than = false;
        auto lhs_locked_rep_driver = rep_driver.lock();
        auto rhs_locked_rep_driver = rhs.rep_driver.lock();
        if ( lhs_locked_rep_driver && rhs_locked_rep_driver )
        {
            less_than = std::strcmp( lhs_locked_rep_driver->get_Name(), rhs_locked_rep_driver->get_Name() ) == 0;
        }

        // Only compare vector layers from the same driver for proper equality
        // since object / feature IDs are unique within a single layer.
        if ( less_than )
        {
            auto lhs_locked_vector_layer = rep_vector_layer.lock();
            auto rhs_locked_vector_layer = rhs.rep_vector_layer.lock();
            if ( lhs_locked_vector_layer == rhs_locked_vector_layer )
            {
                less_than = ( id < rhs.id );
            }
        }
        return less_than;
	}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Equality operator overload for std::set hashing.               </summary>
    /// 
    /// <param name="rhs"> FeatureInfo to compare against 'this'.                </param>    
    /// 
    /// <returns> If the rhs FeatureInfo should be considered "equal to" 'this'. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	bool operator ==( const FeatureInfo& rhs ) const
	{
        bool equal = false;
        auto lhs_locked_rep_driver = rep_driver.lock();
        auto rhs_locked_rep_driver = rhs.rep_driver.lock();
        if ( lhs_locked_rep_driver && rhs_locked_rep_driver )
        {
            equal = std::strcmp( lhs_locked_rep_driver->get_Name(), rhs_locked_rep_driver->get_Name() ) == 0;
        }

        // Only compare vector layers from the same driver for proper equality
        // since object / feature IDs are unique within a single layer.
        if ( equal )
        {
            auto lhs_locked_vector_layer = rep_vector_layer.lock();
            auto rhs_locked_vector_layer = rhs.rep_vector_layer.lock();
            if ( lhs_locked_vector_layer == rhs_locked_vector_layer )
            {
                equal = ( id == rhs.id );
            }
        }
        return equal;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic IGW3DVectorLayerStream which invokes OnStream 
///           for every Feature in the VectorLayer being streamed.
///           Its purpose is to randomize the properties of each
///           feature independently from one another.               </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class FeaturePropertyRandomizerStream : public Geoweb3d::IGW3DVectorLayerStream
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> FeaturePropertyRandomizerStream constructor.                 </summary>
    /// 
    /// <param name="vector_rep"> The IGW3DVectorRepresentationWPtr 
    ///                           the VectorLayer belongs to.                  </param>    
    /// 
    /// <returns> FeaturePropertyRandomizerStream object that can be streamed. </returns>    
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FeaturePropertyRandomizerStream( Geoweb3d::IGW3DVectorRepresentationWPtr vector_rep )
        : vector_rep_( vector_rep )
    {}
   
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Invoked when Stream is called. This will randomize
    ///           the property collection for the current Feature ID. </summary>
    /// 
    /// <param name="result"> The IGW3DVectorLayerStreamResult which 
    ///                       contains certain information about the 
    ///                       current Feature being iterated over.    </param>    
    /// 
    /// <returns> Whether or not the properties of the
    ///           current Feature were properly randomized.           </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult* result ) override
    {
        bool successful = false;
        long feature_id = result->get_ObjectID();        
        
        if ( !vector_rep_.expired() )
        {
            vector_rep_.lock()->put_PropertyCollection( feature_id, RandomizeProperties( vector_rep_.lock()->get_PropertyCollection()->create_Clone() ) );
            successful = true;
        }
        return successful;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> OnError IGW3DVectorLayerStream override.
    ///           This can be used to handle any custom
    ///           stop or error-related stream cancel.           </summary>
    /// 
    /// <remarks> If the count of the stream is 0, this is
    ///           ignored, regardless of what value is returned. </remarks>
    /// 
    /// <returns> true.                                          </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool OnError() override { return true; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> count IGW3DVectorLayerStream override.                                     </summary>
    /// 
    /// <remarks> If you return 0 on Geoweb3d::IGW3DVectorLayerStream::count(), it'll 
    ///           implicitly iterate over all Object IDs in OnStream. As a result, we 
    ///           don't really need to implement a container or true iterator tracking.
    ///           For a more complex and in-depth streamer refer to the 
    ///           DrapedLineIntersectionTestStream below. It does the same thing as this 
    ///           streamer does, but only for a single feature ID. It requires more book
    ///           keeping and client-side maintenance.                                       </remarks>
    /// 
    /// <returns> 0.                                                                         </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual unsigned long count() const override { return 0; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Gets the next feature/object id in the 
    ///           collection and increments the iterator.                                 </summary>
    ///
    /// <remarks> If the count returns 0, then this function is ignored.                  </remarks>
    /// 
    /// <param name="ppVal"> [out] If non-null and non-zero count(), the 
    ///                      next Feature / Object ID the OnStream result
    ///                      will contain.                                                </param>
    ///
    /// <returns> true if it succeeds, false if no more features exist in the collection. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool next( int64_t* ppVal ) override { return true; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Resets the iterator.                                   </summary>
    ///
    /// <remarks> If the count returns 0, then this function is ignored. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void reset() override { }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Array indexer operator overload.                       </summary>
    ///
    /// <remarks> Get the feature/object id at the given index.
    ///           If the count returns 0, then this function is ignored. </remarks>
    ///
    /// <param name="index"> Zero-based index of the feature/object id.  </param>
    ///
    /// <returns> The feature/object id.                                 </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int64_t operator[]( unsigned long index ) override { return 0; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Gets feature/object id at index.                       </summary>
    ///
    /// <remarks> Get the feature/object id at the given index.
    ///           If the count returns 0, then this function is ignored. </remarks>
    ///
    /// <param name="index"> Zero-based index of the feature/object id.  </param>
    ///
    /// <returns> The feature/object id.                                 </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int64_t get_AtIndex( unsigned long index ) override { return 0; }

private:

    Geoweb3d::IGW3DVectorRepresentationWPtr vector_rep_;

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A slightly more in-depth IGW3DVectorLayerStream which
///           invokes OnStream for a single Feature in the VectorLayer
///           being streamed. Its purpose is to randomize the properties
///           of a single feature independently from one another.        </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class DrapedLineIntersectionTestStream : public Geoweb3d::IGW3DVectorLayerStream
{
	public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Constructor which ensures the iterator index is proper. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        DrapedLineIntersectionTestStream()
		{
			reset();
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Destructor for any local class cleanup. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual ~DrapedLineIntersectionTestStream( )
		{
			feature_info_set.clear();
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Helper function used in conjuction with a LineSegmentIntersectionTest.
        ///           All valid results of the IGW3DLineSegmentIntersectionReportCollection
        ///           from the App's PostDraw will be inserted into a local set of FeatureInfo
        ///           objects.                                                                    </summary>
        ///
        /// <param name="object_id"> Zero-based index of the feature/object id.                   </param>
        /// 
        /// <param name="vector_rep"> The VectorRepresentation containing the layer and features. </param>
        /// 
        /// <param name="intersection_distance"> Distance in meters from the camera the 
        ///                                      object_id was collided with in the scene.        </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void insert ( long object_id, Geoweb3d::IGW3DVectorRepresentationWPtr vector_rep, double intersection_distance )
		{
            // Boiler-plate FeatureInfo setup. This information will be used in the OnStream call.
			FeatureInfo new_feature_info( object_id );
            new_feature_info.intersection_distance = intersection_distance;
            new_feature_info.rep_vector = vector_rep;
            if( auto locked_rep_vector = new_feature_info.rep_vector.lock() )
            {
                new_feature_info.rep_driver       = locked_rep_vector->get_Driver();
                new_feature_info.rep_vector_layer = locked_rep_vector->get_VectorLayer();
                new_feature_info.rep_vector_layer.lock()->get_RepresentationLayerCollection();
                feature_info_set.insert( new_feature_info );
            }
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Interface to access a FeatureInfo object, rather than its iterator. </summary>
        ///
        /// <param name="object_id"> Zero-based index of the feature/object id.           </param>
        ///                         
        /// <returns> The FeatureInfo for the given object_id.                            </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		const FeatureInfo& find ( long object_id )
		{
			return *feature_info_set.find( object_id );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Invoked when Stream is called. This will randomize
        ///           the property collection for the current Feature ID. </summary>
        /// 
        /// <param name="result"> The IGW3DVectorLayerStreamResult which 
        ///                       contains certain information about the 
        ///                       current Feature being iterated over.    </param>    
        /// 
        /// <returns> Whether or not the properties of the
        ///           current Feature were properly randomized.           </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult* result ) override
		{
            bool successful = false;

            // Query the FeatureInfo object / feature id from the current StreamResult.
			long object_id = result->get_ObjectID();                        
			FeatureInfo finfo = find( object_id );
            Geoweb3d::IGW3DVectorRepresentationWPtr vector_rep = finfo.rep_vector;
            
            // If we found a VectorRepresentation with the object ID, we'll try to check its properties and print to std::cout.
            if ( !vector_rep.expired() )
            {
                // Iterates over the definition collection of a property collection and prints the current property description alongside its value.
                auto print_properties = []( const Geoweb3d::IGW3DPropertyCollection* property_collection )
                {
                    if ( property_collection != nullptr )
                    {
                        const Geoweb3d::IGW3DDefinitionCollection* definition_collection = property_collection->get_DefinitionCollection();
                        for ( unsigned int definition_index = 0; definition_index < definition_collection->count(); ++definition_index )
                        {
                            const Geoweb3d::GW3DPropertyDescription* current_property = definition_collection->get_AtIndex( definition_index );
                            Geoweb3d::IGW3DStringPtr property_value_str = Geoweb3d::IGW3DString::create( "" );
                            property_collection->get_AtIndex( definition_index )->to_string( property_value_str );
                            std::cout << "Property #" << definition_index << "( " << current_property->property_name << " ): " << property_value_str->c_str() << std::endl;
                        }
                        std::cout << std::endl;
                    }
                };

                std::cout << "Feature Info: <id : " << object_id << " > " << " < Intersection Distance: " << finfo.intersection_distance << " >" << std::endl;
                std::cout << "Properties before: " << std::endl;
                print_properties( result->get_VectorRepresentationProperties( vector_rep ) );
                
                // Now we'll randomize the properties to show user interaction with the vector representation.
                vector_rep.lock()->put_PropertyCollection( object_id, RandomizeProperties( vector_rep.lock()->get_PropertyCollection()->create_Clone() ) );

                std::cout << "Properties after: " << std::endl;
                print_properties( result->get_VectorRepresentationProperties( vector_rep ) );

                successful = true;

            }

			return successful;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> OnError IGW3DVectorLayerStream override.
        ///           This can be used to handle any custom
        ///           stop or error-related stream cancel.           </summary>
        /// 
        /// <returns> true.                                          </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnError() override
		{
			return true;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Gets the count of features to be processed. </summary>
        ///
        /// <returns> The size of the feature_info_set.           </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned long count() const override
		{
			return static_cast<unsigned long>( feature_info_set.size() );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Gets the next feature/object id in the 
        ///           collection and increments the iterator.                                 </summary>
        /// 
        /// <param name="ppVal"> [out] If non-null and non-zero count(), the 
        ///                      next Feature / Object ID the OnStream result
        ///                      will contain.                                                </param>
        ///
        /// <remarks> This function isn't called within the context of this Streamer example,
        ///           but exemplifies how you would properly implement the next override in
        ///           the event it was necessary.                                             </remarks>
        /// 
        /// <returns> true if it succeeds, false if no more features exist in the collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool next( int64_t* ppVal ) override
		{
            bool successful = false;
            ++nIndex_;
			if ( nIndex_ >= count() )
			{
				*ppVal = 0;
				reset();
            }
            else
            {
                *ppVal = feature_info_set_iterator->id;
                ++feature_info_set_iterator;
                successful = true;
            }
			return successful;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Resets the iterator(s). </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void reset() override
		{
            nIndex_ = 0;
			feature_info_set_iterator = feature_info_set.begin();
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Array indexer operator. Gets feature/object id at index. </summary>
        ///
        /// <remarks> For the most part, this can usually forward the call to
        ///           get_AtIndex with the index paramater being passed along. </remarks>
        ///
        /// <param name="index"> Zero-based index of the feature/object id.    </param>
        ///
        /// <returns> The feature/object id.                                   </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int64_t operator[](unsigned long index) override
		{
			return get_AtIndex( index );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Gets feature/object id at index.                         </summary>
        ///
        /// <param name="index"> Zero-based index of the feature/object id.    </param>
        ///
        /// <returns> The feature/object id. -1 if the index is out of bounds. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int64_t get_AtIndex( unsigned long index ) override
		{
            int feature_id = -1;
            if ( index < count() )
            {
                std::set< FeatureInfo >::iterator temp_feature_info_set_iterator = feature_info_set.begin();
                std::advance( temp_feature_info_set_iterator, index );
                feature_id = temp_feature_info_set_iterator->id;
            }
			return feature_id;
		}

	private:
		unsigned nIndex_;
		std::set< FeatureInfo > feature_info_set;
		std::set< FeatureInfo >::iterator feature_info_set_iterator;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> An object to encapsulate left-click events from the 
///           App's ProcessEvent calls. This helps seperate out the
///           logic to determine if the LineSegmentIntersectionTest
///           hit the registered Representation or not. This will 
///           create a FeatureInfo object for a hit Feature and then
///           Stream the associated IGW3DVectorLayerWPtr from the
///           LineSegmentIntersectionTest's IGW3DLineSegmentIntersectionReportCollection. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class DrapeLineClickedEventTask
{
	public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> DrapeLineClickedEventTask Value Constructor.                           </summary>
        ///
        /// <param name="line_segment"> The LineSegmentIntersectionTest driven by a user's
        ///                             mouse left-click event in the window. It may or may 
        ///                             not contain details about an intersection containing
        ///                             a  particular VectorLayer.                           </param>
        /// 
        /// <returns> A value constructed DrapeLineClickedEventTask with an
        ///           initialized IGW3DLineSegmentIntersectionTestWPtr.                      </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		DrapeLineClickedEventTask( Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment )
			: line_segment_intersector_ ( line_segment )
		{ }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Invoked by the App's DoPostDrawTask. App passes along whether
        ///           or not its window draw succeeded for the most recent frame.         </summary>
        /// 
        /// <remarks> This will query from the IGW3DLineSegmentIntersectionTestWPtr its
        ///           IGW3DLineSegmentIntersectionReportCollection and check via a
        ///           IGW3DLineSegmentIntersectionDetail for the hit object / feature ID.
        ///           If at least one valid IGW3DLineSegmentIntersectionDetail was found,
        ///           all will be inserted as a FeatureInfo within the
        ///           DrapedLineIntersectionTestStream and then subsequently streamed.    </remarks>
        /// 
        /// <param name="draw_succeeded"> Whether the last window frame was drawn or not. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void OnPostDraw( bool draw_succeeded )
		{
            if ( draw_succeeded )
            {
                Geoweb3d::IGW3DLineSegmentIntersectionReportCollection* intersection_report_collection = line_segment_intersector_.lock()->get_IntersectionReportCollection();

                Geoweb3d::IGW3DLineSegmentIntersectionReport* intersection_report;

                intersection_report_collection->reset();

                DrapedLineIntersectionTestStream streamer;

                while ( intersection_report_collection->next( &intersection_report ) )
                {
                    Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

                    Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

                    details_collection->reset();

                    //collect details for the intersected feature(s)
                    while ( details_collection->next( &intersection_detail ) )
                    {
                        streamer.insert( intersection_detail->get_ObjectID()
                                       , intersection_report->get_VectorRepresentation()
                                       , intersection_detail->get_IntersectionDistance() );

                        //just showing we can also access the actual intersection point
                        const Geoweb3d::GW3DPoint* hit_point = intersection_detail->get_IntersectionPoint();
                    }

                    if ( streamer.count() > 0 )
                    {
                        // process the results
                        Geoweb3d::IGW3DVectorLayerWPtr draped_line_vector_layer = intersection_report->get_VectorRepresentation().lock()->get_VectorLayer();
                        if ( auto locked_draped_line_vector_layer = draped_line_vector_layer.lock() )
                        {
                            locked_draped_line_vector_layer->Stream( &streamer );
                        }
                    }

                }
            }
		}

	private:

		Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_intersector_;

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic application class encapsulating a Geoweb3d SDK engine context,
///           a viewable window, and a camera the window relies on. This example App
///           class will exemplify how to properly instantiate an imagery / elevation
///           layer and use information from the loaded layer(s) to draw a Draped Line
///           from a layer's top-left coordinate to its bottom-right coordinate.
///           While the App runs, every 15 seconds the Draped Line's properties will
///           change and print the current property state to std::cout.                 </summary>
/// 
/// <remarks> This application inherits from the IGW3DWindowCallback so that the user
///           can extend mouse or key events to interact with the camera naviagtor and
///           the Reprensetations in the scene.                                         </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class App : public Geoweb3d::IGW3DWindowCallback
{
	public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an App instance.                        </summary>
        ///
        /// <param name="sdk_context"> A Geoweb3d SDK engine context. </param>
        /// 
        /// <returns> An instance to an App.                          </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		App( const Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context )
			: sdk_engine_context_( sdk_context )
			, navHelper_         ( new NavigationHelper )			
		{
            // Needed for the property randomization
            srand ( static_cast < unsigned > ( std::time( 0 ) ) );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Cleans up any App allocated memory. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		~App()
		{
			delete navHelper_;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback OnCreate() override.                     </summary>
        ///
        /// <remarks> This must be overridden, but is unnecessary in this example. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
		
		void OnCreate() override { }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback OnDrawBegin() override.                  </summary>
        ///
        /// <remarks> This must be overridden, but is unnecessary in this example. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void OnDrawBegin() override { }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback OnDrawEnd() override.                    </summary>
        ///
        /// <remarks> This must be overridden, but is unnecessary in this example. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void OnDrawEnd() override { }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback OnDraw2D() override.                     </summary>
        ///
        /// <remarks> This must be overridden, but is unnecessary in this example. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr& camera ) override { }
		
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback ProcessEvent() override. 
        ///           This will listen for mouse left click events on
        ///           the window in order to start a Line Intersection 
        ///           hit in an attempt to collide with a Draped Line 
        ///           Feature in order to kick off a Streamer on the hit.  </summary>
        ///
        /// <param name="win_event"> A Geoweb3d window event.
        ///                          See Geoweb3d::WindowEvent::EventType 
        ///                          for a list of window events that can
        ///                          be captured and listened to.          </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void ProcessEvent( const Geoweb3d::WindowEvent& win_event ) override
		{
			bool filter_out = false;

			switch( win_event.Type )
			{
				case win_event.MouseButtonReleased:
				{
					switch( win_event.MouseButton.button )
					{
						case Geoweb3d::Mouse::Left:
						{
							if( !line_segment_intersector_.expired() )
							{
								// Configure the pick ray from the camera's eyepoint to the mouse pixel location.
								Geoweb3d::GW3DPoint ray_start;
								Geoweb3d::GW3DPoint ray_end;

								int mouse_x = win_event.MouseButton.x;
								int mouse_y = win_event.MouseButton.y;

								Geoweb3d::IGW3DCameraController* camera_controller = camera_.lock()->get_CameraController();

								camera_controller->get_Ray( mouse_x, mouse_y, ray_start, ray_end );

								const Geoweb3d::GW3DPoint* cam_pos = camera_controller->get_Location();

								line_segment_intersector_.lock()->put_StartEnd( *cam_pos, ray_end );

								frame_task_.push_back( Geoweb3d::GW3DSharedPtr< DrapeLineClickedEventTask >( new DrapeLineClickedEventTask( line_segment_intersector_ ) ) );
							}
                            break;
						}

						default:
						    break;
					};
                    break;
                }
				
			default:
				break;
			};

			if ( !filter_out )
			{
				navHelper_->ProcessEvent( win_event, window_ );
			}
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Pumps the window draw and PostDraw tasks.                  </summary>
        ///
        /// <remarks> This will continuosuly pump so long as the draw was valid. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void Run()
        {
            bool valid = true;

            while ( valid )
            {
                valid = ( sdk_engine_context_->draw( window_ ) == Geoweb3d::GW3D_sOk );

                DoPostDrawTask( valid );
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will, for each frame task, attempt to check what was
        ///           collided with during the draw call. For geometries collided
        ///           with, it will change the PropertyCollection at a per
        ///           feature level for what was clicked via a Streamer.           </summary>
        ///
        /// <param name = "draw_successful"> Whether or not the window properly
        ///                                  completed its draw call or not.       </param>
        /// 
        /// <remarks> This will continuosuly pump so long as the draw was valid.   </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void DoPostDrawTask( bool draw_successful )
        {            
            for ( size_t i = 0; i < frame_task_.size(); ++i )
            {
                frame_task_[ i ]->OnPostDraw( draw_successful );
            }

            frame_task_.clear();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initializes the App instances' window, camera, camera navigator,
        ///           raster layer, elevation layer, Draped Line layer, features, 
        ///           representation, and Line Intersection tester.                        </summary>
        ///         
        /// <returns> true if all initializations in the summary above completed properly. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Initialize()
		{
			window_ = sdk_engine_context_->get_WindowCollection()->create_3DWindow( "MultiFeature Example", GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), this ); 
			
            // Toggle the macro definitions at the top of this file to switch between driver types for data sources.
#if SHAPEFILE_MODE
            Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "ESRI Shapefile" );
#elif GEOPACKAGE_MODE
            Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "GPKG" );
#endif

			if( driver.expired() )
			{
				std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilites example" << std::endl;
				return false;
			}

			if( !driver.lock()->get_IsCapable( "CreateDataSource" ) )
			{
				std::cout << "This driver is not capable of creating new datasources!" << std::endl;
				return false;
			}
            
            std::string data_source_name = "TestDataSource";

            // Certain data sources have a preferred extension. Most will only have a single extension though.
            // Some may not have any extension at all though, so make sure the extension collection is valid.
            Geoweb3d::IGW3DStringCollectionPtr driver_file_extensions = driver.lock()->get_FileExtentionCollection();
            if ( driver_file_extensions && driver_file_extensions->count() > 0 )
            {
                // Will append .gpkg / .shp depending on the active driver.
                data_source_name += "." + std::string( driver_file_extensions->get_AtIndex( 0 )->c_str() );
            }
            data_source_ = driver.lock()->get_VectorDataSourceCollection()->create( data_source_name.c_str() );

			if( data_source_.expired() )
			{
				std::cout << "Could not create data source." << std::endl;
				return false;
			}

            // This line_segment_intersector_ is used for doing a line intersection test from the user's left-click mouse events.
            // It will collect results about what was hit and will allow access to information about the feature hit.
			line_segment_intersector_ = sdk_engine_context_->get_LineSegmentIntersectionTestCollection()->create();

            // Loads a raster imagery and elevation layer via the imagery / elevation driver(s) 
            // that we can use to gather some latitude and longitude positions from.
            CreateImageryLayer();
            CreateElevationLayer();
            auto locked_raster_layer = imagery_layer_.lock();
            Geoweb3d::GW3DEnvelope raster_envelope;
            if ( locked_raster_layer )
            {
                raster_envelope = locked_raster_layer->get_Envelope();
            }

            double left_most_lon   = raster_envelope.MinX;
            double right_most_lon  = raster_envelope.MaxX;
            double bottom_most_lat = raster_envelope.MaxY;
            double top_most_lat    = raster_envelope.MinY;
            double line_elev       = 0.0; // no elevation to show how it implicitly drapes on any elevation data.
            double camera_elev     = 5000.0;
            double center_lon      = ( right_most_lon + left_most_lon ) / 2.0;
            double center_lat      = ( top_most_lat + bottom_most_lat ) / 2.0;

			UpdateCamera( center_lon, center_lat, camera_elev );

            // This function will exemplify how to properly add a multi-feature single-layer Draped Line Representation by:
            //    1. Querying SDK engine context for the proper VectorRepresentationDriver.
            //    2. Creating a VectorLayer off of a VectorDataSource with its corresponding VectorRepresentationDriver's default properties.
            //    3. Creating multiple Features off of a properly setup VectorLayer with default properties.
            //    4. How to use a streamer to perform an action on all features of a particular layer.
            //    5. Modifying the VectorLayer Feature's properties using a streamer when selected via a left mouse click event.
            //    6. Representing the VectorLayer by its corresponding VectorRepresentationDriver.
			AddDrapedLines( "DrapedLines", left_most_lon, top_most_lat );

			return true;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an imagery layer from a raster file. This is used so the
        ///           Draped Line layer will have positional information to draw to and from. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateImageryLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper RasterDriver for the particular data type.
            //         Auto open will attempt to use the best-match raster driver for the data type.
            Geoweb3d::GW3DResult res;
            imagery_data_source_ = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//DC_1ft_demo.ecw", res );
            if ( !imagery_data_source_.expired() )
            {
                // Step 2. Get the layer collection from the raster data source.
                Geoweb3d::IGW3DRasterLayerCollection* layer_collection = imagery_data_source_.lock()->get_RasterLayerCollection();
                if ( layer_collection && layer_collection->count() > 0 )
                {
                    // Step 3. Retrieve the layer needed for representation via the Imagery raster layer driver.
                    //         In this case, the *.ecw file used only has a single layer, so we'll retrieve just that layer.
                    imagery_layer_ = layer_collection->get_AtIndex( 0 );

                    if ( !imagery_layer_.expired() )
                    {
                        // Step 4. Retrieve the Imagery raster layer driver so the layer can be properly created and visualized.
                        imagery_rep_driver_ = sdk_engine_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Imagery" );
                        if ( !imagery_rep_driver_.expired() )
                        {
                            // Step 5. Create the actual RasterRepresentation. You can optionally capture the return of this,
                            //         but is not necessary for one-time visualization purposes, like in this example App.
                            Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
                            params.page_level = 0;
                            params.priority = 0;
                            params.representation_layer_activity = true;

                            imagery_rep_driver_.lock()->get_RepresentationLayerCollection()->create( imagery_layer_, params );
                        }
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an elevation layer from a raster file. This is used so the
        ///           Draped Line layer will have elevation information to exemplify draping. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateElevationLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper RasterDriver for the particular data type.
            //         Auto open will attempt to use the best-match raster driver for the data type.
            Geoweb3d::GW3DResult res;
            elevation_data_source_ = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//NED 10-meter DC.tif", res);
            if ( !elevation_data_source_.expired() )
            {
                // Step 2. Get the layer collection from the raster data source.
                Geoweb3d::IGW3DRasterLayerCollection* elevation_collection = elevation_data_source_.lock()->get_RasterLayerCollection();
                if ( elevation_collection && elevation_collection->count() > 0 )
                {
                    // Step 3. Retrieve the layer needed for representation via the Elevation raster layer driver.
                    //         In this case, the *.tif file used only has a single layer, so we'll retrieve just that layer.
                    elevation_layer_ = elevation_collection->get_AtIndex( 0 );

                    if ( !elevation_layer_.expired() )
                    {
                        // Step 4. Retrieve the Elevation raster layer driver so the layer can be properly created and visualized.
                        elevation_rep_driver_ = sdk_engine_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Elevation" );
                        if ( !elevation_rep_driver_.expired() )
                        {
                            // Step 5. Create the actual RasterRepresentation. You can optionally capture the return of this,
                            //         but is not necessary for one-time visualization purposes, like in this example App.
                            Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
                            params.page_level = 0;
                            params.priority = 0;
                            params.representation_layer_activity = true;

                            elevation_rep_driver_.lock()->get_RepresentationLayerCollection()->create( elevation_layer_, params );
                        }
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Sets the window's camera the a particular coordinate and elevation. </summary>
        /// 
        /// <param name="longitude"> Camera's new longitude coordinate.                   </param>
        /// 
        /// <param name="latitude">  Camera's new latitude coordinate.                    </param>
        /// 
        /// <param name="elevation"> Camera's new elevation (in meters).                  </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void UpdateCamera( double longitude, double latitude, double elevation )
		{
			if( camera_.expired() )
			{
				camera_ = window_.lock()->get_CameraCollection()->create( "Main Camera" );
				navHelper_->add_Camera(camera_);
			}

			camera_.lock()->get_CameraController()->put_Location ( longitude, latitude );
			camera_.lock()->get_CameraController()->put_Elevation( elevation, Geoweb3d::IGW3DPositionOrientation::Absolute );
            
            float pitch = 90.0f;
            camera_.lock()->get_CameraController()->put_Rotation ( 0.0f, pitch, 0.0f );
            navHelper_->put_HomePosition( camera_ );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Forwards information to CreateMultiFeatureDrapedLineLayer and to 
        ///           CreateDrapedLineRepresentation to properly create a Draped
        ///           Line layer and Representation to visualize it in the map.                  </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                    </param>
        /// 
        /// <param name="longitude"> West-most longitude of the imagery raster envelope.         </param>
        /// 
        /// <param name="latitude"> North-most latitude of the imagery raster envelope.          </param>
        /// 
        /// <returns> true when successful and false if an error occurred or something failed.   </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool AddDrapedLines( const char* layer_name, double longitude, double latitude )
        {
            // Create a new vector layer which will hold all of the geometry information.
            // Some engines serialize their data from previous runs, so sometimes a layer_name
            // that isn't unique for the first time instead will just be retrieved.
            auto layer = CreateMultiFeatureDrapedLineLayer( layer_name, longitude, latitude );

            if ( layer.expired() )
            {
                std::cout << "Error creating " << layer_name << std::endl;
                return false;
            }

            if ( !CreateDrapedLineRepresentation( layer ) )
            {
                std::cout << "Error creating " << layer_name << " representation." << std::endl;
                return false;
            }

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates rows and columns of many different Draped Line Features,
        ///           all held by a singular layer.                                              </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                    </param>
        /// 
        /// <param name="layer_lon"> West-most longitude of the imagery raster envelope.         </param>
        /// 
        /// <param name="layer_lat"> North-most latitude of the imagery raster envelope.         </param>
        /// 
        /// <returns> true when successful and false if an error occurred or something failed.   </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		Geoweb3d::IGW3DVectorLayerWPtr CreateMultiFeatureDrapedLineLayer( const char* layer_name, double layer_lon, double layer_lat )
		{
            // We're not using the field_definition for anything in particular for this example, but can be used for attribute mapping.
            // Without an initialized Geoweb3d::IGW3DDefinitionCollectionPtr, the VectorLayerCollection create call will fail for your layer, 
            // so it must be created / allocated. We also need it so we can create an empty / default property collection for the feature(s) of the layer.
            Geoweb3d::IGW3DDefinitionCollectionPtr field_definition( Geoweb3d::IGW3DDefinitionCollection::create() );

            auto driver_layer_creation_properties  = data_source_.lock()->get_Driver().lock()->get_LayerCreateOptions()->create_Clone();
            auto driver_layer_creation_definitions = driver_layer_creation_properties->get_DefinitionCollection();

            // By default, GeoPackages do not allow creation of the same layer more than once with the same name.
            // This should allow the layer creation to overwrite the previous one for GeoPackages. The Shapefile driver doesn't have this issue.
#if GEOPACKAGE_MODE
            driver_layer_creation_properties->put_Property( driver_layer_creation_definitions->get_IndexByName( "OVERWRITE" ), "YES" );
#endif
            
            // For Draped Lines, you should use Geoweb3d::gtLINE_STRING_25D. 
            // This will create the layer to be used for the feature creation.
            Geoweb3d::IGW3DVectorLayerWPtr layer = data_source_.lock()->get_VectorLayerCollection()->create( layer_name, Geoweb3d::gtLINE_STRING_25D, field_definition, Geoweb3d::IGW3DPropertyCollectionPtr() );

            // Some drivers have persistence which will allow layers to be loaded into memory implicitly when instantiating said driver.
            // A layer may fail to create if its name is not unique and was already created in a said previous run.
            if( !layer.expired() )
			{
				const unsigned int NUM_ROWS = 4;
				const unsigned int NUM_COLS = 8;
				const unsigned int NUM_FEATURES = NUM_ROWS * NUM_COLS;

				const double inter_feature_gap = 0.0075;

                const double start_lon = layer_lon;

                double lon = layer_lon;
                double lat = layer_lat;
				
				Geoweb3d::GW3DResult result;							

                Geoweb3d::IGW3DPropertyCollectionPtr default_property_collection = field_definition->create_PropertyCollection();

				for ( unsigned int i = 0; i < NUM_FEATURES; ++i )
				{					
					if ( i > 0 && ( i % NUM_COLS == 0 ) )
					{
					    lon =  start_lon;
                        lat -= inter_feature_gap;
					}

					Geoweb3d::GW3DPoint start_point( lon, lat );
                    Geoweb3d::GW3DPoint end_point  ( lon, lat - inter_feature_gap );
                    Geoweb3d::GW3DLineString line_geometry;
                    line_geometry.put_CoordinateDimension( 2 );
                    line_geometry.put_NumPoints( 2 );
                    line_geometry.put_Point( 0, &start_point );
                    line_geometry.put_Point( 1, &end_point );

                    // This will create a feature per iteration. We'll modify the properties per-feature later in the streamer(s).
                    layer.lock()->create_Feature( default_property_collection, &line_geometry, result );

					lon += inter_feature_gap;

				}
            }

			return layer;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Takes a Vector Layer and creates a Representation from it.
        ///           This will also exemplify how to properly retrieve the default
        ///           property collection from a respective Representation Driver
        ///           that's used during the creation process. Additionally, this
        ///           will exemplify how you can Stream a layer to access / manipulate
        ///           all of its features' properties properly.                         </summary>
        /// 
        /// <param name="layer"> The Vector Layer that's been properly initialized.
        ///                      This will be used directly as a part of the
        ///                      Representation Driver's creation process to produce
        ///                      the Representation. This layer should have more than
        ///                      one feature.                                           </param>
        /// 
        /// <returns> true if the Representation was properly created, otherwise false. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CreateDrapedLineRepresentation( Geoweb3d::IGW3DVectorLayerWPtr layer )
		{
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr rep_driver = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "DrapedLine" );

			if( rep_driver.expired() )
			{
				std::cout << "Error creating vector representation driver." << std::endl;
				return false;
			}

			Geoweb3d::GW3DResult can_represent;
			can_represent = rep_driver.lock()->get_CapabilityToRepresent( layer );

			if( !Geoweb3d::Succeeded( can_represent ) )
			{
				std::cout << "Error: The selected representation driver cannot represent this layer." << std::endl;
				return false;
			}

			Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;
			rep_layer_creation_params.page_level = 1;
            rep_layer_creation_params.representation_default_parameters = rep_driver.lock()->get_PropertyCollection()->create_Clone();

            Geoweb3d::IGW3DVectorRepresentationWPtr new_representation = rep_driver.lock()->get_RepresentationLayerCollection()->create( layer, rep_layer_creation_params );
            if ( auto locked_rep = new_representation.lock() )
            {
                if ( auto locked_vector_layer = locked_rep->get_VectorLayer().lock() )
                {
                    // Think of a streamer similarly to a vertex shader. For each vertex you can do an
                    // operation on said vertex. Streamers let you did the same, but at a per-feature level of a layer.
                    // This streamer is custom-made for just randomzing a set of properties on all features of a layer.
                    // Streamers are a more efficient way to modify properties of existing features, so this
                    // is an example on how you can make a custom streamer. It can be made super general or 
                    // very specific. We'll make a very specific, but simple streamer for this.
                    // This will invoke FeaturePropertyRandomizerStream::OnStream.
                    FeaturePropertyRandomizerStream property_randomizer_stream( new_representation );
                    locked_vector_layer->Stream( &property_randomizer_stream );
                }
            }

            // Add the new representation to the line_segment intersection test for hit tests.
            if ( !line_segment_intersector_.expired() )
            {
                line_segment_intersector_.lock()->get_VectorRepresentationCollection()->add( new_representation );
            }

			return true;
		}

	private:

        typedef std::deque< Geoweb3d::GW3DSharedPtr< DrapeLineClickedEventTask > > ClickEventTask;

		Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_engine_context_;
        Geoweb3d::IGW3DWindowWPtr     window_;
        Geoweb3d::IGW3DCameraWPtr     camera_;
        NavigationHelper*             navHelper_;

        Geoweb3d::IGW3DVectorDataSourceWPtr data_source_;

        Geoweb3d::IGW3DRasterRepresentationDriverWPtr imagery_rep_driver_;
        Geoweb3d::IGW3DRasterDataSourceWPtr           imagery_data_source_;
        Geoweb3d::IGW3DRasterLayerWPtr                imagery_layer_;

        Geoweb3d::IGW3DRasterRepresentationDriverWPtr elevation_rep_driver_;
        Geoweb3d::IGW3DRasterDataSourceWPtr           elevation_data_source_;
        Geoweb3d::IGW3DRasterLayerWPtr                elevation_layer_;

		Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_intersector_;        
        ClickEventTask                                 frame_task_;

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Runs the MultiFeature Draped Line Application instance.
///           This will continuosly run until the window is closed.   </summary>
/// 
/// <param name="sdk_context"> The Geoweb3d engine context.           </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	App app( sdk_context );

	if( app.Initialize() )
	{
		app.Run();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Initializes the Geowebd3d engine SDK context. 
///           Initializes SDK configuration.                
///           Runs the Draped Line example application.            </summary>
/// 
/// <param name="argc"> Number of command line arguments.           </param>
/// 
/// <param name="argv"> The contents of the command line arguments. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling();

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
			std::cout << "\nError creating SDK context! Press any key to continue...\n" << std::endl;
            getchar();
        }
    }
    else
    {
        return -1;
    }
   
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Function handler for fatal errors inside the engine.
///           For the purposes of this demo, we only print to the 
///           end user what went wrong.                             </summary>
/// 
/// <param name="msg"> Geoweb3d::Fatal categorized messages.        </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_fatal_function(const char *msg )
{
    std::cout << "Fatal Info: " << msg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> General info function, this will be used to
///           print any general info and non-fatal warnings.                           </summary>
/// 
/// <param name="msg"> Geoweb3d::Information / Geoweb3d::Warning categorized messages. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_info_function( const char* msg )
{
    std::cout << "General Info: " << msg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Maps GeoWeb3d_InfoType information warnings to a function. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetInformationHandling()
{
    // Tell the engine about our error function handling.
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information, my_info_function  );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,     my_info_function  );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,       my_fatal_function );
}
