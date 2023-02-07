// Smoke.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

#include "GeoWeb3dCore/SystemExports.h"

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "../Common/MiniEngineCore.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();


Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );
Geoweb3d::IGW3DVectorDataSourceWPtr CreateGeoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *name );
Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer );


class MiniEngine : public Geoweb3d::IGW3DWindowCallback, MiniEngineCore
{
public:
    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) : sdk_context_(sdk_context) {}
    virtual ~MiniEngine()
    {
    }


    bool Update()
    {
        DoPreDrawWork();

        if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
        {
            double lon,lat;
            camera_controller_->get_Location(lon,lat);
            //camera_controller_->put_Location(lon,lat+=.0000001 );

            double elevation;
            Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
            camera_controller_->get_Elevation(elevation, mode);
            camera_controller_->put_Elevation(elevation + .001, mode);
            float heading,  pitch, roll;
            camera_controller_->get_Rotation( heading,  pitch, roll);
            //camera_controller_->put_Rotation(heading,  pitch+=.02f, roll+=.02f);

            DoPostDrawWork();
            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

    bool LoadConfiguration(const char *example_name )
    {
        window_ = CreateWindowCreation( sdk_context_, example_name, this );

        if(window_.expired())return false;

        camera_ = CreateCamera(window_,"Camera 1");
        camera_controller_ = camera_.lock()->get_CameraController();

        if(camera_.expired())return false;

		camera_.lock()->get_CameraStereoSettings()->put_Enable(true);

        sdk_context_->put_EnableEventStream( true );

        return true;
    }

    bool LoadData( const char *rootpath )
    {
        Geoweb3d::IGW3DVectorRepresentationDriverWPtr smoke_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "Smoke" );

        Geoweb3d::GW3DResult validitychk;

        if(!Geoweb3d::Succeeded( validitychk =  smoke_driver.lock()->get_CapabilityToRepresent( local_smoke_layer ) ))
        {
            printf("not able to mix this geometry type with the rep..\n");
        }
        else
        {
            //good to go!
            Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
            smokerepresentation = smoke_driver.lock()->get_RepresentationLayerCollection()->create( local_smoke_layer, params );
        }

        Geoweb3d::IGW3DVectorRepresentationDriverWPtr model_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "Model" );

        if(!Geoweb3d::Succeeded( validitychk =  model_driver.lock()->get_CapabilityToRepresent( local_smoke_layer ) ))
        {
            printf("not able to mix this geometry type with the rep..\n");
        }
        else
        {
            //good to go!
            Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
            //smokerepresentation = model_driver.lock()->get_RepresentationLayerCollection()->create( local_smoke_layer, params );
        }


        //imagery has yet to be brought over to the C++ api.
        std::string imagerypath(rootpath);
        imagerypath += "/DC_1ft_demo.ecw";

        if (CreateImageryRep(sdk_context_, imagerypath.c_str(), 6, 0, true).expired())
        {
            printf("CreateImageryRep Error\n");
        }

        imagerypath = rootpath;
        imagerypath  += "/Building Footprints.shp";

        Geoweb3d::GW3DResult res;
        //NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
        //having to guess which data source driver is needed to open up a particular dataset.
        Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open( imagerypath.c_str(), res);
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
        camera_controller_->put_Elevation( 25, Geoweb3d::IGW3DPositionOrientation::Relative );
        camera_controller_->put_Rotation( 0.0, 0.0, 0.0 );

        return true;
    }

private:
    void DoPreDrawWork()
    {
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


private:

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    Geoweb3d::IGW3DWindowWPtr window_;
    Geoweb3d::IGW3DCameraWPtr camera_;
    Geoweb3d::IGW3DCameraController *camera_controller_;

    /// <summary>	The features datasource. </summary>
    Geoweb3d::IGW3DVectorDataSourceWPtr  smoke_datasource;
    /// <summary>	The  layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr local_smoke_layer;
    /// <summary>	The representation. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr  smokerepresentation;
}; //engine end of class

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

    MiniEngine *pengine = new MiniEngine( sdk_context );

    if(pengine->LoadConfiguration("Example  - Stereo"))
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char * root_path = "../examples/media";
        //  download the Geoweb3d DC sample project here: http://www.geoweb3d.com/download/sample-projects/
        if(pengine->LoadData( root_path ))
        {

        }
        else
        {
			printf("\n\nCould not load data, you probably need to setup the path to your data\n");
			printf("Download the Geoweb3d DC sample project here:\nhttp://www.geoweb3d.com/download/sample-projects/\n\n\n");
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

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0 )
{
    Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );

    Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title, GW3D_OVERLAPPED, 10, 10, 1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), window_events);

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

    if(!camera.expired())
    {
        printf("DelimitedString: %s\n",   camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
        printf("UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());

        camera.lock()->get_DateTime()->put_isUseComputerDate(false);
        camera.lock()->get_DateTime()->put_isUseComputerTime(false);


        if(!camera.lock()->get_DateTime()->put_DateFromDelimitedString("2002-1-25"))
        {
            printf("Error with put_DateFromDelimitedString\n");
        }

        if(!camera.lock()->get_DateTime()->put_DateFromUnDelimitedString("2002125"))
        {
            printf("Error with put_DateFromUnDelimitedString\n");
        }


        printf("User Set: DelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromDelimitedString()->c_str());
        printf("User Set: UnDelimitedString: %s\n", camera.lock()->get_DateTime()->get_DateFromUnDelimitedString()->c_str());

        camera.lock()->get_DateTime()->put_Time(12,0);
    }

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

/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg )
{
    printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char *msg )
{
    printf("General Info: %s", msg);
}

/*! Information handling is not required */
void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}