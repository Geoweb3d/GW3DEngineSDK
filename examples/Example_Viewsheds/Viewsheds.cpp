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
#include <windows.h>


#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "engine/GW3DCamera.h"

#include "MiniEngine.h"

#include <iostream>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )

void my_fatal_function( const char* msg );
void my_info_function( const char* msg );
void SetInformationHandling();

class MiniEngine;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the application operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

    MiniEngine* pengine = new MiniEngine( sdk_context );

    if( pengine->LoadConfiguration( "Viewsheds" ) )
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        //const char * root_path = "C:/Projects/Washington DC"; //"C:/Path/to/Sample Projects/Washington DC";
		const char* root_path = "../examples/media";

        //  download the Geoweb3d DC sample project here: http://www.geoweb3d.com/download/sample-projects/
        if( !pengine->LoadData( root_path ) )
        {
			std::cout<< "Error loading data - Please verify data path." << std::endl;
        }

        //the engine loaded all its data ok

        while( pengine->Update() )
        {
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

int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling( );
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
		//create the initialization configuration
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
		//sdk_init->put_TerrainMaxLODDepth(20);
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
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

	 //Example showing control over SDK destruction.
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

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
	std::cout<< "Fatal Info: "<< msg;
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
	std::cout<< "General Info: "<< msg;
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