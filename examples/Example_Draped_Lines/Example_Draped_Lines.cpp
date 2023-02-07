#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DRaster.h"

#include "Geoweb3dCore/LayerParameters.h"
#include "GeoWeb3dCore/SystemExports.h"

#include <set>
#include <map>
#include <deque>
#include <iostream>
#include <tchar.h>
#include <ctime>
#include <chrono>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )


void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic application class encapsulating a Geoweb3d SDK engine context,
///           a viewable window, and a camera the window relies on. This example App
///           class will exemplify how to properly instantiate an imagery / elevation
///           layer and use information from the loaded layer(s) to draw a Draped Line
///           from a layer's top-left coordinate to its bottom-right coordinate.
///           While the App runs, every 15 seconds the Draped Line's properties will
///           change and print the current property state to std::cout.                </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class App
{
	public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an App instance.                            </summary>
        ///
        /// <param name="sdk_context"> A Geoweb3d SDK engine context.     </param>
        ///
        /// <param name="last_randomized_time_"> Tracks the last clock
        ///                                      time from when the
        ///                                      representation properties
        ///                                      were last modified.       </param>
        /// 
        /// <returns> An instance to an App.                               </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		App( const Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context )
			: sdk_engine_context_  ( sdk_context )
            , last_randomized_time_( std::chrono::steady_clock::now() )
		{
            // Needed for the property randomization
            srand ( static_cast < unsigned > ( std::time( 0 ) ) );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initializes the App instances' window, camera, raster layer,
        ///           Draped Line layer, feature, and representation.              </summary>
        ///
        /// <remarks> This function will continuously pump the created window via
        ///           the Geoweb3d SDK engine context until the window is closed.   </remarks>
        ///         
        /// <returns> true when the window is finally closed or false if an error
        ///           occurs during initialization.                                 </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool Initialize()
        {
            window_ = sdk_engine_context_->get_WindowCollection()->create_3DWindow( "Draped Lines Example", GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), nullptr );
            Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );            

            if ( driver.expired() )
            {
                std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilites example" << std::endl;
                return false;
            }

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

            // Attempt to draw a diagonal line from the top left of the imagery raster to the bottom right.
            double left_most_lon   = raster_envelope.MinX;
            double right_most_lon  = raster_envelope.MaxX;
            double bottom_most_lat = raster_envelope.MaxY;
            double top_most_lat    = raster_envelope.MinY;
            double line_elev       = 0.0; // no elevation to show how it implicitly drapes on any elevation data.
            double camera_elev     = 3500.0;
            double center_lon      = ( right_most_lon + left_most_lon ) / 2.0;
            double center_lat      = ( top_most_lat + bottom_most_lat ) / 2.0;

            // Sets the camera on the window to pitch 45 degrees from the bottom of the raster source.
            UpdateCamera( center_lon, bottom_most_lat - ( ( top_most_lat - bottom_most_lat ) / 2 ), camera_elev );

            // This function will exemplify how to properly add a Draped Line by:
            //    1. Querying SDK engine context for the proper VectorRepresentationDriver.
            //    2. Creating a VectorLayer off of a VectorDataSource with its corresponding VectorRepresentationDriver's default properties.
            //    3. Creating a Feature off of a properly setup VectorLayer with default properties.
            //    4. Modifying the VectorLayer's properties.
            //    5. Representing the VectorLayer by its corresponding VectorRepresentationDriver.
            AddDrapedLine( "DrapedLine", left_most_lon, top_most_lat, line_elev, right_most_lon, bottom_most_lat, line_elev );

            // Continuously pump the draw thread until the user closes the window.
            while ( SUCCEEDED( sdk_engine_context_->draw( window_ ) ) )
            {
                // Every 15 seconds this will modify the color / line width properties of the
                // Draped Line Representation and print to std::cout its current property states.
                RandomizeDrapedLineRepresentationProperties();
            }

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
            Geoweb3d::GW3DResult result;
            imagery_data_source_ = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//DC_1ft_demo.ecw", result );
            if ( Geoweb3d::Succeeded( result ) && !imagery_data_source_.expired() )
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
            Geoweb3d::GW3DResult result;
            elevation_data_source_ = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//NED 10-meter DC.tif", result);
            if ( Geoweb3d::Succeeded( result ) && !elevation_data_source_.expired() )
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
                            elevation_rep_driver_.lock()->get_RepresentationLayerCollection()->create( elevation_layer_, params);
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
            if ( camera_.expired() )
            {
                camera_ = window_.lock()->get_CameraCollection()->create( "Main Camera" );
            }

            camera_.lock()->get_CameraController()->put_Location ( longitude, latitude );
            camera_.lock()->get_CameraController()->put_Elevation( elevation, Geoweb3d::IGW3DPositionOrientation::Relative );

            // This will pitch the camera towards the ground at an angle to show draping.
            float pitch = 45.0f;
            camera_.lock()->get_CameraController()->put_Rotation ( 0.0f, pitch, 0.0f );
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Forwards information to CreateDrapedLineLayer and to 
        ///           CreateDrapedLineRepresentation to properly create a Draped
        ///           Line layer and Representation to visualize it in the map.                  </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                    </param>
        /// 
        /// <param name="longitude_start"> Longitude of the first point of the line.             </param>
        /// 
        /// <param name="latitude_start"> Latitude of the first point of the line.               </param>
        /// 
        /// <param name="elevation_start"> Elevation (in meters) of the first point of the line. </param>
        /// 
        /// <param name="longitude_end"> Longitude of the end point of the line.                 </param>
        /// 
        /// <param name="latitude_end"> Latitude of the end point of the line.                   </param>
        /// 
        /// <param name="elevation_end"> Elevation (in meters) of the end point of the line.     </param>
        /// 
        /// <returns> true when successful and false if an error occurred or something failed.   </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool AddDrapedLine( const char* layer_name,
                            double longitude_start, double latitude_start, double elevation_start,
                            double longitude_end,   double latitude_end,   double elevation_end )
		{            
            
            // For each type of representation, there's a specific driver name.
            // If you're unsure of which type to use or which are available, you can get the 
            // VectorRepresentationDriverCollection and loop over each index and call get_Name on them.
            // In this example we're going to be using the "DrapedLine" driver explicitly.
            draped_line_rep_driver_ = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "DrapedLine" );
            
            if ( draped_line_rep_driver_.expired() )
            {
                std::cout << "Error getting DrapedLine Vector Representation Driver for layer: " << layer_name << std::endl;
                return false;
            }

			draped_line_layer_ = CreateDrapedLineLayer( layer_name, longitude_start, latitude_start, elevation_start, longitude_end, latitude_end, elevation_end );

			if( draped_line_layer_.expired() )
			{
				std::cout << "Error creating Draped Line Layer for: " << layer_name << std::endl;
				return false;
			}			
			
			if( !CreateDrapedLineRepresentation( draped_line_layer_ ) )
			{
				std::cout << "Error creating " << layer_name << "'s representation." << std::endl;
				return false;
			}

			return true;
		}
		
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Takes the information forwarded from AddDrapedLine and creates
        ///           a Vector Layer with a single feature that can be represented.              </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                    </param>
        /// 
        /// <param name="longitude_start"> Longitude of the first point of the line.             </param>
        /// 
        /// <param name="latitude_start"> Latitude of the first point of the line.               </param>
        /// 
        /// <param name="elevation_start"> Elevation (in meters) of the first point of the line. </param>
        /// 
        /// <param name="longitude_end"> Longitude of the end point of the line.                 </param>
        /// 
        /// <param name="latitude_end"> Latitude of the end point of the line.                   </param>
        /// 
        /// <param name="elevation_end"> Elevation (in meters) of the end point of the line.     </param>
        /// 
        /// <returns> A valid Vector Layer if successful that can later be represented.          </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		Geoweb3d::IGW3DVectorLayerWPtr CreateDrapedLineLayer(const char* layer_name, 
                                                             double longitude_start, double latitude_start, double elevation_start,
                                                             double longitude_end,   double latitude_end,   double elevation_end )
		{
            // We need a vector driver for datasources so we can create a datasource that will be used to create a layer.
            // For a list of valid vector drivers, loop over the VectorDriverCollection and print out each respective driver's name.
            Geoweb3d::IGW3DVectorDriverWPtr vector_driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );
            
            // Using the Vector Driver we can then create a new VectorDataSource with whatever unique name we want. 
            // This name can be used to retrieve it in the future.
            Geoweb3d::IGW3DVectorDataSourceWPtr dataset = vector_driver.lock()->get_VectorDataSourceCollection()->create( "DrapedLinesDataset" );
            
            // We're not using the field_definition for anything in particular for this example, but can be used for attribute mapping.
            // Without an initialized Geoweb3d::IGW3DDefinitionCollectionPtr, the VectorLayerCollection create call will fail for your layer, so it must be created / allocated.
            // We also need it so we can create an empty / default property collection for the feature of the layer.
            Geoweb3d::IGW3DDefinitionCollectionPtr field_definition( Geoweb3d::IGW3DDefinitionCollection::create() );

            // For Draped Lines, you should use Geoweb3d::gtLINE_STRING_25D. 
            // This will create the layer to be used for the feature creation.
            Geoweb3d::IGW3DVectorLayerWPtr layer = dataset.lock()->get_VectorLayerCollection()->create( layer_name, Geoweb3d::gtLINE_STRING_25D, field_definition );

            // Add some coordinates to the line to draw to and from
			if( !layer.expired() )
			{
                Geoweb3d::GW3DResult     result;
                Geoweb3d::GW3DPoint      start_point( longitude_start, latitude_start, elevation_start );
                Geoweb3d::GW3DPoint      end_point  ( longitude_end,   latitude_end,   elevation_end );
                Geoweb3d::GW3DLineString draped_line_coordinates;
                draped_line_coordinates.put_CoordinateDimension( 3 );
                draped_line_coordinates.put_NumPoints( 2 );
                draped_line_coordinates.put_Point( 0, &start_point );
                draped_line_coordinates.put_Point( 1, &end_point );

                // This will create the feature for the layer so that when represented it'll have default properties and some geometry to display based off of.
                // After this, however, we will update the default property collection periodically every 15 seconds to show how that's done too in RandomizeDrapedLineRepresentationProperties.
                Geoweb3d::IGW3DPropertyCollectionPtr default_property_collection = field_definition->create_PropertyCollection();
                layer.lock()->create_Feature( default_property_collection, &draped_line_coordinates, result );
			}

			return layer;
		}	

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Takes a Vector Layer and creates a Representation from it.
        ///           This will also exemplify how to properly retrieve the default
        ///           property collection from a respective Representation Driver
        ///           that's used during the creation process.                          </summary>
        /// 
        /// <param name="layer"> The Vector Layer that's been properly initialized.
        ///                      This will be used directly as a part of the
        ///                      Representation Driver's creation process to produce
        ///                      the Representation.                                    </param>
        /// 
        /// <returns> true if the Representation was properly created, otherwise false. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CreateDrapedLineRepresentation( Geoweb3d::IGW3DVectorLayerWPtr layer )
		{
			if( draped_line_rep_driver_.expired() )
			{
				std::cout<< "Error creating DrapedLine vector representation driver." << std::endl;
				return false;
			}

            // Boiler plate for ensuring that the representation driver for the given layer is valid. 
            // It's not necessary, but good practice to prevent crashes due to mismatched types.
			Geoweb3d::GW3DResult can_represent;
			can_represent = draped_line_rep_driver_.lock()->get_CapabilityToRepresent( layer );
			if( !Geoweb3d::Succeeded( can_represent ) )
			{
				std::cout << "Error: The selected DrapedLine representation driver cannot represent this layer." << std::endl;
				return false;
			}            

			// Method 1: Modifying representation properties via enums.
			// Geoweb3d::IGW3DPropertyCollectionPtr default_draped_line_properties = draped_line_rep_driver_.lock()->get_PropertyCollection()->create_Clone();
            // default_draped_line_properties->put_Property( Geoweb3d::Vector::DrapedLinesProperties::RED,   0.0 );
            // default_draped_line_properties->put_Property( Geoweb3d::Vector::DrapedLinesProperties::GREEN, 1.0 );
            // default_draped_line_properties->put_Property( Geoweb3d::Vector::DrapedLinesProperties::BLUE,  0.0 );
            // default_draped_line_properties->put_Property( Geoweb3d::Vector::DrapedLinesProperties::ALPHA, 1.0 );
            // default_draped_line_properties->put_Property( Geoweb3d::Vector::DrapedLinesProperties::TRANSLATION_Z_OFFSET_MODE, Geoweb3d::Vector::ABSOLUTE_MODE );
            // default_draped_line_properties->put_Property( Geoweb3d::Vector::DrapedLinesProperties::TRANSLATION_Z_OFFSET, 0.001 );
                        	        
            // Method 2: Modifying representation properties via get_IndexByName.
            Geoweb3d::IGW3DPropertyCollectionPtr default_draped_line_properties = draped_line_rep_driver_.lock()->get_PropertyCollection()->create_Clone();
            const Geoweb3d::IGW3DDefinitionCollection* definition_collection    = default_draped_line_properties->get_DefinitionCollection();
            // Note: If you don't which properties are available for the particular driver, you can loop over the IGW3DDefinitionCollection and check the `property_name`.
            //       Also the properties are case-sensitive.
            default_draped_line_properties->put_Property( definition_collection->get_IndexByName( "RED"   ), 0.0 );
            default_draped_line_properties->put_Property( definition_collection->get_IndexByName( "GREEN" ), 1.0 );
            default_draped_line_properties->put_Property( definition_collection->get_IndexByName( "BLUE"  ), 0.0 );
            default_draped_line_properties->put_Property( definition_collection->get_IndexByName( "ALPHA" ), 1.0 );
            default_draped_line_properties->put_Property( definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET_MODE" ), Geoweb3d::Vector::ABSOLUTE_MODE );
            default_draped_line_properties->put_Property( definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET" ),      0.001 );
			
            Geoweb3d::Vector::RepresentationLayerCreationParameter draped_line_rep_layer_creation_params;
            // Setting to a page_level of 1 will allow the representation to be visualized at nearly any altitude.
            draped_line_rep_layer_creation_params.page_level = 1;
            draped_line_rep_layer_creation_params.representation_default_parameters = default_draped_line_properties;

            // This will tell the driver to officially represent the layer with the previous set layer paramaters and should now be visible in the scene.
			draped_line_rep_ = draped_line_rep_driver_.lock()->get_RepresentationLayerCollection()->create( layer, draped_line_rep_layer_creation_params );           			

            if ( draped_line_rep_.expired() )
            {
                std::cout << "Error. The representation was not properly created." << std::endl;
                return false;
            }

			return true;
		}
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Occasionally modifies the Draped Line Representation's properties that
        ///           was created in CreateDrapedLineRepresentation. Every 15 seconds it will 
        ///           randomize the RGB color and the screen-space width of the representation
        ///           and print the current state of all properties to std::cout.               </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void RandomizeDrapedLineRepresentationProperties()
        {
            std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
            // Check the system time since the last call and every certain amount of seconds later, 
            // just modify the properties and print the results.
            long long elapsed_seconds = std::chrono::duration_cast< std::chrono::seconds >( current_time - last_randomized_time_ ).count();
            if ( elapsed_seconds > 15 )
            {
                // Modify the draped_line_rep's property collection.
                Geoweb3d::IGW3DPropertyCollectionPtr       new_draped_line_property_collection = draped_line_rep_.lock()->get_PropertyCollection()->create_Clone();
                const Geoweb3d::IGW3DDefinitionCollection* definition_collection               = new_draped_line_property_collection->get_DefinitionCollection();
                new_draped_line_property_collection->put_Property( definition_collection->get_IndexByName( "RED" ),   static_cast< float >( rand() ) / static_cast< float >( RAND_MAX ) );
                new_draped_line_property_collection->put_Property( definition_collection->get_IndexByName( "GREEN" ), static_cast< float >( rand() ) / static_cast< float >( RAND_MAX ) );
                new_draped_line_property_collection->put_Property( definition_collection->get_IndexByName( "BLUE" ),  static_cast< float >( rand() ) / static_cast< float >( RAND_MAX ) );
                new_draped_line_property_collection->put_Property( definition_collection->get_IndexByName( "LINE_WIDTH" ), rand() % 10 + 1 );
                draped_line_rep_.lock()->put_PropertyCollection( new_draped_line_property_collection );

                // Print out property collection information about the Draped Line Representation.
                auto locked_draped_line_rep = draped_line_rep_.lock();
                if ( locked_draped_line_rep )
                {
                    const Geoweb3d::IGW3DPropertyCollection*   draped_line_property_collection = locked_draped_line_rep->get_PropertyCollection();
                    const Geoweb3d::IGW3DDefinitionCollection* definition_collection           = draped_line_property_collection->get_DefinitionCollection();
                    for ( unsigned int definition_index = 0; definition_index < definition_collection->count(); ++definition_index )
                    {
                        const Geoweb3d::GW3DPropertyDescription* current_property = definition_collection->get_AtIndex( definition_index );
                        Geoweb3d::IGW3DStringPtr property_value_str = Geoweb3d::IGW3DString::create( "" );
                        draped_line_property_collection->get_AtIndex( definition_index )->to_string( property_value_str );
                        std::cout << "Property #" << definition_index << "( " << current_property->property_name << " ): " << property_value_str->c_str() << std::endl;
                    }
                    std::cout << std::endl;
                }

                last_randomized_time_ = current_time;
            }
        }

	private:

		Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_engine_context_;
        Geoweb3d::IGW3DWindowWPtr     window_;
        Geoweb3d::IGW3DCameraWPtr     camera_;
        
        Geoweb3d::IGW3DRasterRepresentationDriverWPtr imagery_rep_driver_;        
        Geoweb3d::IGW3DRasterDataSourceWPtr           imagery_data_source_;
        Geoweb3d::IGW3DRasterLayerWPtr                imagery_layer_;

        Geoweb3d::IGW3DRasterRepresentationDriverWPtr elevation_rep_driver_;
        Geoweb3d::IGW3DRasterDataSourceWPtr           elevation_data_source_;
        Geoweb3d::IGW3DRasterLayerWPtr                elevation_layer_;

        Geoweb3d::IGW3DVectorRepresentationDriverWPtr draped_line_rep_driver_;
		Geoweb3d::IGW3DVectorLayerWPtr			      draped_line_layer_;
		Geoweb3d::IGW3DVectorRepresentationWPtr	      draped_line_rep_;
		        
        std::chrono::steady_clock::time_point last_randomized_time_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Runs the Draped Line Application instance.
///           This will continuosly run until the window is closed. </summary>
/// 
/// <param name="sdk_context"> The Geoweb3d engine context.         </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{    
    App app( sdk_context );
    app.Initialize();
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
	std::cout<< "Fatal Info: " << msg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> General info function, this will be used to
///           print any general info and non-fatal warnings.                           </summary>
/// 
/// <param name="msg"> Geoweb3d::Information / Geoweb3d::Warning categorized messages. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_info_function( const char* msg )
{
	std::cout<< "General Info: " << msg;
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
