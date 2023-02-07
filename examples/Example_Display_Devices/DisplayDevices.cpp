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
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "GeoWeb3dCore/SystemExports.h"

#include "engine/IGW3DDisplayDevices.h"
#include "engine/IGW3DDisplayDetails.h"
#include "engine/IGW3DDisplayMode.h"
#include "engine/IGW3DDisplayModeCollection.h"

#include <stdio.h>
#include <stdarg.h>

#include <iostream>

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


void my_fatal_function( const char* msg );
void my_info_function( const char* msg );
void SetInformationHandling();


void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr p )
{
	//The hardware display device  interface.
	Geoweb3d::IGW3DDisplayDevices* devices = p->get_DisplayDevices();

	Geoweb3d::IGW3DDisplayModePtr mode;
	Geoweb3d::IGW3DDisplayDetailsPtr details;

	for( int i = 0; i < devices->get_Count(); ++i )
	{
		details = devices->get_Details( static_cast<unsigned>( i ) );

		std::cout<< "\n** Device Name: " << details->get_DeviceName();
		std::cout<< "\n** Device Description: " << details->get_DeviceDescription() << std::endl;

		mode = details->get_CurrentDeviceMode();

		std::cout<< "\t Is currently setup in this mode: \n\n";
		std::cout<< "\t\t Position x: " << mode->get_PositionX() << std::endl;
		std::cout<< "\t\t Position y: " << mode->get_PositionY() << std::endl;
		std::cout<< "\t\t Width: "      << mode->get_Width() << std::endl;
		std::cout<< "\t\t Height: "     << mode->get_Height() << std::endl;
		std::cout<< "\t\t BitsPerPel: " << mode->get_BitsPerPel() << std::endl;
		std::cout<< "\t\t Refresh Rate: " << mode->get_DisplayFrequency() << std::endl;

		std::cout<< "\n** Will now attempt to see if ["<<details->get_DeviceName()<<"] supports Geoweb3d!"<< std::endl;
		system("pause");

		Geoweb3d::IGW3DDisplayModeTestPtr abilities = mode->TestModeForGeoweb3dAbilities( false );

		if( abilities->get_IsGeoweb3dCapable() )
		{
			std::cout<< "*** The Current Mode For ["<<details->get_DeviceName()<<"] Supports  the following: ***\\" << std::endl;
			std::cout<< "\tGeoweb3d: "<< ( abilities->get_IsGeoweb3dCapable() == true ? "true": "false" ) << std::endl;
			std::cout<< "\tAdvanced Stereo: " << ( abilities->get_IsAdvancedStereoCapable() == true ? "true" : "false" ) << std::endl;
			std::cout<< "***\n";
		}
		else
		{
			std::cout<< "The Current Mode For [" << details->get_DeviceName() << "] Does not currently have the ability to run Geoweb3d";
		}
	}

	std::cout<< "\n** Will now attempt to see if [" << details->get_DeviceName( ) << "] supports " << mode->get_Width() << "x" << mode->get_Height() << ", at 120Hz with 32 bits.!" << std::endl;
	system("pause");

	//We are only forcing the smart pointer below to be null since we are going to use it to know if we found the device mode.
	//mode.reset();

	Geoweb3d::IGW3DDisplayModeCollection* modes = details->get_DeviceModeCollection();

	Geoweb3d::IGW3DDisplayModePtr othermodes;

	while( modes->next( &othermodes ))
	{
		if( ( othermodes->get_Width() == mode->get_Width()) && (mode->get_Height() == othermodes->get_Height()) && othermodes->get_DisplayFrequency() == 120 && othermodes->get_BitsPerPel() == 32 )
		{
			std::cout<< "***Found!*** WILL NOW APPLY IT! " << std::endl;
			othermodes->Apply();
			mode = othermodes;
			break;
		}	
	
	}
	if( !mode )
	{
		std::cout<< "Your hardware display does not support the requested mode!" << std::endl;
		system("pause");
	}
	else
	{
		Geoweb3d::IGW3DDisplayModeTestPtr abilities = mode->TestModeForGeoweb3dAbilities( false );

		if( abilities->get_IsGeoweb3dCapable() )
		{
			std::cout<< "*** The Current Mode For [" << details->get_DeviceName() << "] Supports  the following: ***" << std::endl;
			std::cout<< "\tGeoweb3d: "<< ( abilities->get_IsGeoweb3dCapable() == true ? "true" : "false" ) << std::endl;
			std::cout<< "\tAdvanced Stereo: " << ( abilities->get_IsAdvancedStereoCapable() == true ? "true" : "false" ) << std::endl;
			std::cout<< "***" << std::endl;
		}
		else
		{
			std::cout<< "The Current Mode For [" << details->get_DeviceName() << "] Does not currently have the ability to run Geoweb3d";
		}
		system( "pause" );
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_context->create_InitializationConfiguration() , 5, 0 )))
        {
            RunApplication( sdk_context );
        }
        else
        {
			std::cout<< "\nSDK Context Creation Error! Press any key to continue..." << std::endl;
            getchar();
        }
    }
    else
    {
        return -1;
    }

	return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function( const char* msg ) 
{
	std::cout<< "Fatal Info: " << msg;
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function( const char* msg ) 
{
	std::cout<< "General Info: " << msg << std::endl;
}

/*! Information handling is not required */
void SetInformationHandling()
{
	/*! Tell the engine about our error function handling */
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}