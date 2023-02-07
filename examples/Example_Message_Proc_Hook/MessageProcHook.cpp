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

// The purpose of this demonstration is to ensure a user can control display device (mostly monitors/LCD
// screens), as well as control situation where a user has more than one monitor.

#include "stdafx.h"
#include <windows.h>

#include "GeoWeb3dCore/SystemExports.h"

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

#include "engine/GW3DVector.h"

#include "common/IGW3DSpatialReference.h"

#include <stdio.h>
#include <stdarg.h>

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

class MyCallbackImplimentation : public Geoweb3d::IGW3DWindowCallback
{
public:

	virtual void OnCreate( ) {};
	virtual void OnDrawBegin( ) {}; 
	virtual void OnDrawEnd( ) {};	

    void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
    {
		// Callback with an OpenGL context, for drawing in 2D
    }

	int ProcessLowLevelEvent(OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam )
    {
        // We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
        if (message == WM_CLOSE)
        {
            printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
            return 0;
        }
        return 1;
    }

	void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
	{
		switch(win_event.Type)
        {
			case win_event.KeyPressed:
			{
				switch(	win_event.Key.code )
				{
				case Geoweb3d::Key::Space:
					printf ("space key was pressed\n");
					break;
				}
			}
		}
	}
};




void SimpleAndQuick_CameraBeforeWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );

    MyCallbackImplimentation *winproc = new MyCallbackImplimentation;

    Geoweb3d::IGW3DWindowWPtr window1 = wcol->create_3DWindow("test", GW3D_OVERLAPPED, 10, 10, 800,600, 0, Geoweb3d::IGW3DStereoDriverPtr(),winproc);

    if(!window1.lock())
    {
        printf("Error Creating a window!\n");

        return ;
    }

    Geoweb3d::IGW3DCameraCollection *cameras = window1.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera1=	cameras->create( "test" );

    printf("Camera Name Readback: %s\n", camera1.lock()->get_Name());

    double lat = 38.8951;
	double lon = 77.0367;

    if(!camera1.expired())
    {
        camera1.lock()->get_CameraController()->put_Location( lon,lat );
    }

    Geoweb3d::IGW3DCameraController* position_controller = camera1.lock()->get_CameraController();

    while( sdk_context->draw( window1 )==Geoweb3d::GW3D_sOk )
    {

        if(!camera1.expired()) //this is just showing a best practice.  If somehow the window was deleted that this camera belongs to!
        {

            position_controller->put_Location( lon,lat );

            //again, this is pointing out the readback and setting capabilities
            float heading, pitch, roll;
            position_controller->get_Rotation( heading, pitch, roll );
            position_controller->put_Rotation( heading + 2.f, pitch + 1.1f, roll + 1.f );
        }

    }


    if(winproc)
        delete winproc;
}

void RunApplication(  Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

    SimpleAndQuick_CameraBeforeWindowCreation( sdk_context );

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

    //example to show if you want to control its when the sdk context gets destroyed.
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