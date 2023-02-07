#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "common/GW3DCommon.h"
#include "common/IGW3DString.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"
#include "engine/IGW3DPositionOrientation.h"

#include "Geoweb3dCore/LayerParameters.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "GeoWeb3dCore/GeometryExports.h"

// A simple example of navigation control
#include "../Common/NavigationHelper.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <tchar.h>

#include <string.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )

void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic struct that encapsulates all the information someone may need to
///           display search results after a search has been completed and needs to be
///           stored for future use.                                                   </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct GeocodeStreamResultInformation
{
    Geoweb3d::GW3DEnvelope envelope;
    std::string            display_name;
    std::string            street;
    std::string            city;
    std::string            state;
    std::string            postcode;
    std::string            country;

    friend std::ostream& operator<<( std::ostream& os, const GeocodeStreamResultInformation& geocode_result_information )
    {
        os << geocode_result_information.display_name << std::endl;
        return os;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A VectorLayerStreamer subclass that can extract the Geocoding search
///           results. Each VectorLayer being streamed will correlate to a single result. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class GeocodeStream : public Geoweb3d::IGW3DVectorLayerStream
{

public: 

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Creates an instance for a Geocode Stream. Keep this object alive
    ///           if trying to collect multiple layers' worth of search results. </summary>
    /// 
    /// <returns> An instance to a GeocodeStream.                                </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GeocodeStream() { }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> A helper function for extracting the Geocoder's query for a bounding box. 
    ///           This will be used in conjunction with an IGW3DCamera::ensure_Visible call
    ///           to determine a proper eyepoint to set the camera to such that the entire
    ///           bounding box will be visible.                                                </summary>
    /// 
    /// <param name = "bounding_box_string"> The stringified bounding box returned by the
    ///                Geocoder with the following format: "min_lat,max_lat,min_lon,max_lon"   </param>
    /// 
    /// <returns> Returns a GW3DEnvelope populated with the bounding_box_string's information. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Geoweb3d::GW3DEnvelope ParseBoundingBoxString( const std::string& bounding_box_string )
    {
        // Format for the bounding box strings are in the following format:
        //     "min_lat,max_lat,min_lon,max_lon"
        // Therefore, the extraction of data will be the following ordering:
        //     [0] == min_lat
        //     [1] == max_lat
        //     [2] == min_lon
        //     [3] == max_lon
        Geoweb3d::GW3DEnvelope bounding_box_envelope;
        std::stringstream string_stream( bounding_box_string );
        std::string temp_token;
        std::vector<std::string> bounding_box_coordinates;

        // Steps over each comma delimited entry of: "min_lat,max_lat,min_lon,max_lon"
        while( std::getline( string_stream, temp_token, ',' ) )
        {
            bounding_box_coordinates.push_back( temp_token );
        }

        try
        {
            bounding_box_envelope.MinY = std::stod( bounding_box_coordinates[ 0 ] );
            bounding_box_envelope.MaxY = std::stod( bounding_box_coordinates[ 1 ] );
            bounding_box_envelope.MinX = std::stod( bounding_box_coordinates[ 2 ] );
            bounding_box_envelope.MaxX = std::stod( bounding_box_coordinates[ 3 ] );
        }
        catch( std::exception& )
        {
            std::cerr << "There was an error trying to convert the envelope strings to their decimal representations." << std::endl;
        }
        
        return bounding_box_envelope;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Invoked when Stream is called. This streamer will create and populate a new
    ///           GeocodeStreamResultInformation object and insert it into this class' search_results_.   </summary>
    ///
    /// <param name="result"> The IGW3DVectorLayerStreamResult which contains the Geocoder's information. </param>
    /// 
    /// <returns> Whether or not the GeocodeStreamResultInformation was populated and stored properly.    </returns>
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult* result )
    {
        const Geoweb3d::IGW3DAttributeCollection*  attribute_collection  = result->get_AttributeCollection();
        const Geoweb3d::IGW3DDefinitionCollection* definition_collection = attribute_collection->get_DefinitionCollection();
        int bounding_box_index = definition_collection->get_IndexByName( "boundingbox" );
        int display_name_index = definition_collection->get_IndexByName( "display_name" );
        int city_index         = definition_collection->get_IndexByName( "city" );
        int state_index        = definition_collection->get_IndexByName( "state" );
        int postcode_index     = definition_collection->get_IndexByName( "postcode" );
        int country_index      = definition_collection->get_IndexByName( "country" );
        // Note: There are more than just these indices. If you require more information,
        //       print all available keys from the IGW3DDefinitionCollection.

        // This will store all of the information from the current result for future querying from a client.
        GeocodeStreamResultInformation result_information;

        Geoweb3d::IGW3DStringPtr bounding_box_string;
        attribute_collection->get_Property( bounding_box_index ).to_string( bounding_box_string );
        
        Geoweb3d::GW3DEnvelope envelope = ParseBoundingBoxString( bounding_box_string->c_str() );
        result_information.envelope = envelope;

        Geoweb3d::IGW3DStringPtr parse_string;
        attribute_collection->get_Property( display_name_index ).to_string( parse_string );
        result_information.display_name = parse_string->c_str();

        attribute_collection->get_Property( city_index ).to_string( parse_string );
        result_information.city = parse_string->c_str();

        attribute_collection->get_Property( state_index ).to_string( parse_string );
        result_information.state = parse_string->c_str();

        attribute_collection->get_Property( postcode_index ).to_string( parse_string );
        result_information.postcode = parse_string->c_str();

        attribute_collection->get_Property( country_index ).to_string( parse_string );
        result_information.country = parse_string->c_str();

        search_results_.push_back( result_information );
      
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> OnError IGW3DVectorLayerStream override. This can be used 
    ///           to handle any custom stop or error-related stream cancel.  </summary>
    /// 
    /// <returns> true.                                                      </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool OnError()
    {
        return true;
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Gets the count of features to be processed. </summary>
    ///
    /// <returns> The size of the feature_info_set_.          </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual unsigned long count() const
    {
        return 0;
    }
   
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Determines how to iterate to the next result, but only if count() is non-zero. </summary>
    /// 
    /// <param name="ppVal"> [out] If non-null and non-zero count(), the 
    ///                      next Feature / Object ID the OnStream result
    ///                      will contain.                                                       </param>
    ///
    /// <remarks> Unnecessary in this case due to us streaming all features.                     </remarks>
    /// 
    /// <returns> true if it succeeds, false if no more features exist in the collection.        </returns>
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool next( int64_t* ppVal )
    {
        return true;
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Resets the iterator, but unnecessary in this case since count() is zero. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    virtual void reset() { }

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

    virtual int64_t operator[]( unsigned long index )
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

    virtual int64_t get_AtIndex( unsigned long index )
    {
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Returns all results gathered via OnStream calls. </summary>
    ///
    /// <returns> An iterable container of all stream results.     </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector< GeocodeStreamResultInformation > get_SearchResults() const
    {
        return search_results_;
    }

protected:
    
    int result_index_ = 1;
    std::vector<GeocodeStreamResultInformation> search_results_;

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic application class encapsulating a Geoweb3d SDK engine context,
///           a viewable window, and a camera the window relies on. This example App
///           class will exemplify how to properly instantiate a Geocoder driver and
///           query for results and use them in a meaningful way with other types of
///           Representations.                                                          </summary>
/// 
/// <remarks> This application inherits from the IGW3DWindowCallback so that the user
///           can extend mouse or key events to interact with the camera naviagtor and
///           the Reprensetations in the scene. The "L" key is used to being a new
///           serach query, using the console window as user input.                     </remarks>
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
            : sdk_engine_context_ ( sdk_context )
            , navHelper_          ( new NavigationHelper() )
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

		void OnDrawEnd() override { }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback OnDraw2D() override.                     </summary>
        ///
        /// <remarks> This must be overridden, but is unnecessary in this example. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr& camera ) override { }
		
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback ProcessEvent() override. </summary>
        ///
        /// <param name="win_event"> A Geoweb3d window event.
        ///                          See Geoweb3d::WindowEvent::EventType 
        ///                          for a list of window events that can
        ///                          be captured and listened to.          </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void ProcessEvent( const Geoweb3d::WindowEvent& win_event ) override
		{
            switch( win_event.Type )
            {
                case win_event.KeyPressed:
                {
                    switch( win_event.Key.code )
                    {
                        // Note: After pressing the "L" key, focus must be explicitly set to the console window.
                        case Geoweb3d::Key::Code::L:
                            QueryLocation();
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }

            navHelper_->ProcessEvent( win_event, window_ );
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Pumps the window draw and PostDraw tasks.                  </summary>
        ///
        /// <remarks> This will continuosuly pump so long as the draw was valid. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool Run()
        {
            return Geoweb3d::Succeeded( sdk_engine_context_->draw( window_ ) );
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initializes the App instances' window, camera, camera navigator, and imagery layer. </summary>
        ///         
        /// <returns> true if all initializations in the summary above completed properly.                </returns>
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        bool Initialize()
        {
            window_ = sdk_engine_context_->get_WindowCollection()->create_3DWindow( "Geocoding Example", GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), this );
            
            geocode_driver_         = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "GEOCODE" );
            draped_line_rep_driver_ = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "DrapedLine" );
            
            if ( geocode_driver_.expired() || draped_line_rep_driver_.expired() )
            {
                std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilites example" << std::endl;
                return false;
            }

            // Loads an imagery layer via the imagery driver. We'll use a google maps web-based layer.
            // So, an internet connection is necessary for this example to work properly.
            CreateImageryLayer();

            // Creates and sets up some defaults for the camera.
            camera_ = window_.lock()->get_CameraCollection()->create( "Main Camera" );
            navHelper_->add_Camera( camera_ );
            camera_.lock()->get_CameraController()->put_Location( 0.0, 0.0 );
            navHelper_->put_HomePosition( camera_ );

            // Note: This first query will stall the application with an initial query example.
            //       Like mentioned in comments above, you must give focus to the console window
            //       and input your selection before the program will continue.
            QueryLocation( "Washington DC" );

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an imagery layer from a raster file. This is used so the
        ///           Geocoder's envelope and camera settings have some contextual information. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateImageryLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper RasterDriver for the particular data type.
            //         Auto open will attempt to use the best-match raster driver for the data type.
            Geoweb3d::GW3DResult result;
            imagery_data_source_ = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "http://app.geoweb3d.com/dashboard/data/frmt_wms_virtualearth_hybrid.xml", result );
            if ( Geoweb3d::Succeeded( result ) && !imagery_data_source_.expired() )
            {
                // Step 2. Get the layer collection from the raster data source.
                Geoweb3d::IGW3DRasterLayerCollection* layer_collection = imagery_data_source_.lock()->get_RasterLayerCollection();
                if ( layer_collection && layer_collection->count() > 0 )
                {
                    // Step 3. Retrieve the layer needed for representation via the Imagery raster layer driver.
                    //         In this case, the WMS file used only has a single layer, so we'll retrieve just that layer.
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
                            imagery_rep_driver_.lock()->get_RepresentationLayerCollection()->create( imagery_layer_, params);
                        }
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates a Vector Layer with a single feature that can be represented. </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.               </param>
        /// 
        /// <param name="envelope"> Envelope to grab points from to visualize.              </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void CreateDrapedLineLayer( const char* layer_name, Geoweb3d::GW3DEnvelope envelope )
		{
            // Clears any previous Datasources and or Vector Layer being used from previous search 
            // results, so only one Vector Layer and Datasource can be active at a time.
            if( !draped_line_datasource_.expired() )
            {
                close_VectorDataSource( draped_line_datasource_ );
                draped_line_datasource_.reset();
                draped_line_layer_.reset();
            }

            // We need a vector driver for datasources so we can create a datasource that will be used to create a layer.
            // For a list of valid vector drivers, loop over the VectorDriverCollection and print out each respective driver's name.
            Geoweb3d::IGW3DVectorDriverWPtr vector_driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );
            
            // Using the Vector Driver we can then create a new VectorDataSource with whatever unique name we want. 
            // This name can be used to retrieve it in the future.
            draped_line_datasource_ = vector_driver.lock()->get_VectorDataSourceCollection()->create( "DrapedLinesDataset" );
            
            // We're not using the field_definition for anything in particular for this example, but can be used for attribute mapping.
            // Without an initialized Geoweb3d::IGW3DDefinitionCollectionPtr, the VectorLayerCollection create call will fail for your layer,
            // so it must be created / allocated. We also need it so we can create an empty / default property collection for the feature of the layer.
            Geoweb3d::IGW3DDefinitionCollectionPtr field_definition( Geoweb3d::IGW3DDefinitionCollection::create() );

            // For Draped Lines, you should use Geoweb3d::gtLINE_STRING_25D as the GeometryType.
            // This will create the layer to be used for the feature creation.
            draped_line_layer_ = draped_line_datasource_.lock()->get_VectorLayerCollection()->create( layer_name, Geoweb3d::gtLINE_STRING_25D, field_definition );

            // Add some coordinates to the line to draw to and from. This will outline the envelope passed in.
            if( !draped_line_layer_.expired() )
            {
                Geoweb3d::GW3DResult     result;
                Geoweb3d::GW3DPoint      bottom_left_point ( envelope.MinX, envelope.MinY, 0.0 );
                Geoweb3d::GW3DPoint      top_left_point    ( envelope.MinX, envelope.MaxY, 0.0 );
                Geoweb3d::GW3DPoint      top_right_point   ( envelope.MaxX, envelope.MaxY, 0.0 );
                Geoweb3d::GW3DPoint      bottom_right_point( envelope.MaxX, envelope.MinY, 0.0 );
                Geoweb3d::GW3DLineString draped_line_coordinates;
                draped_line_coordinates.put_CoordinateDimension( 3 );
                draped_line_coordinates.put_NumPoints( 6 );
                draped_line_coordinates.put_Point( 0, &bottom_left_point );
                draped_line_coordinates.put_Point( 1, &top_left_point );
                draped_line_coordinates.put_Point( 2, &top_right_point );
                draped_line_coordinates.put_Point( 3, &bottom_right_point );
                draped_line_coordinates.put_Point( 4, &bottom_left_point );
                draped_line_coordinates.put_Point( 5, &top_left_point );

                // This will create the feature for the layer, so that when represented, it'll have default properties and some geometry to display based off of.
                Geoweb3d::IGW3DPropertyCollectionPtr default_property_collection = field_definition->create_PropertyCollection();
                draped_line_layer_.lock()->create_Feature( default_property_collection, &draped_line_coordinates, result );
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Takes a Vector Layer and creates a Representation from it.
        ///           This will also exemplify how to properly retrieve the default
        ///           property collection from a respective Representation Driver
        ///           that's used during the creation process.                          </summary>
        /// 
        /// <returns> true if the Representation was properly created, otherwise false. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool CreateDrapedLineRepresentation()
        {
            // Just like in CreateDrapedLineLayer above, we'll clear any previously created Representations.
            if( !draped_line_rep_.expired() )
            {
                close_VectorRepresentation( draped_line_rep_ );
            }

            if( draped_line_rep_driver_.expired() )
            {
                std::cout << "Error creating DrapedLine vector representation driver." << std::endl;
                return false;
            }

            // Boiler plate for ensuring that the representation driver for the given layer is valid. 
            // It's not necessary, but good practice to prevent crashes due to mismatched types.
            Geoweb3d::GW3DResult can_represent;
            can_represent = draped_line_rep_driver_.lock()->get_CapabilityToRepresent( draped_line_layer_ );
            if( !Geoweb3d::Succeeded( can_represent ) )
            {
                std::cout << "Error: The selected DrapedLine representation driver cannot represent this layer." << std::endl;
                return false;
            }
            
            Geoweb3d::IGW3DPropertyCollectionPtr default_draped_line_properties = draped_line_rep_driver_.lock()->get_PropertyCollection()->create_Clone();
            const Geoweb3d::IGW3DDefinitionCollection* definition_collection = default_draped_line_properties->get_DefinitionCollection();
            // Note: If you don't which properties are available for the particular driver, you can loop over the IGW3DDefinitionCollection and check the `property_name`.
            //       Also the properties are case-sensitive.
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "RED" ),   1.0 );
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "GREEN" ), 1.0 );
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "BLUE" ),  0.0 );
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "ALPHA" ), 1.0 );
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "LINE_WIDTH" ), 5 );
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "OUTLINE_MODE" ), Geoweb3d::Vector::OutlineMode::OUTLINE_ON );
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET_MODE" ), Geoweb3d::Vector::ABSOLUTE_MODE );
            default_draped_line_properties->put_Property(definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET" ), 0.001 );

            Geoweb3d::Vector::RepresentationLayerCreationParameter draped_line_rep_layer_creation_params;
            // Setting to a page_level of 1 will allow the representation to be visualized at nearly any altitude.
            draped_line_rep_layer_creation_params.page_level = 1;
            draped_line_rep_layer_creation_params.representation_default_parameters = default_draped_line_properties;

            // This will tell the driver to officially represent the layer with the previous set layer paramaters and should now be visible in the scene.
            draped_line_rep_ = draped_line_rep_driver_.lock()->get_RepresentationLayerCollection()->create( draped_line_layer_, draped_line_rep_layer_creation_params );

            if( draped_line_rep_.expired() )
            {
                std::cout << "Error. The representation was not properly created." << std::endl;
                return false;
            }

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Invoked when the user presses the "L" key within focus of the 3D Window.
        ///           This function will prompt the user to give input from std::cin, so it's
        ///           necessary for the user to give focus to the console window before typing
        ///           in their selection. After giving a search string, the user will have the
        ///           ability to see up to 10 search results from their location input. They will
        ///           then have the ability to select which of the results they'd like to select
        ///           for setting a new Draped Line Representation which outlines the envelope of
        ///           their selection and will update the camera to the new position.               </summary>
        /// 
        /// <param name="location"> The location string to be fed to the GeoCoder. If it's empty, 
        ///                         the user will be prompted via std::cin for their input instead. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void QueryLocation( const std::string& location = "" )
        {
            // ability to quit the search early.
            if( location == "q" ) return;

            std::string location_to_search = location;
            // User-supplied input via std::cin.
            if( location.empty() )
            {
                std::cout << "Enter a new location to go to:" << std::endl;
                std::getline( std::cin, location_to_search );
            }
            
            // Let's attempt to do a Geocode search to determine the camera's placement and the Draped Line Representation's points.
            Geoweb3d::GW3DResult result;
            Geoweb3d::IGW3DVectorDataSourceWPtr geocode_search_datasource = geocode_driver_.lock()->get_VectorDataSourceCollection()->open( location_to_search.c_str(), result );

            Geoweb3d::IGW3DVectorLayerCollection* layer_collection = nullptr;
            if( !geocode_search_datasource.expired() )
            {
                layer_collection = geocode_search_datasource.lock()->get_VectorLayerCollection();
            }

            Geoweb3d::IGW3DVectorLayerWPtr layer;
            GeocodeStream geocode_extraction_stream;
            // Stream all of the results since there can be multiple hits for a generic search.
            for ( unsigned  i = 0; layer_collection && i < layer_collection->count(); ++i )
            {
                layer = layer_collection->get_AtIndex( i );
                layer.lock()->Stream( &geocode_extraction_stream );
            }

            geocode_driver_.lock()->get_VectorDataSourceCollection()->close( geocode_search_datasource );

            // Now we should have collected all of the results from the Streamer and can iterate over them.
            std::vector< GeocodeStreamResultInformation > search_results = geocode_extraction_stream.get_SearchResults();
            if( !search_results.empty() )
            {
                std::cout << "Please select which result you'd like to go to:" << std::endl;
                // If there's only one valid search result, there's no point in asking for input.
                bool received_valid_response = search_results.size() == 1;
                for( int i = 1; i <= search_results.size(); ++i )
                {
                    if( i >= 10 )
                    {
                        std::cout << "\t " << "[" << i << "] " << search_results.at(i - 1);
                    }
                    else
                    {
                        // Extra padding so the entries align neatly.
                        std::cout << "\t " << "[" << i << "]  " << search_results.at(i - 1);
                    }
                }

                int selection = 0;
                bool user_quit = false;
                // Safe std::cin method for determing user's selection for which index in the search results we will act upon.
                while( !received_valid_response && !user_quit )
                {
                    std::string response;
                    getline( std::cin, response );
                    if( response == "q" )
                    {
                        user_quit = true;
                        break;
                    }

                    selection = 0;

                    try
                    {
                        selection = std::stoi( response );
                    }
                    catch( std::exception& ) { }

                    if( selection >= 1 && selection < search_results.size() + 1 )
                    {
                        // selection from the user is 1-index based, but we're accessing a vector, so this will make it 0-index based.
                        --selection;
                        received_valid_response = true;
                    }
                    else
                    {
                        std::cout << "Error, selection was out of range, please eneter a valid entry from 1 to " << search_results.size() << std::endl;
                    }
                }

                // At this point we have the ability to properly update the camera position and redraw the feature's points for a Draped Line Representation.
                if( received_valid_response )
                {
                    Geoweb3d::GW3DResult ensure_visible_valid;
                    GeocodeStreamResultInformation picked_result = search_results.at( selection );
                    // ensure_Visible will take a GW3DEnvelope and give the user a point such that it's guaranteed that the envelope will be completely within view of a camera.
                    Geoweb3d::GW3DPoint geocoding_location = camera_.lock()->get_CameraController()->ensure_Visible( picked_result.envelope, ensure_visible_valid );
                    if( Geoweb3d::Succeeded( ensure_visible_valid ) )
                    {
                        camera_.lock()->get_CameraController()->put_Location( geocoding_location.get_X(), geocoding_location.get_Y() );
                        camera_.lock()->get_CameraController()->put_Elevation( geocoding_location.get_Z() );
                        camera_.lock()->get_CameraController()->put_Rotation( 0.0, 90.0, 0.0 );

                        CreateDrapedLineLayer( picked_result.display_name.c_str(), picked_result.envelope );
                        CreateDrapedLineRepresentation();
                        navHelper_->put_HomePosition( camera_ );
                    }
                }
            }
            else
            {
                std::cout << "No results were found for: " << location_to_search << std::endl;
                std::cout << "Please refine your search to contain less information to get a wider query." << std::endl;
            }

        }

    private:
        Geoweb3d::IGW3DGeoweb3dSDKPtr   sdk_engine_context_;
        Geoweb3d::IGW3DVectorDriverWPtr geocode_driver_;
        Geoweb3d::IGW3DWindowWPtr       window_;
        Geoweb3d::IGW3DCameraWPtr       camera_;
        NavigationHelper*               navHelper_;
        
        Geoweb3d::IGW3DRasterRepresentationDriverWPtr imagery_rep_driver_;
        Geoweb3d::IGW3DRasterDataSourceWPtr		      imagery_data_source_;
        Geoweb3d::IGW3DRasterLayerWPtr			      imagery_layer_;

        Geoweb3d::IGW3DVectorRepresentationDriverWPtr draped_line_rep_driver_;
        Geoweb3d::IGW3DVectorDataSourceWPtr           draped_line_datasource_;
        Geoweb3d::IGW3DVectorLayerWPtr			      draped_line_layer_;
        Geoweb3d::IGW3DVectorRepresentationWPtr		  draped_line_rep_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Runs the Geocoding Application instance.
///           This will continuosly run until the window is closed. </summary>
/// 
/// <param name="sdk_context"> The Geoweb3d engine context.         </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{    
    App app( sdk_context );
    app.Initialize();
    bool valid = true;
    while( valid )
    {
        valid = app.Run();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Initializes the Geowebd3d engine SDK context. 
///           Initializes SDK configuration.                
///           Runs the Geocoding example application.               </summary>
/// 
/// <param name="argc"> Number of command line arguments.           </param>
/// 
/// <param name="argv"> The contents of the command line arguments. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype( &_pclose )> pipe(_popen(cmd, "r"), _pclose);
    if( !pipe )
    {
        throw std::runtime_error("popen() failed!");
    }
    while( fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr )
    {
        result += buffer.data();
    }
    return result;
}

int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling();
    std::cout << exec( "curl --version" ) << std::endl;
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
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information, my_info_function  );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,     my_info_function  );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,       my_fatal_function );
}
