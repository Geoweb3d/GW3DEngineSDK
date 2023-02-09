// LineRepresentations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "engine/IGW3DGeoweb3dSDK.h"

//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

#include "GeoWeb3dCore/SystemExports.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

char myinfobuf[1024] = {0};
void my_fatal_function(const char *msg,...);
void my_info_function(const char *msg,...);
void SetInfomationHandling();
Geoweb3d::IGW3DVectorDataSourceWPtr create_Geoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *datasourcename )
{
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

    if(!driver.expired())
    {
        Geoweb3d::IGW3DVectorDataSourceCollection *datasource_collection = driver.lock()->get_VectorDataSourceCollection();

        return datasource_collection->create( datasourcename );
    }
    else
    {
        printf("SDK Bug Detected\n");
    }

    return Geoweb3d::IGW3DVectorDataSourceWPtr();
}

void RunApplication(  Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

    if(driver.expired())
    {
        printf("Could  not find the driver requested!  For a list of supported drivers, run the Print Capabilities example");
        return;
    }

	if (!driver.lock()->get_IsCapable(isCreateDataSourceSupported))
	{
		printf("This driver is not capable of creating new datasources!\n");
		return;
	}

    Geoweb3d::IGW3DVectorDataSourceWPtr created_datasource = create_Geoweb3dDataSource(sdk_context, "Dynamic_Features.geoweb3d");

    Geoweb3d::GeometryType geotype = Geoweb3d::gtLINE;

	//Showing a little bit of attribute mapping from the datasource too.  So we create our custom datasource, where in the field
	//we add the texture to use when we represent this feature.
    Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
    field_definition->add_Property("counter time?",Geoweb3d::PROPERTY_DOUBLE, "just something to add.");

	// create some layers
    Geoweb3d::IGW3DVectorLayerWPtr waypoints	= created_datasource.lock()->get_VectorLayerCollection()->create("WayPoints",geotype, field_definition	);
	waypoints.lock()->put_GeometryEditableMode(true);

	// create some features in one of the layers

    Geoweb3d::IGW3DPropertyCollectionPtr field_values (field_definition->create_PropertyCollection());

    Geoweb3d::GW3DPoint pa(18.491890828685225 ,-33.945519782777708 ,  0.04950790405273);
	Geoweb3d::GW3DPoint pb(18.491890828685225 ,-33.945519782777708 ,  117.74950790405273);


	Geoweb3d::GW3DLineString linestring;

	linestring.add_Point(&pa);
	linestring.add_Point(&pb);


	Geoweb3d::GW3DResult result;
	//showing we will be able to load off disk
	field_values->put_Property(0,0.0);
    printf("FeatureID created: %d \n", waypoints.lock()->create_Feature(field_values, &linestring, result) );


	Geoweb3d::IGW3DWindowWPtr window = sdk_context->get_WindowCollection(  )->create_3DWindow("Line Representations", GW3D_OVERLAPPED, 10, 10, 800,600, 0, Geoweb3d::IGW3DStereoDriverPtr(), 0);
	
    sdk_context->draw( window );

	Geoweb3d::IGW3DCameraWPtr camera1 = window.lock()->get_CameraCollection()->create("Main Camera");

	//figure out where to zoom
    Geoweb3d::IGW3DVectorLayerCollection *vlayercollection=  created_datasource.lock()->get_VectorLayerCollection();

    Geoweb3d::IGW3DVectorLayerWPtr tester;
	Geoweb3d::GW3DEnvelope env;
    while(vlayercollection->next( &tester)) //just to see
    {
		env.merge( tester.lock()->get_Envelope() );
   }

	//most gis datasource layers don't have a Z value for the bounds,
	//so here just set it to the last fid for now

	env.MaxZ = pb.get_Z();

    double longitude, latitude;
    longitude = (env.MinX + env.MaxX ) * 0.5;
    latitude  = (env.MinY + env.MaxY ) * 0.5;
	latitude = latitude - .0005;
    camera1.lock()->get_CameraController()->put_Location( longitude, latitude);

	camera1.lock()->get_CameraController()->put_Rotation(0,50,0);
    camera1.lock()->get_CameraController()->put_Elevation(env.MaxZ + 40, Geoweb3d::IGW3DPositionOrientation::Relative );

	//
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr line = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ColoredLine" );
	Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
	Geoweb3d::IGW3DVectorRepresentationWPtr rep = line.lock()->get_RepresentationLayerCollection()->create( waypoints,params);




   while( sdk_context->draw( window )==Geoweb3d::GW3D_sOk )
   {


   }
}

int _tmain(int argc, _TCHAR* argv[])
{
    SetInfomationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if(sdk_context)
    {
        if(Geoweb3d::Succeeded(sdk_context->InitializeLibrary("geoweb3dsdkdemo", sdk_context->create_InitializationConfiguration(), 5, 0 )))
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


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg,...)
{
    va_list ap;
    va_start( ap, msg );
    vsprintf_s(myinfobuf,msg,ap);
    va_end(ap);
    printf("Fatal Info: %s", myinfobuf);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char *msg,...)
{
    va_list ap;
    va_start( ap, msg );
    vsprintf_s(myinfobuf,msg,ap);
    va_end(ap);
    printf("General Info: %s", myinfobuf);
}

/*! Information handling is not required */
void SetInfomationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}
