// Editable_Curtain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#
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
#include "engine/IGW3DMediaCenter.h"
#include "engine/IGW3DImage.h"

#include "GeoWeb3dCore/SystemExports.h"

#include "../Common/MiniEngineCore.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

 double CalculateHeading( const double &long1, const double &lat1, const double &long2, const double &lat2)
{
    double a = lat1 * M_PI / 180.;
    double b = long1 * M_PI / 180.;
    double c = lat2 * M_PI / 180.;
    double d = long2 * M_PI / 180.;

    if (cos(c) * sin(d - b) == 0)
        if (c > a)
            return 0.0;
        else
            return 180.0;
    else
    {
        double angle = atan2(cos(c) *sin(d - b),sin(c) * cos(a) - sin(a) * cos(c) *cos(d - b));
        return (angle * 180.0 /M_PI + 360.0);

    }
}

void RunApplication(  Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	Geoweb3d::IGW3DWindowWPtr window = 
		sdk_context->get_WindowCollection(  )->create_3DWindow("Poster Capture", GW3D_OVERLAPPED, 
		10, 10, 1280,720, 0, Geoweb3d::IGW3DStereoDriverPtr(), 0);


    Geoweb3d::GW3DEnvelope env;
    MiniEngineCore mini_engine;
    mini_engine.LoadDemoImageryDataset( sdk_context, env );

    sdk_context->draw( window );// gets the imagery loading while we do the rest
  
	double longitude, latitude;
    longitude = ( env.MinX + env.MaxX ) * 0.5;
    latitude  = ( env.MinY + env.MaxY ) * 0.5;
	latitude = latitude - .0005;
	
	Geoweb3d::IGW3DCameraWPtr camera1 = window.lock()->get_CameraCollection()->create("Main Camera");

	camera1.lock()->get_DateTime()->put_isUseComputerTime(false);
	camera1.lock()->get_DateTime()->put_Time(12,0);
	camera1.lock()->get_DateTime()->put_isFollowCameraLocationForReference(true);
	camera1.lock()->get_CameraController()->put_Elevation(1000, Geoweb3d::IGW3DPositionOrientation::Relative );

	float counter = 0;
	double heading = 0;
	int screenshot_counter = 0;
   while( sdk_context->draw( window )==Geoweb3d::GW3D_sOk )
   {

	   if(screenshot_counter > (60 * 10)) //about 10 seconds if 60hz
	   {
		   screenshot_counter = 0;
		   printf("SCREEN CAPTURE START->");
            const int width = 4000; //try a 4kx4k snapshot 
            const int height= 3000;

            Geoweb3d::IGW3DMediaCenter *medcenter = sdk_context->get_MediaCenter();
            //save as a lossless format as we are interested in a poster.
			Geoweb3d::IGW3DMediaCenterTaskPtr ctask = medcenter->create_Image(window,width,height,"C:/temp/test_deferred.bmp");
            //note, when the task is completed, I want to have to go to disk,  Note that
            //this may get written now or 10 minutes from now.. if you really want to be
            //sure the task is completed and the file is written, you need to call wait_ForCompletion.

            //note, we can force the event and or/wait
            //if(ctask->get_Status() == Geoweb3d::GW3D_sPending)
            {
                medcenter->wait_ForCompletion(ctask, -1 ); //this is always required, async is not yet supported

				printf("COMPLETED\n");

                //ctask->get_Image().lock()->put_FlipVertical();
                //ctask->get_Image().lock()->put_ToDisk("C:/temp/vertical_flip.png" );

                ////be careful as this might take a while to scale
                //ctask->get_Image().lock()->put_ScaleImage(6000,6000);
                //ctask->get_Image().lock()->put_ToDisk("C:/temp/scale_up.png" );

                //ctask->get_Image().lock()->put_ScaleImage(32,32);
                //ctask->get_Image().lock()->put_ToDisk("C:/temp/scale_down.png" );
	       }
	   }

	   screenshot_counter++;
	   double camlongitude,camlatitude;

	   camera1.lock()->get_CameraController()->put_Location(longitude + (.01 * cos( counter )),latitude + (.01 *sin( counter )));

	   camera1.lock()->get_CameraController()->get_Location( camlongitude,camlatitude);
	   heading = CalculateHeading(camlongitude, camlatitude, longitude,latitude);


	   camera1.lock()->get_CameraController()->put_Rotation( static_cast<float>(heading), 22.0f, 0.0f );

	   counter+=.01f;
   }
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


