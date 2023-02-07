#include "stdafx.h"
#include "Util.h"
#include "OpenVRStereoDevice.h"
#include "../Common/EntityController.h"
#include "../Common/NavigationHelper.h"
#include "../Common/MiniEngineCore.h"

#include <string>
#include <iostream>

#include <GeoWeb3dCore/SystemExports.h>
#include <Geoweb3dCore/LayerParameters.h>
#include <Geoweb3dCore/CoordinateExports.h>

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/IGW3DStereoDriverCollection.h"
#include "engine/IGW3DStereoDriver.h"
#include "engine/IGW3DEventStream.h"

//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

/*
To get started:
Install Steam and SteamVR
Download and install Steam : https://store.steampowered.com/about/
Note: You will need to create an account with Steam
Through Steam install SteamVR : http://steam://run/250820

Update your graphics drivers to the latest version:
NVIDIA Drivers : http://www.nvidia.com/Download/index.aspx?lang=en-us
AMD Drivers : http://support.amd.com/en-us/download

Make sure your computer is VR Ready : http://store.steampowered.com/app/323910/

Install Hardware Specifc Software. (Instructions are usually packaged with the device)
HTC Vive : https://www.vive.com/us/setup/
Oculus Rift : https://www.oculus.com/setup/

Launch SteamVR
You can do this by clicking the VR button that appears in the top right corner of the
Steam client.
You can also launch SteamVR from the Steam library under Tools.
The SteamVR Status window will show up. Follow the prompts to fnish installing
your Device.
*/

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events, Geoweb3d::IGW3DStereoDriverPtr driver );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );
Geoweb3d::IGW3DVectorDataSourceWPtr CreateGeoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *name );
Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer );

class MiniEngine
    : public Geoweb3d::IGW3DWindowCallback
    , public Geoweb3d::IGW3DEventStream
    , public MiniEngineCore
{
public:
    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
		: sdk_context_(sdk_context)
		, nav_helper_ ( new NavigationHelper() )
	{}

    virtual ~MiniEngine()
    {
    }

    bool Update()
    {
        DoPreDrawWork();

		bool stereo_enabled = camera_.lock()->get_CameraStereoSettings()->get_Enable();

		int v_sync_state = stereo_enabled
			? 0		// vsync disabled for Stero Device
			: -1;	// adaptive vsync 

        if( sdk_context_->draw( window_, v_sync_state ) == Geoweb3d::GW3D_sOk )
        {
            DoPostDrawWork();
            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

	Geoweb3d::IGW3DStereoDriverPtr CreateStereoDriver( Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context )
	{
		Geoweb3d::IGW3DStereoDevicePtr my_device( new OpenVRStereoDevice() );
		Geoweb3d::IGW3DStereoDriverWPtr driver = sdk_context->get_StereoDriverCollection()->Create( my_device );
		return driver.lock();
	}

    bool LoadConfiguration(const char *example_name )
    {
		Geoweb3d::IGW3DStereoDriverPtr driver = CreateStereoDriver( sdk_context_ );
        if (driver)
        {
            bool vr_capable = false;
            bool vr_hardware_available = false;
            vr_capable = driver->get_IsSystemSupported();
            vr_hardware_available = driver->get_IsHardwareDetected();
            printf("VR Capable: %i, HMD Available: %i \n", (int)vr_capable, (int)vr_hardware_available);
        }

        window_ = CreateWindowCreation( sdk_context_, example_name, this, driver );

        if(window_.expired())return false;

        camera_ = CreateCamera(window_,"Camera 1");
        camera_controller_ = camera_.lock()->get_CameraController();

        if(camera_.expired())
			return false;

		nav_helper_->add_Camera(camera_);

		camera_.lock()->get_CameraStereoSettings()->put_Enable( true );

        sdk_context_->put_EnableEventStream( true );

        return true;
    }

    bool LoadData( const char *rootpath )
    {
        //imagery has yet to be brought over to the C++ api.
        std::string imagerypath(rootpath);
        imagerypath += "/DC_1ft_demo.ecw";

        if (CreateImageryRep(sdk_context_, imagerypath.c_str(), 6, 0, true).expired())
        {
            printf("CreateImageryRep Error\n");
        }

        std::string vectorpath(rootpath);
		vectorpath  += "/Building Footprints.shp";

        Geoweb3d::GW3DResult res;
        //NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
        //having to guess which data source driver is needed to open up a particular dataset.
        Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open( vectorpath.c_str(), res );
        RepresentAsExtruded(sdk_context_, data_source.lock()->get_VectorLayerCollection()->get_AtIndex(0));

		//lets try to get a starting point for the camera by looking at all the vector data
        //and zoom to the center area of its bounds. This is more of an SDK test too, as
        //we go through all the vector drivers and gather all the layers etc. to find
        //the overall 2D bounding box.
        Geoweb3d::GW3DEnvelope env;

		Geoweb3d::IGW3DVectorDriverWPtr vdriver;
        while( sdk_context_->get_VectorDriverCollection()->next(&vdriver) )
        {
            //note, going through a lock() all the time creates alot of extra work
            //we can avoid when we are in a tight loop, as we know we are all good programers
            Geoweb3d::IGW3DVectorDataSourceCollection *vdscollection = vdriver.lock()->get_VectorDataSourceCollection();

            Geoweb3d::IGW3DVectorDataSourceWPtr vdatasource;
            while(vdscollection->next(&vdatasource))
            {
                Geoweb3d::IGW3DVectorLayerCollection *vlyrcollection = vdatasource.lock()->get_VectorLayerCollection();

                Geoweb3d::IGW3DVectorLayerWPtr layer;

                while(vlyrcollection->next(&layer))
                {
                    env.merge( layer.lock()->get_Envelope() );
                }
            }
        }

        camera_controller_->put_Location(((env.MinX + env.MaxX ) * 0.5) , ((env.MinY + env.MaxY ) * 0.5) - .0002);
        camera_controller_->put_Elevation( 1.27, Geoweb3d::IGW3DPositionOrientation::Absolute );
        camera_controller_->put_Rotation( 0.0, 0.0, 0.0 );
        return true;
    }

private:
    void DoPreDrawWork()
    {
        sdk_context_->run_EventStream( *this );
    }

    void DoPostDrawWork()
    {
    }

private:

	virtual void OnCreate( )
	{
	}

	virtual void OnDrawBegin( )
	{
	}

	virtual void OnDrawEnd( )
	{
	}

    virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
    {
    }


    // OS event system, raw data directly from the windows message pump.
    virtual int ProcessLowLevelEvent( OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam )
    {
        return 1;
    }

    virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
    {
		current_camera_ = nav_helper_->ProcessEvent(win_event, window_);

		switch( win_event.Type )
        {
        case Geoweb3d::WindowEvent::KeyPressed:
        {
            switch( win_event.Key.code )
            {
			case Geoweb3d::Key::V:
			{
				bool enabled = !camera_.lock()->get_CameraStereoSettings()->get_Enable();

				Geoweb3d::GW3DResult result = camera_.lock()->get_CameraStereoSettings()->put_Enable( enabled );
				if ( Geoweb3d::Succeeded( result ) )
				{
					std::cout << "VR enable state chaanged: " << enabled << std::endl;
				}
				else
				{
					std::cout << "Error Encountered trying to change the enable state: " << result << std::endl;
				}
			}
			break;

			case Geoweb3d::Key::S:
			{
				CreateDynamicEntityLayer_( camera_.lock()->get_Name() );
				int entity_id = SpawnDynamicEntity_();
				if ( entity_id != -1 )
				{
					entity_representation_.lock()->put_Enabled( true );
				}
			}
			break;

			case Geoweb3d::Key::T:
            {
                if ( !entity_layer_.expired() )
                {
                    double lon = 0.0;
                    double lat = 0.0;
                    double elev = 0.0;
                    Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode = Geoweb3d::IGW3DPositionOrientation::Absolute;
                    camera_controller_->get_Location( lon, lat );
                    camera_controller_->get_Elevation( elev, mode );

                    elev += Geoweb3d::Coordinates::MajorRadius();

                    double x, y, z;
                    Geoweb3d::Coordinates::GeodeticToCartesian( lon, lat, elev, x, y, z, false );

                    double dir_offset = 10.0;
                    double dir_x, dir_y, dir_z;
                    camera_controller_->get_Direction( dir_x, dir_y, dir_z );
                    x += ( dir_x * dir_offset );
                    y += ( dir_y * dir_offset );
                    z += ( dir_z * dir_offset );

                    double right_offset = 0.0;
                    double right_x, right_y, right_z;
                    camera_controller_->get_Right( right_x, right_y, right_z );
                    x += ( right_x * right_offset );
                    y += ( right_y * right_offset );
                    z += ( right_z * right_offset );

                    double up_offset = 1.0;
                    double up_x, up_y, up_z;
                    camera_controller_->get_Up( up_x, up_y, up_z );
                    x += ( up_x * up_offset );
                    y += ( up_y * up_offset );
                    z += ( up_z * up_offset );

                    Geoweb3d::Coordinates::CartesianToGeodetic( x, y, z, lon, lat, elev, false );
                    elev -= Geoweb3d::Coordinates::MajorRadius();

                    TourPathPoint entity_update;
                    entity_update.longitude = lon;
                    entity_update.latitude = lat;
                    entity_update.elevation = elev;
                    entity_update.yaw = 0.0;
                    entity_update.pitch = 0.0;
                    entity_update.roll = 0.0;

                    entity_stream_.SeEntityPosition( 0, entity_update );
                    entity_layer_.lock()->Stream( &entity_stream_ );
                }
            }
			break;

            default:
            break;
            }
        }
        break;
        default:
        break;
        }
    }

    void OnPagingEvent(Geoweb3d::IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const Geoweb3d::GW3DEnvelope& bounds, unsigned int value) override
    {
    }

    void OnStereoEvent(Geoweb3d::IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype) override
    {
        if (etype == Geoweb3d::IGW3DEventStream::HMD_CONNECTED)
        {
            printf("A HMD has been connected. \n");
        }
        else if (etype == Geoweb3d::IGW3DEventStream::HMD_DISCONNECTED)
        {
            printf("A HMD has been disconnected. \n");
        }
    }

	void CreateDynamicEntityLayer_( const char* layer_name )
	{
		if ( !entity_datasource_.expired() )
			return;

		Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );
		if ( !driver.expired() )
		{
			Geoweb3d::IGW3DVectorDataSourceCollection *datasource_collection = driver.lock()->get_VectorDataSourceCollection();
			std::string data_source_name = layer_name;
			data_source_name += ":Dynamic_Entities.geoweb3d";
			entity_datasource_ = datasource_collection->create( data_source_name.c_str() );
		}
		else
		{
			printf( "SDK Bug Detected\n" );
			return;
		}

		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
		Geoweb3d::IGW3DPropertyCollectionPtr field_values(field_definition->create_PropertyCollection());

		entity_layer_ = entity_datasource_.lock()->get_VectorLayerCollection()->create("Entity_Dynamic", Geoweb3d::gtPOINT_25D, field_definition);
		if ( !entity_layer_.expired() )
		{
			entity_layer_.lock()->put_GeometryEditableMode( true );

			Geoweb3d::IGW3DVectorRepresentationDriverWPtr model_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "Model" );

			Geoweb3d::GW3DResult validitychk;
			if ( !Geoweb3d::Succeeded( validitychk = model_driver.lock()->get_CapabilityToRepresent(entity_layer_) ) )
			{
				printf( "not able to mix this geometry type with the rep..\n" );
			}
			else
			{
				//good to go!
				Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
				params.page_level = 1;
				params.representation_default_parameters = model_driver.lock()->get_PropertyCollection()->create_Clone();
				std::string url = R"(data\sdk\models\DIS\t72m1_tank.flt)";
				params.representation_default_parameters->put_Property( Geoweb3d::Vector::ModelProperties::URL, url.c_str() );
				params.representation_layer_activity = false;

				entity_representation_ = model_driver.lock()->get_RepresentationLayerCollection()->create( entity_layer_, params );
				entity_stream_.SetRepresentation( entity_representation_ );
			}
		}
	}

	int SpawnDynamicEntity_()
	{
		if ( entity_layer_.expired() )
			return -1;

		double lon = 0.0;
		double lat = 0.0;
		double elev = 0.0;
		Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode = Geoweb3d::IGW3DPositionOrientation::Absolute;
		camera_controller_->get_Location( lon, lat );
		camera_controller_->get_Elevation( elev, mode );

		Geoweb3d::GW3DPoint geom = Geoweb3d::GW3DPoint( lon, lat, elev );
		Geoweb3d::GW3DResult result;

		Geoweb3d::IGW3DPropertyCollectionPtr field_values( entity_layer_.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection() );

		long feature_id = entity_layer_.lock()->create_Feature( field_values, &geom, result );

		if ( !Geoweb3d::Succeeded( result ) )
		{
			printf( "create_FeatureWithinMemoryFile Error\n" );
			return -1;
		}

		return static_cast<int>( feature_id );
	}

private:
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    Geoweb3d::IGW3DWindowWPtr window_;
    Geoweb3d::IGW3DCameraWPtr camera_;
    Geoweb3d::IGW3DCameraController *camera_controller_;

	// navigation
	NavigationHelper* nav_helper_;
	Geoweb3d::IGW3DCameraWPtr current_camera_;

	// entity controlling
	EntityController entity_stream_;
	Geoweb3d::IGW3DVectorDataSourceWPtr entity_datasource_;
	Geoweb3d::IGW3DVectorLayerWPtr entity_layer_;
	Geoweb3d::IGW3DVectorRepresentationWPtr entity_representation_;
}; //engine end of class

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    MiniEngine *pengine = new MiniEngine( sdk_context );

    if(pengine->LoadConfiguration("Example - User Stereo Device"))
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char* root_path = "../examples/media";
        if(pengine->LoadData( root_path ))
        {

        }
        else
        {
			printf("\n\nCould not load data, you probably need to setup the path to your data\n");
        }

        //the engine loaded all its data ok
        while( pengine->Update() )
        {
            //could do other app stuff here
        }
    }
    delete pengine;
}

int _tmain(int argc, _TCHAR* argv[])
{
    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if(sdk_context)
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
            printf("\nSDK Context Creation Error! Press any key to continue...\n");
            getchar();
        }
    }
    else
    {
        return -1;
    }

    //example to show if you want to control its when the sdk context gets destroyed.  This will invalidate all
    // the pointers the SDK owned!  (xxx.expired() ==true)
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();


    return 0;
}

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0, Geoweb3d::IGW3DStereoDriverPtr stereo_driver = Geoweb3d::IGW3DStereoDriverPtr() )
{
    Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );

    Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title, GW3D_OVERLAPPED, 10, 10, 800,600, 0, stereo_driver, window_events);

    if(window.expired())
    {
        printf("Error Creating window: [%s]!\n", title );

        return Geoweb3d::IGW3DWindowWPtr();
    }

    return window;
}

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer )
{

    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPolygon" );


    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representatin
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a extruded polygon propery: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name );
        }

        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}

Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle )
{
    Geoweb3d::IGW3DCameraCollection *cameracollection = windowptr.lock()->get_CameraCollection();
    Geoweb3d::IGW3DCameraWPtr camera =	cameracollection->create( cameratitle );
    return camera;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates geoweb 3D data source. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">   	Context for the sdk. </param>
/// <param name="datasourcename">	The datasourcename. </param>
///
/// <returns>	The new geoweb 3D data source. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DVectorDataSourceWPtr CreateGeoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *datasourcename )
{
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

    if(!driver.expired())
    {
        Geoweb3d::IGW3DVectorDataSourceCollection *datasource_collection = driver.lock()->get_VectorDataSourceCollection();

        return datasource_collection->create( datasourcename  );
    }
    else
    {
        printf("SDK Bug Detected\n");
    }

    return Geoweb3d::IGW3DVectorDataSourceWPtr();
}
