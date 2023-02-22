/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2014
 * Author: Vincent A. Autieri, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
**/

#include "stdafx.h"

#include "GeoWeb3dCore/SystemExports.h" //needed for debug/log information

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "engine/IGW3DWindow.h"
#include "engine/IGW3DWindowCollection.h"
#include "engine/IGW3DWindowCallback.h"
#include "engine/IGW3DCamera.h"
#include "engine/IGW3DDateTime.h"
#include "engine/IGW3DCameraCollection.h"
#include "common/IGW3DString.h"
#include "engine/IGW3DCameraStereoSettings.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );

class MiniEngine : public Geoweb3d::IGW3DWindowCallback
{
public:
	MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) : sdk_context_(sdk_context)
	{
	
	}	
	virtual ~MiniEngine()
	{

	}

	bool LoadConfiguration(const char *example_name )
	{		    
        window_ = CreateWindowCreation( sdk_context_, example_name, this );

		if(window_.expired())return false;


		////bottom left corner
		const float spacing = .001f;

		//NOTE! currently only 1 camera on a window is working with stereo.  for passive stereo, this will
		//change eventually.  This is why stereo is commented out.
		camera1_ = CreateCamera(window_,"1 Camera");	
		camera1_.lock()->put_NormalizeWindowCoordinates(0.f,			0.5f - spacing, 0.5f + spacing,	1.0f);
		//camera1_.lock()->get_CameraStereoSettings()->put_Enable(true);

		//bottom right corner
		camera2_ = CreateCamera(window_,"2 Camera");
		camera2_.lock()->put_NormalizeWindowCoordinates(0.5f + spacing, 1.0f,			0.5f + spacing,	1.0f);
		//camera2_.lock()->get_CameraStereoSettings()->put_Enable(true);

		////upper left corner
		camera3_ = CreateCamera(window_,"3 Camera");
		camera3_.lock()->put_NormalizeWindowCoordinates(0.0f,			0.5f - spacing, 0.0f,			0.5f - spacing);
		//camera3_.lock()->get_CameraStereoSettings()->put_Enable(true);

		//upper right corner
		camera4_ = CreateCamera(window_,"4 Camera");
		camera4_.lock()->put_NormalizeWindowCoordinates(0.5f + spacing, 1.0f,			0.0f,			0.5f - spacing);
		//camera4_.lock()->get_CameraStereoSettings()->put_Enable(true);

		return true;
	}

	bool LoadData( const char *rootpath )
	{
		//load data (optional)
		return true;
	}


	bool Update()
	{
		  if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
		  {
			  return true;
		  }

		 //return that we did not draw. (app closing?)
		  return false;
	}

	virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
	{

	}

	virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
	{

	}

	virtual void OnCreate( ) {};
	virtual void OnDrawBegin( ) {}; 
	virtual void OnDrawEnd( ) {};	

private:
	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
	
	Geoweb3d::IGW3DWindowWPtr window_;
	
	Geoweb3d::IGW3DCameraWPtr camera1_;
	Geoweb3d::IGW3DCameraWPtr camera2_;
	Geoweb3d::IGW3DCameraWPtr camera3_;
	Geoweb3d::IGW3DCameraWPtr camera4_;
};
void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

	MiniEngine *pengine = new MiniEngine( sdk_context );

	if(pengine->LoadConfiguration("Example - Multi-Camera/Single Window"))
	{
		////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
		const char * root_path = "C:/dev_new/dcdata";

		if(pengine->LoadData( root_path ))
		{
			
		}
		else
		{
			printf("Could not load data, you probably need to setup the path to your data\n");
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
    //sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();
	
	//system("pause");
	
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
		//go back to useing the date and time of this computer
		//camera.lock()->get_DateTime()->put_isUseComputerDate(true);
		//camera.lock()->get_DateTime()->put_isUseComputerTime(true);
    }

    return camera;
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
