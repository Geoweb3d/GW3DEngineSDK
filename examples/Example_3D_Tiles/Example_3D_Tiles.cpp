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

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment( lib, "GW3DCommon.lib" )

void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic application class encapsulating a Geoweb3d SDK engine context,
///           a viewable window, and a camera the window relies on. This example App
///           class will exemplify how to load 3DTiles. </summary>
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
			window_ = sdk_engine_context_->get_WindowCollection()->create_3DWindow( "3D Tiles", GW3D_OVERLAPPED, 10, 10, 1280, 720, 0, Geoweb3d::IGW3DStereoDriverPtr(), this ); 
			
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

            // Loads raster imagery layers via the imagery driver(s)
            CreateGlobalImageryLayer();

            //Place your user specific 3D-Tile data source into this folder.
   		    //LoadDataset( "..//examples//media//" );

			if ( representations_ )
			{
				for ( unsigned int i = 0; i < representations_->count(); i++ )
				{
					auto locked_rep = representations_->get_AtIndex( i ).lock();
					if ( locked_rep )
					{
						auto base_scene_layer = locked_rep->get_VectorLayer().lock();
						if ( base_scene_layer )
						{
							Geoweb3d::GW3DEnvelope layer_envelope = base_scene_layer->get_Envelope();
							if ( !layer_envelope.get_IsNull() )
							{
								double longitude = ( layer_envelope.MaxX + layer_envelope.MinX ) * 0.5;
								double latitude  = ( layer_envelope.MaxY + layer_envelope.MinY ) * 0.5;
								double camera_elev = 500.0;
								UpdateCamera( longitude, latitude, camera_elev );
								break;
							}
						}
					}
				}
			}
			else if ( auto locked_raster_layer = imagery_layer_.lock() )
			{
				Geoweb3d::GW3DEnvelope raster_envelope = locked_raster_layer->get_Envelope();
				if ( !raster_envelope.get_IsNull() )
				{
					double longitude = ( raster_envelope.MaxX + raster_envelope.MinX ) * 0.5;
					double latitude = ( raster_envelope.MaxY + raster_envelope.MinY ) * 0.5;
					double camera_elev = 500.0;
					UpdateCamera( longitude, latitude, camera_elev );
				}
			}
			else
			{
				Geoweb3d::GW3DEnvelope raster_envelope;
				raster_envelope.put( -77.10073231, -76.99214369, 38.87198808, 38.90985241, 53.77714057, 53.78162677 );

				double longitude = ( raster_envelope.MaxX + raster_envelope.MinX ) * 0.5;
				double latitude = ( raster_envelope.MaxY + raster_envelope.MinY ) * 0.5;
				double camera_elev = ( raster_envelope.MaxZ + raster_envelope.MinZ ) * 0.5;
				UpdateCamera( longitude, latitude, camera_elev + 500.0 );
			}

			if ( camera_.expired() )
			{
				UpdateCamera( 0.0, 0.0, 500.0 );
			}

			return true;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an imagery layer from a raster file. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void CreateGlobalImageryLayer()
        {
            // Step 1. Either use auto_Open or retrieve the proper RasterDriver for the particular data type.
            //         Auto open will attempt to use the best-match raster driver for the data type.
            Geoweb3d::GW3DResult res;
            auto imagery_data_source = sdk_engine_context_->get_RasterDriverCollection()->auto_Open( "..//examples//media//WSI-Earth99-2k.ecw", res );
            if ( !imagery_data_source.expired() )
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
                            imagery_rep_driver.lock()->get_RepresentationLayerCollection()->create( imagery_layer, params );
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
		/// <summary> Loads a 3D Tiles dataset from disk. </summary>
		/// 
		/// <param name="dataset_filename"></param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void LoadDataset( const std::string& dataset_filename )
		{
			Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "3D_Tiles" );
			auto locked_driver = driver.lock();
			if ( locked_driver )
			{
                Geoweb3d::GW3DResult res;
				data_source_ = locked_driver->get_VectorDataSourceCollection()->open( dataset_filename.c_str(), res );
                auto locked_ds = data_source_.lock();
                if ( locked_ds )
                {
                    //NOTE: Using auto_Open allows the system to select the default, built-in representation corresponding to the 
                    //the specified data source. Certain data sources have their specific internal representation that needs to be
                    //enforced and aut_Open is the means by which to do so. This will ensure that the most suitable representation for
                    //the datasource is selected.
					representations_ = sdk_engine_context_->get_VectorRepresentationDriverCollection()->auto_Open( data_source_ );
                }
			}
		}

    private:
		Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_engine_context_;
		Geoweb3d::IGW3DWindowWPtr     window_;
		Geoweb3d::IGW3DCameraWPtr     camera_;
		NavigationHelper* navHelper_;

		Geoweb3d::IGW3DRasterLayerWPtr imagery_layer_;
		Geoweb3d::IGW3DVectorDataSourceWPtr data_source_;
        Geoweb3d::IGW3DVectorRepresentationHelperCollectionPtr representations_;
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