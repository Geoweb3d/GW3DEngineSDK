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
/// <summary> A basic application class encapsulating a Geoweb3d SDK engine context,
///           a viewable window, and a camera the window relies on. This example App
///           class will exemplify how to properly instantiate an imagery / elevation
///           layer and use information from the loaded layer(s) to load Textured
///           Draped Lines.                                                             </summary>
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

		void OnDrawEnd() override { }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback OnDraw2D() override.                     </summary>
        ///
        /// <remarks> This must be overridden, but is unnecessary in this example. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr& camera ) override { }
		
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> IGW3DWindowCallback ProcessEvent() override.             </summary>
        ///
        /// <param name="win_event"> A Geoweb3d window event.
        ///                          See Geoweb3d::WindowEvent::EventType 
        ///                          for a list of window events that can
        ///                          be captured and listened to.              </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		void ProcessEvent( const Geoweb3d::WindowEvent& win_event ) override
		{			
            navHelper_->ProcessEvent( win_event, window_ );
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
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initializes the App instances' window, camera, camera navigator,
        ///           raster layer, elevation layer, Textured Draped Line layers, 
        ///           features, and representations.                                       </summary>
        ///         
        /// <returns> true if all initializations in the summary above completed properly. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Initialize()
		{
			window_ = sdk_engine_context_->get_WindowCollection()->create_3DWindow( "Textured Draped Lines", GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), this ); 
			
            Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );
            
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

            data_source_ = driver.lock()->get_VectorDataSourceCollection()->create( "TestDataSource" );
			if( data_source_.expired() )
			{
				std::cout << "Could not create data source." << std::endl;
				return false;
			}

            // Loads raster imagery layers, elevation layer, and an ExtrudedPolygon Representation via the
            // imagery / elevation / VectorRepresentation driver(s) that we can use to gather some latitude
            // and longitude positions from and display the textured draped lines.
            CreateDCImageryLayer();
            CreateGlobalImageryLayer();
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

            // This example app's intended purpose is to show how to create Textured Draped Lines.
			AddTexturedDrapedLines( "ShortTexturedDrapedLine", "LongTexturedDrapedLines", longitude, latitude );

			return true;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an imagery layer from a raster file. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateDCImageryLayer()
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
        /// <summary> Creates an imagery layer from a raster file. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void CreateGlobalImageryLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper RasterDriver for the particular data type.
            //         Auto open will attempt to use the best-match raster driver for the data type.
            Geoweb3d::GW3DResult result;
            auto imagery_data_source = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//WSI-Earth99-2k.ecw", result );
            if ( Geoweb3d::Succeeded( result ) && !imagery_data_source.expired() )
            {
                // Step 2. Get the layer collection from the raster data source.
                Geoweb3d::IGW3DRasterLayerCollection* layer_collection = imagery_data_source.lock()->get_RasterLayerCollection();
                if ( layer_collection && layer_collection->count() > 0 )
                {
                    // Step 3. Retrieve the layer needed for representation via the Imagery raster layer driver.
                    //         In this case, the *.ecw file used only has a single layer, so we'll retrieve just that layer.
                    auto imagery_layer = layer_collection->get_AtIndex( 0 );

                    if ( !imagery_layer.expired() )
                    {
                        // Step 4. Retrieve the Imagery raster layer driver so the layer can be properly created and visualized.
                        auto imagery_rep_driver = sdk_engine_context_->get_RasterLayerRepresentationDriverCollection()->get_Driver( "Imagery" );
                        if ( !imagery_rep_driver.expired() )
                        {
                            // Step 5. Create the actual RasterRepresentation. You can optionally capture the return of this,
                            //         but is not necessary for one-time visualization purposes, like in this example App.
							Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
							params.page_level = 0;
							params.priority = 0;
							params.representation_layer_activity = true;
                            imagery_rep_driver.lock()->get_RepresentationLayerCollection()->create( imagery_layer, params);
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
                        property_collection->put_Property( definition_collection->get_IndexByName( "VERTICAL_SCALE" ), "10" );
                        for ( unsigned long i = 0; i < definition_collection->count(); i++ )
                        {
                            std::cout << definition_collection->get_AtIndex( i )->property_name << std::endl;
                        }
                        if ( !elevation_rep_driver.expired() )
                        {
                            // Step 5. Create the actual RasterRepresentation. You can optionally capture the return of this,
                            //         but is not necessary for one-time visualization purposes, like in this example App.
							Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
							params.page_level = 0;
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
				navHelper_->add_Camera( camera_ );
			}

			camera_.lock()->get_CameraController()->put_Location ( longitude, latitude );
			camera_.lock()->get_CameraController()->put_Elevation( elevation, Geoweb3d::IGW3DPositionOrientation::Absolute );
            float pitch = 90.0f;
			camera_.lock()->get_CameraController()->put_Rotation ( 0.0f, pitch, 0.0f );
            navHelper_->put_HomePosition( camera_ );
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Forwards information to CreateShortTexturedDrapedLineLayer,
        ///           CreateLongTexturedDrapedLineLayer, CreateShortTexturedDrapedLineRepresentation,
        ///           and CreateLongTexturedDrapedLinesRepresentation to properly create Textured
        ///           Draped Line layers and Representations to visualize it in the map.              </summary>
        /// 
        /// <param name="short_line_layer_name"> Custom name of the shorter line layer being created. </param>
        /// 
        /// <param name="long_line_layer_name"> Custom name of the longer line layer being created.   </param>
        /// 
        /// <param name="longitude"> Center longitude of the imagery raster envelope.                 </param>
        /// 
        /// <param name="latitude"> Center latitude of the imagery raster envelope.                   </param>
        /// 
        /// <returns> true when successful and false if an error occurred or something failed.        </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool AddTexturedDrapedLines( const char* short_line_layer_name, const char* long_line_layer_name, double longitude, double latitude )
        {
            Geoweb3d::IGW3DVectorLayerWPtr textured_short_draped_line_layer = CreateShortTexturedDrapedLineLayer( short_line_layer_name, longitude, latitude );

            if ( textured_short_draped_line_layer.expired() )
            {
                std::cout << "Error creating " << short_line_layer_name << std::endl;
                return false;
            }

            if ( !CreateShortTexturedDrapedLineRepresentation( textured_short_draped_line_layer ) )
            {
                std::cout << "Error creating " << short_line_layer_name << " representation." << std::endl;
                return false;
            }

            Geoweb3d::IGW3DVectorLayerWPtr textured_long_draped_lines_layer = CreateLongTexturedDrapedLinesLayer( long_line_layer_name, longitude, latitude );

            if ( textured_long_draped_lines_layer.expired() )
            {
                std::cout << "Error creating " << long_line_layer_name << std::endl;
                return false;
            }

            if ( !CreateLongTexturedDrapedLinesRepresentation( textured_long_draped_lines_layer ) )
            {
                std::cout << "Error creating " << long_line_layer_name << " representation." << std::endl;
                return false;
            }

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates a Textured Draped Line Feature held by a singular layer.         </summary>
        /// 
        /// <param name="short_line_layer_name"> Custom name of the shorter line layer
        ///                                      being created.                                </param>
        /// 
        /// <param name="layer_lon"> Center longitude of the imagery raster envelope.          </param>
        /// 
        /// <param name="layer_lat"> Center latitude of the imagery raster envelope.           </param>                
        /// 
        /// <returns> true when successful and false if an error occurred or something failed. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		Geoweb3d::IGW3DVectorLayerWPtr CreateShortTexturedDrapedLineLayer( const char* short_line_layer_name, double layer_lon, double layer_lat )
		{
            // We're not using the field_definition for anything in particular for this example, but can be used for attribute mapping.
            // Without an initialized Geoweb3d::IGW3DDefinitionCollectionPtr, the VectorLayerCollection create call will fail for your layer, 
            // so it must be created / allocated. We also need it so we can create an empty / default property collection for the feature of the layer.
            Geoweb3d::IGW3DDefinitionCollectionPtr field_definition( Geoweb3d::IGW3DDefinitionCollection::create() );
            
            // Draped Lines use the gtLINE_STRING_25D geometry type.
            Geoweb3d::IGW3DVectorLayerWPtr layer = data_source_.lock()->get_VectorLayerCollection()->create( short_line_layer_name, Geoweb3d::gtLINE_STRING_25D, field_definition);
            
            // We need a default property collection to create the layer.
            Geoweb3d::IGW3DPropertyCollectionPtr default_property_collection( field_definition->create_PropertyCollection() );
            
            // This will place it in a road in the DC area.
            Geoweb3d::GW3DPoint short_start_point( layer_lon + 0.001, layer_lat );
            Geoweb3d::GW3DPoint short_end_point  ( layer_lon + 0.001, layer_lat - 0.001 );
            Geoweb3d::GW3DLineString short_line_geometry;
            short_line_geometry.put_CoordinateDimension( 2 );
            short_line_geometry.put_NumPoints( 2 );
            short_line_geometry.put_Point( 0, &short_start_point );
            short_line_geometry.put_Point( 1, &short_end_point );

            Geoweb3d::GW3DResult result;
            layer.lock()->create_Feature( default_property_collection, &short_line_geometry, result );

            return layer;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates two Textured Draped Line Features held by a singular layer.      </summary>
        /// 
        /// <param name="long_line_layer_name"> Custom name of the longer line layer
        ///                                     being created.                                 </param>
        /// 
        /// <param name="layer_lon"> Center longitude of the imagery raster envelope.          </param>
        /// 
        /// <param name="layer_lat"> Center latitude of the imagery raster envelope.           </param>                
        /// 
        /// <returns> true when successful and false if an error occurred or something failed. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        Geoweb3d::IGW3DVectorLayerWPtr CreateLongTexturedDrapedLinesLayer( const char* long_line_layer_name, double layer_lon, double layer_lat )
        {
            // We're not using the field_definition for anything in particular for this example, but can be used for attribute mapping.
            // Without an initialized Geoweb3d::IGW3DDefinitionCollectionPtr, the VectorLayerCollection create call will fail for your layer, 
            // so it must be created / allocated. We also need it so we can create an empty / default property collection for the feature of the layer.
            Geoweb3d::IGW3DDefinitionCollectionPtr field_definition( Geoweb3d::IGW3DDefinitionCollection::create() );

            // Draped Lines use the gtLINE_STRING_25D geometry type.
            Geoweb3d::IGW3DVectorLayerWPtr layer = data_source_.lock()->get_VectorLayerCollection()->create( long_line_layer_name, Geoweb3d::gtLINE_STRING_25D, field_definition );

            // We need a default property collection to create the layer.
            Geoweb3d::IGW3DPropertyCollectionPtr default_property_collection( field_definition->create_PropertyCollection() );

            Geoweb3d::GW3DResult result;

            // This line will be one longitude degree west of the DC area.
            Geoweb3d::GW3DPoint long_start_point( layer_lon - 1, layer_lat );
            Geoweb3d::GW3DPoint long_end_point  ( layer_lon - 1, layer_lat - 15.0 );
            Geoweb3d::GW3DLineString long_line_geometry;
            long_line_geometry.put_CoordinateDimension( 2 );
            long_line_geometry.put_NumPoints( 2 );
            long_line_geometry.put_Point( 0, &long_start_point );
            long_line_geometry.put_Point( 1, &long_end_point );
            layer.lock()->create_Feature( default_property_collection, &long_line_geometry, result );

            // This line will be one longitude degree east of the DC area.
            Geoweb3d::GW3DPoint long_segmented_start_point( layer_lon + 1, layer_lat );
            Geoweb3d::GW3DPoint long_segmented_end_point  ( layer_lon + 1, layer_lat - 15.0 );
            Geoweb3d::GW3DLineString long_segmented_line_geometry;
            long_segmented_line_geometry.put_CoordinateDimension( 2 );
            long_segmented_line_geometry.put_NumPoints( 2 );
            long_segmented_line_geometry.put_Point( 0, &long_segmented_start_point );
            long_segmented_line_geometry.put_Point( 1, &long_segmented_end_point );
            long_segmented_line_geometry.segmentize( 0.01 );
            layer.lock()->create_Feature( default_property_collection, &long_segmented_line_geometry, result );

            return layer;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Takes a Vector Layer and creates a Representation from it.
        ///           This will show how to create a Textured Draped Line and assign
        ///           a path to read texture data from and visualize in the scene.      </summary>
        /// 
        /// <param name="layer"> The Vector Layer that's been properly initialized.
        ///                      This will be used directly as a part of the
        ///                      Representation Driver's creation process to produce
        ///                      the Representation.                                    </param>
        /// 
        /// <returns> true if the Representation was properly created, otherwise false. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CreateShortTexturedDrapedLineRepresentation( Geoweb3d::IGW3DVectorLayerWPtr short_line_layer )
		{
            // For Draped Lines there is a "DrapedLine" driver.
            Geoweb3d::IGW3DVectorRepresentationDriverWPtr draped_line_rep_driver = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "DrapedLine" );

            // We'll add some default values for the DrapedLine's Representation creation parameters.
            Geoweb3d::IGW3DPropertyCollectionPtr   properties            = draped_line_rep_driver.lock()->get_PropertyCollection()->create_Clone();
            Geoweb3d::IGW3DDefinitionCollectionPtr definition_collection = properties->get_DefinitionCollection()->create_Clone();

            properties->put_Property( definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET" ),   0.0 );
            properties->put_Property( definition_collection->get_IndexByName( "TEXTURE_PALETTE_INDEX" ),  0   );
            properties->put_Property( definition_collection->get_IndexByName( "TEXTURE_TO_COLOR_BLEND" ), 1.0 );

            properties->put_Property( definition_collection->get_IndexByName( "RED" ),   1.0 );
            properties->put_Property( definition_collection->get_IndexByName( "GREEN" ), 1.0 );
            properties->put_Property( definition_collection->get_IndexByName( "BLUE" ),  1.0 );

			Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;
			rep_layer_creation_params.page_level = 1;
            rep_layer_creation_params.representation_default_parameters = properties;

            // Using the layer, we can now create a VectorRepresentation with the above set properties.
            short_textured_draped_line_representation_ = draped_line_rep_driver.lock()->get_RepresentationLayerCollection()->create( short_line_layer, rep_layer_creation_params );
            
            // To add an image collection to the Representation, query for the IGW3DSceneGraphContext and create a new IGW3DImageCollection.
            // Off of the image collection we can visualize a texture at palette index 0.
            Geoweb3d::IGW3DImageCollectionPtr image_collection = sdk_engine_context_->get_SceneGraphContext()->create_ImageCollection();
            image_collection->create( "..//examples//media//Texture//Arrow.png" );
            short_textured_draped_line_representation_.lock()->put_GW3DFinalizationToken( image_collection->create_FinalizeToken() );

			return true;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Takes a Vector Layer and creates a Representation from it.
        ///           This will show how to create a Textured Draped Line and assign
        ///           a path to read texture data from and visualize in the scene.      </summary>
        /// 
        /// <param name="layer"> The Vector Layer that's been properly initialized.
        ///                      This will be used directly as a part of the
        ///                      Representation Driver's creation process to produce
        ///                      the Representation.                                    </param>
        /// 
        /// <returns> true if the Representation was properly created, otherwise false. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool CreateLongTexturedDrapedLinesRepresentation( Geoweb3d::IGW3DVectorLayerWPtr long_lines_layer )
        {
            // For Draped Line there is a "DrapedLine" driver.
            Geoweb3d::IGW3DVectorRepresentationDriverWPtr draped_line_rep_driver = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "DrapedLine" );

            // We'll add some default values for the DrapedLine's Representation creation parameters.
            Geoweb3d::IGW3DPropertyCollectionPtr   properties            = draped_line_rep_driver.lock()->get_PropertyCollection()->create_Clone();
            Geoweb3d::IGW3DDefinitionCollectionPtr definition_collection = properties->get_DefinitionCollection()->create_Clone();

            properties->put_Property( definition_collection->get_IndexByName( "TRANSLATION_Z_OFFSET" ),   0.0 );
            properties->put_Property( definition_collection->get_IndexByName( "TEXTURE_PALETTE_INDEX" ),  0   );
            properties->put_Property( definition_collection->get_IndexByName( "TEXTURE_TO_COLOR_BLEND" ), 1.0 );
            
            properties->put_Property( definition_collection->get_IndexByName( "RED" ),   1.0 );
            properties->put_Property( definition_collection->get_IndexByName( "GREEN" ), 1.0 );
            properties->put_Property( definition_collection->get_IndexByName( "BLUE" ),  1.0 );

            properties->put_Property( definition_collection->get_IndexByName( "LINE_WIDTH" ),    32   );
            properties->put_Property( definition_collection->get_IndexByName( "WIDTH" ),         1000 );
            properties->put_Property( definition_collection->get_IndexByName( "OUTLINE_MODE" ),  Geoweb3d::Vector::OutlineMode::OUTLINE_ON  );

            Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;
            rep_layer_creation_params.page_level = 1;
            rep_layer_creation_params.representation_default_parameters = properties;

            // Using the layer, we can now create a VectorRepresentation with the above set properties.
            long_textured_draped_lines_representation_ = draped_line_rep_driver.lock()->get_RepresentationLayerCollection()->create( long_lines_layer, rep_layer_creation_params );

            // To add an image collection to the Representation, query for the IGW3DSceneGraphContext and create a new IGW3DImageCollection.
            // Off of the image collection we can visualize a texture at palette index 0.
            Geoweb3d::IGW3DImageCollectionPtr image_collection = sdk_engine_context_->get_SceneGraphContext()->create_ImageCollection();
            image_collection->create( "..//examples//media//Texture//Arrow.png" );
            long_textured_draped_lines_representation_.lock()->put_GW3DFinalizationToken( image_collection->create_FinalizeToken() );

            return true;
        }

	private:

		Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_engine_context_;
        Geoweb3d::IGW3DWindowWPtr     window_;
        Geoweb3d::IGW3DCameraWPtr     camera_;
        NavigationHelper*             navHelper_;

        Geoweb3d::IGW3DVectorDataSourceWPtr data_source_;
        
        Geoweb3d::IGW3DRasterLayerWPtr imagery_layer_;

        Geoweb3d::IGW3DVectorRepresentationWPtr short_textured_draped_line_representation_;
        Geoweb3d::IGW3DVectorRepresentationWPtr long_textured_draped_lines_representation_;
        
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Runs the Textured Draped Lines Application instance.
///           This will continuosly run until the window is closed.      </summary>
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
///           Runs the Textured Draped Lines example application. </summary>
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
        sdk_init->put_ESRILicenseCheckout( false ); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
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

void my_fatal_function( const char* msg  )
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