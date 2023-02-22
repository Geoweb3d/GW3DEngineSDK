/**
 * Property of Geoweb3d: GIS SDK
 * --------------------------------
 * Copyright 2008-2020
 * Author: James Pieszala, Geoweb3d
 * Geoweb3d SDK is not free software: you cannot redistribute it and/or modify
 * it under any terms unless we have a written agreement between us.
 * Geoweb3d SDK and example applications are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
**/
#include "stdafx.h"
#include <windows.h>
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/IGW3DDisplayDevices.h"
#include "engine/IGW3DDisplayDetails.h"
#include "engine/IGW3DDisplayMode.h"

//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "engine/GW3DCamera.h"

#include "MiniEngine.h"
#include <iostream>
#include <sstream>

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the application operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

    MiniEngine *pengine = new MiniEngine( sdk_context );

    if(pengine->LoadConfiguration("Raster Layer Example"))
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char * root_path = "../examples/media/";
        if(pengine->PreLoadData( root_path ))
        {
			if (pengine->LoadData(root_path))
			{

			}
			else
			{
				printf("Could not load data\n");
			}
        }
        else
        {
            printf("Could not preload data\n");
        }

        //the engine loaded all its data ok

        while( pengine->Update() )
        {
            //could do other app stuff here
        }
    }



    delete pengine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Main entry-point for this application. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="argc">	The argc. </param>
/// <param name="argv">	[in,out] If non-null, the argv. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

	Geoweb3d::IGW3DDisplayDevices* devices = sdk_context->get_DisplayDevices();
	Geoweb3d::IGW3DDisplayModePtr mode = devices->get_PrimaryDisplay()->get_CurrentDeviceMode();

	std::stringstream ss;
	ss << "\n** Device Name: " << devices->get_PrimaryDisplay()->get_DeviceName() << std::endl;
	ss << "** Device Description: " << devices->get_PrimaryDisplay()->get_DeviceDescription() << std::endl;
	ss << "\t Is currently setup in this mode: \n\n";
	ss << "\t\t Position x: " << mode->get_PositionX() << std::endl;
	ss << "\t\t Position y: " << mode->get_PositionY() << std::endl;
	ss << "\t\t Width: " << mode->get_Width() << std::endl;
	ss << "\t\t Height: " << mode->get_Height() << std::endl;
	ss << "\t\t BitsPerPel: " << mode->get_BitsPerPel() << std::endl;
	ss << "\t\t Refresh Rate: " << mode->get_DisplayFrequency() << std::endl;

	ss << "\n** Will now attempt to see if primary supports Geoweb3d!" << std::endl;

	Geoweb3d::IGW3DDisplayModeTestPtr abilities = mode->TestModeForGeoweb3dAbilities(false);

	bool is_3DRenderingCapable_ = abilities->get_IsGeoweb3dCapable();

	ss << "\tGeoweb3d 3D Map Capable: " << (is_3DRenderingCapable_ ? "yes" : "no") << std::endl;
	ss << "\tAdvanced Stereo: " << (abilities->get_IsAdvancedStereoCapable() ? "true" : "false") << std::endl;
	ss << "\tOpenGL Vendor: " << abilities->get_OpenGL_vendor() << std::endl;
	ss << "\tOpenGL Renderer: " << abilities->get_OpenGL_render() << std::endl;
	ss << "\tOpenGL Version: " << abilities->get_OpenGL_major_version() << "." << abilities->get_OpenGL_minor_version() << std::endl;

	printf("%s\n", ss.str().c_str());

    if(sdk_context)
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();

		Geoweb3d::GW3DResult sdk_result = sdk_context->InitializeLibrary("geoweb3dsdkdemo", sdk_init, 5, 0);
		bool is_sdk_licensed_ = Geoweb3d::Succeeded(sdk_result);

        if( is_sdk_licensed_ )
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

    //system("pause");

    return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My fatal function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_fatal_function(const char *msg )
{
    printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	My information function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="msg">	The message. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_info_function(const char *msg )
{ 
    printf("General Info: %s", msg);
}

/*! Information handling is not required */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets information handling. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}