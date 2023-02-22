// LineRepresentations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <deque>

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DEnvironmentEffects.h"
#include "engine/IGW3DEnvironmentLabs.h"

#include "GeoWeb3dCore/SystemExports.h"

#include "../Common/MiniEngineCore.h"


#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();
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

struct FidZoomer : public Geoweb3d::IGW3DVectorLayerStream
{
	virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult *result ) 
	{	
		feature_envelopes.push_back(Geoweb3d::GW3DEnvelope());
		result->get_Geometry()->get_Envelope(&feature_envelopes.back() );
		return true;
	}

	virtual bool OnError() 
	{
		return true;
	}

	////////////////////
    virtual unsigned long count() const  
	{
		return 0;  //return 0 as we want the whole layer!
	
	}
    virtual bool next( int64_t *ppVal )
	{
		return false;
	
	}
    virtual void reset()
	{
		feature_envelopes.clear();
	
	}
    virtual int64_t operator[](unsigned long index)
	{
		return 0;
	}

	 virtual int64_t get_AtIndex( unsigned long index )
	 {
		return 0;
	 }

	 //////////////
	std::deque<Geoweb3d::GW3DEnvelope> feature_envelopes;
};

void RunApplication(  Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	Geoweb3d::GW3DResult res;

	//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
	//having to guess which data source driver is needed to open up a particular dataset.
	Geoweb3d::IGW3DVectorDataSourceWPtr datasource = sdk_context->get_VectorDriverCollection()->auto_Open("../examples/media/water/hydro.shp", res);
		

	Geoweb3d::IGW3DWindowWPtr window = sdk_context->get_WindowCollection(  )->create_3DWindow("Water and Clouds", GW3D_OVERLAPPED, 10, 10, 1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), 0);
	


    sdk_context->draw( window );// gets the imagery loading while we do the rest

	Geoweb3d::IGW3DCameraWPtr camera1 = window.lock()->get_CameraCollection()->create("Main Camera");

	Geoweb3d::CloudLayerHandle cloudhandle;
	camera1.lock()->get_EnvironmentEffects()->get_EnvironmentLabs()->create_CloudLayer(cloudhandle,Geoweb3d::IGW3DEnvironmentLabs::CUMULUS_CONGESTUS );

	const Geoweb3d::IGW3DPropertyCollection * p =Geoweb3d::get_Properties( cloudhandle );
	printf("cloud properties:\n");
	Geoweb3d::IGW3DStringPtr string_val;
	for (unsigned int i = 0; i < p->count(); ++i) 
	{
		printf("\n    **  %d:  ", i);
		p->get_Property(i).to_string(string_val);
		printf("%s  ==  %s", p->get_DefinitionCollection()->get_AtIndex(i)->property_name, p->get_Property(i).is_null() ? "NULL" : string_val->c_str());
	}
	printf("\n\n");


	FidZoomer fid_zoomer;

	Geoweb3d::IGW3DVectorRepresentationWPtr waterrep;

	//figure out where to zoom
	if(!datasource.expired())
	{
		Geoweb3d::IGW3DVectorLayerCollection *vlayercollection=  datasource.lock()->get_VectorLayerCollection();

		Geoweb3d::IGW3DVectorLayerWPtr tester;
		Geoweb3d::GW3DEnvelope envelope; //start the zoom out by looking over the whole layer
		while(vlayercollection->next( &tester)) //just to see
		{
			envelope.merge( tester.lock()->get_Envelope() );
	    }
		
		double longitude, latitude;
		longitude = (envelope.MinX + envelope.MaxX ) * 0.5;
		latitude  = (envelope.MinY + envelope.MaxY ) * 0.5;
		latitude = latitude - .0005;
		camera1.lock()->get_CameraController()->put_Location( longitude, latitude);

		camera1.lock()->get_CameraController()->put_Rotation(0,7,0);
		camera1.lock()->get_CameraController()->put_Elevation(140, Geoweb3d::IGW3DPositionOrientation::Relative );
		
		if(vlayercollection->count())
		{
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr line = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ColoredLine" );
			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			Geoweb3d::IGW3DVectorRepresentationWPtr rep = line.lock()->get_RepresentationLayerCollection()->create( vlayercollection->get_AtIndex(0) ,params);


			Geoweb3d::IGW3DVectorRepresentationDriverWPtr modeldrv = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "Model" );
			Geoweb3d::IGW3DVectorRepresentationWPtr modelrep = modeldrv.lock()->get_RepresentationLayerCollection()->create( vlayercollection->get_AtIndex(0) ,params);


			Geoweb3d::IGW3DVectorRepresentationDriverWPtr water = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "Water" );
			 waterrep = water.lock()->get_RepresentationLayerCollection()->create( vlayercollection->get_AtIndex(0) ,params);
			
			 vlayercollection->get_AtIndex(0).lock()->Stream(&fid_zoomer );

		}
	}

	//
	MiniEngineCore miniEngine;

	miniEngine.LoadDemoImageryDataset(sdk_context );


	if(miniEngine.CreateElevationRep(sdk_context, "../examples/media/NED 10-meter DC.tif", 6, 0, true, 1.0).expired() )
    {
        printf("CreateElevationRep Error\n");
    }


	int counter = 359 * 2;

	unsigned int current_feature = 0;

	bool once = false;
	while( sdk_context->draw( window )==Geoweb3d::GW3D_sOk )
	{
	   if(!once && !waterrep.expired())
	   {
	   		if(!waterrep.expired())
			{
				auto waterrep_cam_props = waterrep.lock()->get_CameraProperties(camera1);
				if (waterrep_cam_props != nullptr )
				{
					Geoweb3d::IGW3DPropertyCollectionPtr pwater_camera_properties = waterrep_cam_props->create_Clone();

					for (unsigned int i = 0; i < pwater_camera_properties->count(); ++i)
					{
						pwater_camera_properties->put_Property(i, false);
					}
					printf("water properties on this camera:\n");
					//This configures the camera properties by string, but you can also use WaterProperties::CameraProperties in LayerParameters.h
					Geoweb3d::IGW3DStringPtr string_val;
					for (unsigned int i = 0; i < pwater_camera_properties->count(); ++i)
					{
						printf("\n    **  %d:  ", i);
						pwater_camera_properties->get_Property(i).to_string(string_val);
						printf("%s  ==  %s", pwater_camera_properties->get_DefinitionCollection()->get_AtIndex(i)->property_name, pwater_camera_properties->get_Property(i).is_null() ? "NULL" : string_val->c_str());
					}
					printf("\n\n");

					waterrep.lock()->put_CameraProperties(camera1, pwater_camera_properties);
				}

			}
		   once = true;
	   }

	   if(counter == 0)
	   {
		   if(current_feature < fid_zoomer.feature_envelopes.size())
		   {
				Geoweb3d::GW3DEnvelope	envelope( fid_zoomer.feature_envelopes[current_feature] );

				double longitude, latitude;
				longitude = (envelope.MinX + envelope.MaxX ) * 0.5;
				latitude  = (envelope.MinY + envelope.MaxY ) * 0.5;
				latitude = latitude - .0005;
				camera1.lock()->get_CameraController()->put_Location( longitude, latitude);
				++current_feature;
		   }
		   else
		   {
			   current_feature = 0; //wrap around to the start
		   }

			counter = 359 * 2;
	   }
	   
	   camera1.lock()->get_CameraController()->put_Rotation(counter * .5f,7,0);

	   double longitude, latitude;
	   camera1.lock()->get_CameraController()->get_Location( longitude, latitude);
	   camera1.lock()->get_CameraController()->put_Location( longitude, latitude + .00001);
	   

		   --counter;

   }
}

int _tmain(int argc, _TCHAR* argv[])
{
    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if(sdk_context)
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
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
