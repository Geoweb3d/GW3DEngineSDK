#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DImageCollection.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"

#include "Geoweb3dCore/LayerParameters.h"
#include "GeoWeb3dCore/SystemExports.h"

// A simple example of navigation control
#include "../Common/NavigationHelper.h"

#include <set>
#include <map>
#include <deque>
#include <iostream>
#include <tchar.h>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )


void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Helper class for bundling information about a streamed VectorLayer. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct FeatureInfo
{
	long id;
    Geoweb3d::IGW3DVectorRepresentationDriverWPtr rep_driver;
    Geoweb3d::IGW3DVectorLayerWPtr                rep_vector_layer;
    Geoweb3d::IGW3DVectorRepresentationWPtr       vector_rep;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Default Constructor.                       </summary>
    ///
    /// <returns> A constructed FeatureInfo with an invalid  
    ///           feature ID and no distance from camera.    </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FeatureInfo () : id( -1 )
    { }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Value Constructor for the feature of a layer hit in a 
    ///           representation.                                            </summary>
    /// 
    /// <param name="object_id"> Feature ID, unique to the layer.            </param>
    /// 
    /// <returns> A constructed FeatureInfo with a valid feature ID 
    ///           and distance from camera.                                  </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	FeatureInfo ( long object_id )
		: id( object_id )
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
/// <summary> Custom stream object for modifying a feature's geometry. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class TexturedDrapedRingStream : public Geoweb3d::IGW3DVectorLayerStream
{
	public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Enum controls for the Textured Draped Ring's translation mode during OnStream. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////////

        enum ObjectMovementMode
        {
            CAMERA_POS           = 1, // -90 degrees from camera's current angle, it will ignore pitch.
            CAMERA_POS_AND_PITCH = 2  // Takes pitch into account, will use vector data to determine z coordinates.
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Value Constructor for the TexturedDrapedRingStream.                       </summary>
        ///
        /// <param name="camera"> Camera object this streamer will use for positional queries.  </param>
        /// 
        /// <returns> A Value Constructed TexturedDrapedRingStream.                             </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        TexturedDrapedRingStream( Geoweb3d::IGW3DCameraWPtr camera = Geoweb3d::IGW3DCameraWPtr() )
            : dragging_( false )
            , mode_    ( ObjectMovementMode::CAMERA_POS )
            , camera_  ( camera )
            
		{
			reset();
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Destructor for any local class cleanup. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual ~TexturedDrapedRingStream( )
		{
			feature_info_set_.clear();
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Post-construction camera setter.                                         </summary>
        ///
        /// <param name="camera"> Camera object this streamer will use for positional queries. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void SetCamera( Geoweb3d::IGW3DCameraWPtr camera )
        {
            camera_ = camera;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> In this example, this will be called after the VectorRepresentation for
        ///           for the Textured Draped Ring is created by the App instance. The single
        ///           layer will be added and streamed every frame.                               </summary>
        ///
        /// <param name="object_id"> Zero-based index of the feature/object id.                   </param>
        /// 
        /// <param name="vector_rep"> The VectorRepresentation containing the layer and features. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void insert( long object_id, Geoweb3d::IGW3DVectorRepresentationWPtr vector_rep )
		{
			FeatureInfo finfo( object_id );
            finfo.vector_rep = vector_rep;
            // So long as the Vector Representation was valid, the other components should be as well.
            // We will add it for streaming and manipulate the state in OnStream.
            if( auto locked_rep_vector = finfo.vector_rep.lock() )
            {
                finfo.rep_driver       = locked_rep_vector->get_Driver();
                finfo.rep_vector_layer = locked_rep_vector->get_VectorLayer();
                finfo.rep_vector_layer.lock()->get_RepresentationLayerCollection();
                feature_info_set_.insert( finfo );
                dragging_ = true;
            }
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Interface to access a FeatureInfo object, rather than its iterator. </summary>
        ///
        /// <param name="object_id"> Zero-based index of the feature/object id.           </param>
        /// 
        /// <param name="feature_info"> [out] The FeatureInfo for the object_id.          </param>
        ///                         
        /// <returns> true if the object_id correlates to a FeatureInfo object.
        ///           The state of the feature_info param is only guaranteed
        ///           valid for the object_id if this returns true.                       </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool find( long object_id, FeatureInfo& feature_info )
		{
            bool found = false;
            if ( found = feature_info_set_.find( object_id ) != feature_info_set_.end() )
            {
                feature_info = *feature_info_set_.find( object_id );
            }
            return found;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Helper function to perform a Geodetic Query using the camera_'s
        ///           center coordinate. Depending on which mode_ this streamer is in
        ///           will determine which QueryFilerType is used. This will in-place
        ///           modify the "point" param and should be reflected by the next
        ///           frame draw.                                                                </summary>
        ///
        /// <param name="pixel_query"> The pixel query that was created from the camera_ member.
        ///                            Note: A query like this MUST persist from frame to frame,
        ///                            it is more like a polling object that can be queried 
        ///                            after a full frame has completed.                         </param>
        /// 
        /// <param name="point"> The current, modifiable, geometry of an
        ///                      IGW3DVectorLayerStreamResult::get_EditableGeometry.
        ///                      Changes made to this parameter will be reflected
        ///                      as an update on the GPU.                                        </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CenterPointToCenterWindowCoordinate( Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr pixel_query, Geoweb3d::GW3DPoint* point )
        {
            if ( auto locked_pixel_query = pixel_query.lock() )
            {
                auto geodetic_point = locked_pixel_query->get_GeodeticResult();
                point->put_X( geodetic_point->get_X() );
                point->put_Y( geodetic_point->get_Y() );
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Invoked when Stream is called. This streamer does a few things:
        ///               1. Find the object ID of the result in our local feature_info_set_.
        ///               2. Retrieve the editable geometry from the "result" param.
        ///               3. Modify the geometry of the "result" depending on which mode
        ///                  the streamer is currently in.                                   </summary>
        ///
        /// <param name="result"> The IGW3DVectorLayerStreamResult which 
        ///                       contains certain information about the 
        ///                       current Feature being iterated over. Its
        ///                       geometry will be modified in-place.                        </param>    
        /// 
        /// <returns> Whether or not the properties of the current Feature were printed
        ///           and its geometry modified.                                             </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult* result ) override
		{
            bool successful = false;
			long object_id  = result->get_ObjectID();
            FeatureInfo finfo;
            if( find( object_id, finfo ) )
            {
                // On initial click / drag we'll change the position of the object to the cursor or 
                // camera's center location, depending on if the dragging is active or not.
                // Outside interfaces will simply call the streamer to invoke this when necessary.
                Geoweb3d::GW3DPoint* point_geometry = dynamic_cast< Geoweb3d::GW3DPoint* >( result->get_EditableGeometry() );
                if( point_geometry != nullptr )
                {
                    successful = true;
                    switch( mode_ )
                    {
                        case CAMERA_POS:
                        {   
                            auto camera_location = camera_.lock()->get_CameraController()->get_Location();
                            point_geometry->put_X( camera_location->get_X() );
                            point_geometry->put_Y( camera_location->get_Y() );
                            break;
                        }
                        case CAMERA_POS_AND_PITCH:
                        {
                            // As noted above, the pixel_query object must persist from frame to frame,
                            // so we must create and persist a query object via the camera's geodetic collection.
                            Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr pixel_query;
                            if ( camera_central_pixel_queries_.count( camera_ ) == 0 )
                            {                                    
                                auto pixel_query = camera_central_pixel_queries_[ camera_ ] = camera_.lock()->get_WindowCoordinateToGeodeticQueryCollection()->create();
                                // First-time setup for the pixel query so that the query returns the center coordinate of the camera.
                                if ( auto locked_pixel_query = pixel_query.lock() )
                                {
                                    int x_coord         = 0;
                                    int y_coord         = 0;
                                    unsigned int width  = 0;
                                    unsigned int height = 0;
                                    camera_.lock()->get_Window().lock()->get_WindowSize( x_coord, y_coord, width, height );
                                    locked_pixel_query->put_Enabled( true );
                                    locked_pixel_query->put_WindowCoordinateOrigin( Geoweb3d::WindowCoordinateOrigin::LOWER_LEFT );
                                    locked_pixel_query->put_QueryFilter( Geoweb3d::IGW3DWindowCoordinateToGeodeticQuery::QueryFilterType::TERRAIN_ONLY );
                                    locked_pixel_query->put_WindowCoordinate( width / 2, height / 2 );
                                }
                            }
                            pixel_query = camera_central_pixel_queries_[ camera_ ];
                            
                            CenterPointToCenterWindowCoordinate( pixel_query, point_geometry );
                            
                            break;
                        }
                        default:
                            break;
                    }
                }
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
        /// <returns> The size of the feature_info_set_.          </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned long count() const override
		{
			return static_cast<unsigned long>( feature_info_set_.size() );
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
                *ppVal = feature_info_set_iterator_->id;
                ++feature_info_set_iterator_;
                successful = true;
            }
            return successful;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Resets the iterator. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void reset() override
		{
			nIndex_ = 0;
			feature_info_set_iterator_ = feature_info_set_.begin();
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Array indexer operator. Gets feature/object id at index. </summary>
        ///
        /// <remarks> For the most part, this can usually forward the call to
        ///           get_AtIndex with the index parameter being passed along. </remarks>
        ///
        /// <param name="index"> Zero-based index of the feature/object id.    </param>
        ///
        /// <returns> The feature/object id.                                   </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int64_t operator[]( unsigned long index ) override
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
                std::set< FeatureInfo >::iterator temp_feature_info_set_iterator = feature_info_set_.begin();
                std::advance( temp_feature_info_set_iterator, index );
                feature_id = temp_feature_info_set_iterator->id;
            }
            return feature_id;
		}

    public:

        bool                           dragging_;
        ObjectMovementMode             mode_;
        Geoweb3d::IGW3DVectorLayerWPtr textured_draped_ring_vector_layer_being_dragged_;        

	private:

		unsigned int              nIndex_;
        Geoweb3d::IGW3DCameraWPtr camera_;
		std::set< FeatureInfo >   feature_info_set_;
		std::set< FeatureInfo >::iterator feature_info_set_iterator_;
        std::map< Geoweb3d::IGW3DCameraWPtr, Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr> camera_central_pixel_queries_;

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic application class encapsulating a Geoweb3d SDK engine context,
///           a viewable window, and a camera the window relies on. This example App
///           class will exemplify how to properly instantiate an imagery / elevation
///           layer and use information from the loaded layer(s) to load a Textured
///           Draped Ring. The Draped Ring will have two different movement modes,
///           controlled via the numerical "1" and "2" keys on the keyboard. 
///           The Texture Draped Ring will always be tethered to the camera.            </summary>
/// 
/// <remarks> This application inherits from the IGW3DWindowCallback so that the user
///           can extend mouse or key events to interact with the camera navigator and
///           the Representations in the scene.                                         </remarks>
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
			, navHelper_(new NavigationHelper() )
		{ }

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

		void OnDrawEnd() override
        {
            if ( streamer_.count() > 0 && streamer_.dragging_ && !streamer_.textured_draped_ring_vector_layer_being_dragged_.expired() )
            {
                if ( auto locked_textured_draped_ring_vector_layer = streamer_.textured_draped_ring_vector_layer_being_dragged_.lock() )
                {
                    locked_textured_draped_ring_vector_layer->Stream( &streamer_ );
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback OnDraw2D() override.                     </summary>
        ///
        /// <remarks> This must be overridden, but is unnecessary in this example. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr& camera ) override { }
		
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback ProcessEvent() override.
        ///           Also the numerical "1" and "2" keys will
        ///           alter the translation mode of the tracking texture.      </summary>
        ///
        /// <param name="win_event"> A Geoweb3d window event.
        ///                          See Geoweb3d::WindowEvent::EventType 
        ///                          for a list of window events that can
        ///                          be captured and listened to.              </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void ProcessEvent( const Geoweb3d::WindowEvent& win_event ) override
		{
			bool filter_out = false;

			switch( win_event.Type )
			{
                case win_event.KeyPressed:
                {
                    switch ( win_event.Key.code )
                    {
                        case Geoweb3d::Key::Code::Num1:
                            std::cout << "Changing translation mode to CAMERA_POS." << std::endl;
                            streamer_.mode_ = TexturedDrapedRingStream::ObjectMovementMode::CAMERA_POS;
                            break;
                        case Geoweb3d::Key::Code::Num2:
                            std::cout << "Changing translation mode to CAMERA_POS_AND_PITCH." << std::endl;
                            streamer_.mode_ = TexturedDrapedRingStream::ObjectMovementMode::CAMERA_POS_AND_PITCH;
                            break;
				        default:
						    break;
                    }
                    break;
                }
			    default:
				    break;
			}

			if (!filter_out)
			{
				navHelper_->ProcessEvent(win_event, window_);
			}
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Pumps the window draw and PostDraw tasks.                  </summary>
        ///
        /// <remarks> This will continuously pump so long as the draw was valid. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void Run()
        {
            bool valid = true;

            while ( valid )
            {
                valid = ( sdk_engine_context_->draw( window_ ) == Geoweb3d::GW3D_sOk );
                textured_draped_ring_representation_.lock()->get_VectorLayer().lock()->Stream( &streamer_ );
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initializes the App instances' window, camera, camera navigator,
        ///           raster layer, elevation layer, Textured Draped Ring layer, features, 
        ///           representation, and a streamer.                                      </summary>
        ///         
        /// <returns> true if all initializations in the summary above completed properly. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Initialize()
		{
			window_ = sdk_engine_context_->get_WindowCollection()->create_3DWindow( "Moving Textured Draped Ring", GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), this ); 
			
            Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );
            
			if( driver.expired() )
			{
				std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilities example" << std::endl;
				return false;
			}

			if( !driver.lock()->get_IsCapable( "CreateDataSource" ) )
			{
				std::cout << "This driver is not capable of creating new datasources!" << std::endl;
				return false;
			}

            data_source_ = driver.lock()->get_VectorDataSourceCollection()->create( "TestDataSource" );
			if( data_source_.expired() )
			{
				std::cout << "Could not create data source." << std::endl;
				return false;
			}

            // Loads a raster imagery layer and an ExtrudedPolygon Representation via the imagery / VectorRepresentation
            // driver(s) that we can use to gather some latitude and longitude positions from and display the textured draped ring.
            CreateImageryLayer();
            CreateElevationLayer();
            CreateExtrudedPolygonLayer();
            auto locked_raster_layer = imagery_layer_.lock();
            Geoweb3d::GW3DEnvelope raster_envelope;
            if ( locked_raster_layer )
            {
                raster_envelope = locked_raster_layer->get_Envelope();
            }

			double longitude   = ( raster_envelope.MaxX + raster_envelope.MinX ) * 0.5;
			double latitude    = ( raster_envelope.MaxY + raster_envelope.MinY ) * 0.5;
			double camera_elev = 500.0;

			UpdateCamera( longitude, latitude, camera_elev );

            // This example app's intended purpose is to show how a single layer can create a Textured Draped Ring
            // and then select it in the scene to move it around with a couple of different translation modes.
			AddTexturedDrapedRing( "EditableTexturedDrapedRing", longitude, latitude );

			return true;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an imagery layer from a raster file. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateImageryLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper RasterDriver for the particular data type.
            //         Auto open will attempt to use the best-match raster driver for the data type.
            Geoweb3d::GW3DResult result;
            Geoweb3d::IGW3DRasterDataSourceWPtr imagery_data_source = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//DC_1ft_demo.ecw", result );
            if ( Geoweb3d::Succeeded( result ) && !imagery_data_source.expired() )
            {
                // Step 2. Get the layer collection from the raster data source.
                Geoweb3d::IGW3DRasterLayerCollection* layer_collection = imagery_data_source.lock()->get_RasterLayerCollection();
                if ( layer_collection && layer_collection->count() > 0 )
                {
                    // Step 3. Retrieve the layer needed for representation via the Imagery raster layer driver.
                    //         In this case, the *.ecw file used only has a single layer, so we'll retrieve just that layer.
                    imagery_layer_ = layer_collection->get_AtIndex( 0 );

                    if ( !imagery_layer_.expired() )
                    {
                        // Step 4. Retrieve the Imagery raster layer driver so the layer can be properly created and visualized.
                        Geoweb3d::IGW3DRasterRepresentationDriverWPtr imagery_rep_driver = sdk_engine_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Imagery" );
                        if ( !imagery_rep_driver.expired() )
                        {
                            // Step 5. Create the actual RasterRepresentation. You can optionally capture the return of this,
                            //         but is not necessary for one-time visualization purposes, like in this example App.
							Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
							params.page_level = 0;
							params.priority = 0;
							params.representation_layer_activity = true;
                            imagery_rep_driver.lock()->get_RepresentationLayerCollection()->create( imagery_layer_, params);
                        }
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an elevation layer from a raster file. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateElevationLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper RasterDriver for the particular data type.
            //         Auto open will attempt to use the best-match raster driver for the data type.
            Geoweb3d::GW3DResult result;
            Geoweb3d::IGW3DRasterDataSourceWPtr elevation_data_source = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//NED 10-meter DC.tif", result );
            if ( Geoweb3d::Succeeded( result ) && !elevation_data_source.expired() )
            {
                // Step 2. Get the layer collection from the raster data source.
                Geoweb3d::IGW3DRasterLayerCollection* elevation_collection = elevation_data_source.lock()->get_RasterLayerCollection();
                if ( elevation_collection && elevation_collection->count() > 0 )
                {
                    // Step 3. Retrieve the layer needed for representation via the Elevation raster layer driver.
                    //         In this case, the *.tif file used only has a single layer, so we'll retrieve just that layer.
                    Geoweb3d::IGW3DRasterLayerWPtr elevation_layer = elevation_collection->get_AtIndex( 0 );

                    if ( !elevation_layer.expired() )
                    {
                        // Step 4. Retrieve the Elevation raster layer driver so the layer can be properly created and visualized.
                        Geoweb3d::IGW3DRasterRepresentationDriverWPtr elevation_rep_driver = sdk_engine_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Elevation" );
                        auto property_collection   = elevation_rep_driver.lock()->get_PropertyCollection()->create_Clone();
                        auto definition_collection = property_collection->get_DefinitionCollection();
                        // We'll add some vertical scale to the elevation layer so it's more pronounced to see its effects.
                        property_collection->put_Property(Geoweb3d::Raster::ElevationParameters::VERTICAL_SCALE, 10.0 );

                        if ( !elevation_rep_driver.expired() )
                        {
                            // Step 5. Create the actual RasterRepresentation. You can optionally capture the return of this,
                            //         but is not necessary for one-time visualization purposes, like in this example App.
							Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
							params.page_level = 6;
							params.priority = 0;
							params.representation_layer_activity = true;
                            elevation_rep_driver.lock()->get_RepresentationLayerCollection()->create( elevation_layer, params);
                        }
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an extruded polygon layer and Representation from a shape file. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateExtrudedPolygonLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper VectorDriver for the particular data type.
            //         Auto open will attempt to use the best-match vector driver for the data type.
            Geoweb3d::GW3DResult result;
            Geoweb3d::IGW3DVectorDataSourceWPtr building_data_source = sdk_engine_context_->get_VectorDriverCollection()->auto_Open( "..//examples//media//Building Footprints.shp", result );
            if ( Geoweb3d::Succeeded( result ) && !building_data_source.expired() )
            {
                // Step 2. Get the VectorLayerCollection from the VectorDataSource.
                Geoweb3d::IGW3DVectorLayerCollection* building_layer_collection = building_data_source.lock()->get_VectorLayerCollection();
                if ( building_layer_collection && building_layer_collection->count() > 0 )
                {
                    // Step 3. Retrieve the layer needed for representation via the VectorRepresentationDriver.
                    //         In this case, the *.shp file used only has a single layer, so we'll retrieve just that layer.
                    auto building_layer_ = building_layer_collection->get_AtIndex( 0 );

                    if ( !building_layer_.expired() )
                    {
                        // Step 4. Retrieve the ExtrudedPolygon VectorRepresentationDriver to visualize in the scene the .shp file contents.
                        Geoweb3d::IGW3DVectorRepresentationDriverWPtr extruded_polygon_driver = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPolygon" );                        
                        if ( !extruded_polygon_driver.expired() )
                        {
                            Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;
                            rep_layer_creation_params.page_level = 1;

                            // Step 5. Create the actual Representation. You can optionally capture the return of this,
                            //         but is not necessary for one-time visualization purposes, like in this example App.
                            extruded_polygon_driver.lock()->get_RepresentationLayerCollection()->create( building_layer_, rep_layer_creation_params );
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
                streamer_.SetCamera( camera_ );
				navHelper_->add_Camera( camera_ );
			}

			camera_.lock()->get_CameraController()->put_Location ( longitude, latitude );
			camera_.lock()->get_CameraController()->put_Elevation( elevation, Geoweb3d::IGW3DPositionOrientation::Absolute );
            float pitch = 90.0f;
			camera_.lock()->get_CameraController()->put_Rotation ( 0.0f, pitch, 0.0f );
            navHelper_->put_HomePosition( camera_ );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Forwards information to CreateTexturedDrapedRingLayer and to 
        ///           CreateTexturedDrapedRingRepresentation to properly create a 
        ///           Textured Draped Ring layer and Representation to visualize it in the map. </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                   </param>
        /// 
        /// <param name="longitude"> Center longitude of the imagery raster envelope.           </param>
        /// 
        /// <param name="latitude"> Center latitude of the imagery raster envelope.             </param>
        /// 
        /// <returns> true when successful and false if an error occurred or something failed.  </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool AddTexturedDrapedRing( const char* layer_name, double longitude, double latitude )
        {
            Geoweb3d::IGW3DVectorLayerWPtr textured_draped_ring_layer = CreateTexturedDrapedRingLayer( layer_name, longitude, latitude );            

            if ( textured_draped_ring_layer.expired() )
            {
                std::cout << "Error creating " << layer_name << std::endl;
                return false;
            }

            if ( !CreateTexturedDrapedRingRepresentation( textured_draped_ring_layer ) )
            {
                std::cout << "Error creating " << layer_name << " representation." << std::endl;
                return false;
            }

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates a Textured Draped Ring Feature held by a singular layer.         </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                  </param>
        /// 
        /// <param name="layer_lon"> Center longitude of the imagery raster envelope.          </param>
        /// 
        /// <param name="layer_lat"> Center latitude of the imagery raster envelope.           </param>                
        /// 
        /// <returns> true when successful and false if an error occurred or something failed. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		Geoweb3d::IGW3DVectorLayerWPtr CreateTexturedDrapedRingLayer( const char* layer_name, double layer_lon, double layer_lat )
		{
            // We're not using the field_definition for anything in particular for this example, but can be used for attribute mapping.
            // Without an initialized Geoweb3d::IGW3DDefinitionCollectionPtr, the VectorLayerCollection create call will fail for your layer, 
            // so it must be created / allocated. We also need it so we can create an empty / default property collection for the feature of the layer.
            Geoweb3d::IGW3DDefinitionCollectionPtr field_definition( Geoweb3d::IGW3DDefinitionCollection::create() );
            
            // Since we're moving a Draped Ring via its center point, a gtPOINT geometry type is required.            
            Geoweb3d::IGW3DVectorLayerWPtr layer = data_source_.lock()->get_VectorLayerCollection()->create( layer_name, Geoweb3d::gtPOINT, field_definition );
            
            // We need a default property collection to create the layer.
            Geoweb3d::IGW3DPropertyCollectionPtr default_property_collection( field_definition->create_PropertyCollection() );

            // This will be where the Textured Draped Ring's location will be.
            Geoweb3d::GW3DPoint textured_draped_ring_point( layer_lon, layer_lat, 0.0 );

            Geoweb3d::GW3DResult result;
            layer.lock()->create_Feature( default_property_collection, &textured_draped_ring_point, result );
            layer.lock()->put_GeometryEditableMode( true );

            return layer;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Takes a Vector Layer and creates a Representation from it.
        ///           This will show how to create a Textured Draped Ring and assign
        ///           a path to read texture data from and visualize in the scene.      </summary>
        /// 
        /// <param name="layer"> The Vector Layer that's been properly initialized.
        ///                      This will be used directly as a part of the
        ///                      Representation Driver's creation process to produce
        ///                      the Representation.                                    </param>
        /// 
        /// <returns> true if the Representation was properly created, otherwise false. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CreateTexturedDrapedRingRepresentation( Geoweb3d::IGW3DVectorLayerWPtr layer )
		{
            // For Draped Ring there is a "DrapedRing" driver.
            Geoweb3d::IGW3DVectorRepresentationDriverWPtr textured_draped_ring_rep_driver = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "DrapedRing" );

            // We'll add some default values for the DrapedRing's Representation creation parameters.
            Geoweb3d::IGW3DPropertyCollectionPtr   properties            = textured_draped_ring_rep_driver.lock()->get_PropertyCollection()->create_Clone();
            Geoweb3d::IGW3DDefinitionCollectionPtr definition_collection = properties->get_DefinitionCollection()->create_Clone();

            properties->put_Property( definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET" ),   0.0 );
            properties->put_Property( definition_collection->get_IndexByName( "TEXTURE_PALETTE_INDEX" ),    0 );
            properties->put_Property( definition_collection->get_IndexByName( "TEXTURE_TO_COLOR_BLEND" ), 0.5 );
            properties->put_Property( definition_collection->get_IndexByName( "INNER_RAD" ),              0.0 );
            properties->put_Property( definition_collection->get_IndexByName( "RED" ),   1.0 );
            properties->put_Property( definition_collection->get_IndexByName( "GREEN" ), 1.0 );
            properties->put_Property( definition_collection->get_IndexByName( "BLUE" ),  1.0 );

			Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;
			rep_layer_creation_params.page_level = 1;
            rep_layer_creation_params.representation_default_parameters = properties;

            // This will create the VectorRepresentation with the above set properties.
            textured_draped_ring_representation_ = textured_draped_ring_rep_driver.lock()->get_RepresentationLayerCollection()->create( layer, rep_layer_creation_params );
            
            // To add an image collection to the Representation, query for the IGW3DSceneGraphContext and create a new IGW3DImageCollection.
            // Off of the image collection we can add a new texture at palette index 0.
            Geoweb3d::IGW3DImageCollectionPtr image_collection = sdk_engine_context_->get_SceneGraphContext()->create_ImageCollection();
            image_collection->create( "..//examples//media//Texture//bullseye_white.png" );
            textured_draped_ring_representation_.lock()->put_GW3DFinalizationToken( image_collection->create_FinalizeToken() );
            
            // This example will always be assigning the DrapedRing's center position to the camera's location.
            streamer_.textured_draped_ring_vector_layer_being_dragged_ = textured_draped_ring_representation_.lock()->get_VectorLayer();
            streamer_.dragging_ = true;
            streamer_.SetCamera( camera_ );
            streamer_.insert( 0, textured_draped_ring_representation_ );

			return true;
		}

	private:

		Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_engine_context_;
        Geoweb3d::IGW3DWindowWPtr     window_;
        Geoweb3d::IGW3DCameraWPtr     camera_;
        NavigationHelper*             navHelper_;

        Geoweb3d::IGW3DVectorDataSourceWPtr data_source_;
        
        Geoweb3d::IGW3DRasterLayerWPtr imagery_layer_;

        Geoweb3d::IGW3DVectorRepresentationWPtr textured_draped_ring_representation_;
        TexturedDrapedRingStream  streamer_;
        
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Runs the Moving Textured Draped Ring Application instance.
///           This will continuously run until the window is closed.      </summary>
/// 
/// <param name="sdk_context"> The Geoweb3d engine context.              </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    App app( sdk_context );

    if ( app.Initialize() )
    {
        app.Run();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Initializes the Geowebd3d engine SDK context. 
///           Initializes SDK configuration.                
///           Runs the Moving Textured Draped Ring example application. </summary>
/// 
/// <param name="argc"> Number of command line arguments.               </param>
/// 
/// <param name="argv"> The contents of the command line arguments.     </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling();

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if ( sdk_context )
    {
        Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
        if ( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
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

void my_fatal_function( const char* msg )
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
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information, my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning, my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal, my_fatal_function );
}