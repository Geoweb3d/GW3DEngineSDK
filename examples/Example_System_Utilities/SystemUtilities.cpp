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

/**
 * The purpose of this example is to go through the basic initialization functionality of the SDK.
 * The concepts used in this example will be further used in other examples.
 *
 * In this example:
 *		Memory statistics
 *		Threading statistics
 *		Initialization and error information
 *		Using stop watches
 *		Administrative privledges
 *		Licensing Information
 */

#include "stdafx.h"
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DRaster.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

#include "engine/IGW3DStereoDriverCollection.h"
#include "engine/IGW3DStereoDriver.h"

//interfaces that will be going away.
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#include <iostream>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )



/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function( const char* msg ) 
{
	std::cout<< "Fatal Info: " << msg;
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function( const char* msg ) 
{
	std::cout<< "General Info: " << msg;
}

/*! Information handling is not required */
void SetInformationHandling()
{
	/*! Tell the engine about our error function handling */
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
	Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}

/*! Get operating system memory information */
void TestMemory()
{
	Geoweb3d::OS_Helpers::GW3DSystemMemoryData memory_details = Geoweb3d::OS_Helpers::get_SystemMemoryData();
	Geoweb3d::OS_Helpers::get_SystemMemoryData();
	std::cout << "Total Physical Memory: "	<< memory_details.physical_total << std::endl;
	std::cout << "Free Physical Memory: "	<< memory_details.physical_total - memory_details .physical_used<< std::endl	<< std::endl;;
	std::cout << "Total Virtual Memory: "	<< memory_details.virtual_total << std::endl;
	std::cout << "Free Virtual Memory: "	<< memory_details.virtual_total - memory_details.virtual_used << std::endl << std::endl;

	std::cout << "Physical Memory Used by this proccessed: " << memory_details.physical_usedByProcess << std::endl;
	std::cout << "Virtual Memory Used by this proccessed: " << memory_details.virtual_total - memory_details.virtual_usedByProcess << std::endl;


}


void DumpStatistics()
{
	std::cout << "\n** DumpStatistics ** "<< std::endl;

	/*! Number of general tasks left to execute */
	std::cout << "GetGeneralTasksPending(): "	<< Geoweb3d::SDK_Statistics::GetGeneralTasksPending() << std::endl;

	/*! Number of tasks related to imagery to execute */
//	std::cout << "GetImageryTasksPending(): "	<< Geoweb3d::SDK_Statistics::GetImageryTasksPending() << std::endl;

	/*! Number of tasks related to paging 3D models */
	std::cout << "Get3DModelTasksPending(): "	<< Geoweb3d::SDK_Statistics::Get3DModelTasksPending() << std::endl;
	
	/*! Number of frames executed */
	std::cout << "GetMasterFrameCount(): "		<< Geoweb3d::SDK_Statistics::GetMasterFrameCount() << std::endl << std::endl;
}


void DumpLicenseDetails()
{
	std::cout << "\n** DumpLicenseDetails ** "<< std::endl;

	/*! Number of general tasks left to execute */
	const Geoweb3d::IGW3DPropertyCollection* p = Geoweb3d::SDK_License::GetLicenseProperty();

	const Geoweb3d::IGW3DDefinitionCollection* def = p->get_DefinitionCollection();
	Geoweb3d::IGW3DStringPtr string_val;
	for (unsigned int i = 0; i < p->count(); ++i)
	{
		p->get_Property(i).to_string(string_val);
		std::cout << def->get_AtIndex(i)->property_name << ": " << string_val->c_str() << std::endl;
	}

	if( Geoweb3d::SDK_License::isESRICapable())
		std::cout << " Is ESRI Capable" << std::endl;
 
}

void DisplaySupportedRasterDataTypes( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	Geoweb3d::IGW3DRasterDriverCollection* raster_drivers = sdk_context->get_RasterDriverCollection();
	Geoweb3d::IGW3DRasterDriverWPtr driver;

	std::cout << "SUPPORTED RASTER DRIVERS: " << std::endl;
	std::cout << "------------------------- " << std::endl << std::endl;
	int i=0;
	while(raster_drivers->next( &driver ))
	{
		std::cout << driver.lock()->get_Name() 
			<< " [" << i << "] : " 
			<< driver.lock()->get_Description()->c_str();
		if (!driver.lock()->get_Extension()->empty())
		{
			std::cout << " (*." << driver.lock()->get_Extension()->c_str() << ")" << std::endl;
		}
		else
		{
			std::cout << std::endl;
		}
		++i;
	}
}

void DisplaySupportedVectorDataTypes( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	Geoweb3d::IGW3DVectorDriverCollection *vector_drivers = sdk_context->get_VectorDriverCollection();
	Geoweb3d::IGW3DVectorDriverWPtr driver;
	std::cout << std::endl << "SUPPORTED VECTOR DRIVERS: " << std::endl;
	std::cout <<		   "------------------------- " << std::endl << std::endl;
	int i = 0;
	while(vector_drivers->next( &driver ))
	{
		std::cout << driver.lock()->get_Name() << " [" << i << "]" << std::endl;
		++i;
	}
}

void DisplaySupportedVectorRepresentations( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	int num_of_reps =	sdk_context->get_VectorRepresentationDriverCollection()->count();
	std::cout << std::endl << "SUPPORTED VECTOR REPRESENTATION DRIVERS: " << std::endl;
	std::cout <<		   "--------------------------------- " << std::endl << std::endl;
	
	for(int i = 0; i < num_of_reps; ++i)
	{
		const Geoweb3d::IGW3DVectorRepresentationDriverWPtr driverhandle = sdk_context->get_VectorRepresentationDriverCollection()->get_AtIndex(i);
		std::cout << driverhandle.lock()->get_Name() << " [" << i << "]" << std::endl;
		const Geoweb3d::IGW3DPropertyCollection* default_properties = driverhandle.lock()->get_PropertyCollection();
		const Geoweb3d::IGW3DDefinitionCollection* property_definitions = default_properties->get_DefinitionCollection();
		for (unsigned int j = 0; j < property_definitions->count(); ++j)
		{
			std::cout << "-- " << property_definitions->get_AtIndex(j)->property_name << std::endl;  
		}
	}
}

void DisplaySupportedRasterRepresentations( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	int num_of_reps =	sdk_context->get_RasterLayerRepresentationDriverCollection()->count();
	std::cout << std::endl << "SUPPORTED RASTER REPRESENTATION DRIVERS: " << std::endl;
	std::cout <<		   "--------------------------------- " << std::endl << std::endl;
	
	for(int i = 0; i < num_of_reps; ++i)
	{
		const Geoweb3d::IGW3DRasterRepresentationDriverWPtr driverhandle = sdk_context->get_RasterLayerRepresentationDriverCollection()->get_AtIndex(i);
		std::cout << driverhandle.lock()->get_Name() << " [" << i << "]" << std::endl;
		const Geoweb3d::IGW3DPropertyCollection* default_properties = driverhandle.lock()->get_PropertyCollection();
		const Geoweb3d::IGW3DDefinitionCollection* property_definitions = default_properties->get_DefinitionCollection();
		for (unsigned int j = 0; j < property_definitions->count(); ++j)
		{
			std::cout << "-- " << property_definitions->get_AtIndex(j)->property_name << std::endl;   
		}
	}
}

void DisplaySupportedStereoTypes( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
	int num_drivers = sdk_context->get_StereoDriverCollection()->count();
	std::cout << std::endl << "SUPPORTED STEREO DRIVERS: " << std::endl;
	std::cout <<		   "--------------------------------- " << std::endl << std::endl;
	
	for(int i = 0; i < num_drivers; ++i)
	{
		const Geoweb3d::IGW3DStereoDriverWPtr driver = sdk_context->get_StereoDriverCollection()->get_AtIndex(i);
		std::cout << driver.unsafe_get()->get_Name() << " [" << i << "]" << std::endl;
		const Geoweb3d::IGW3DPropertyCollection* default_properties = driver.lock()->get_PropertyCollection();
		const Geoweb3d::IGW3DDefinitionCollection* property_definitions = default_properties->get_DefinitionCollection();
		for (unsigned int j = 0; j < property_definitions->count(); ++j)
		{
			std::cout << "--" << property_definitions->get_AtIndex(j)->property_name << std::endl;  
		}
	}
}

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

	Geoweb3d::SDK_Timers::StopWatchHandle timerhandle = 0;

	/*! Stop watches time CPU functionality */
	if( Geoweb3d::Succeeded( Geoweb3d::SDK_Timers::CreateStopWatchTimer( timerhandle ) ) )
	{
		Geoweb3d::SDK_Timers::StartStopWatchTimer( timerhandle );

		TestMemory();

		std::cout << "Memory profile time was: " << Geoweb3d::SDK_Timers::GetTime( timerhandle ) << std::endl;

		Geoweb3d::SDK_Timers::ResetStopWatchTimer( timerhandle );

		Geoweb3d::SDK_Timers::StartStopWatchTimer( timerhandle );

		DumpStatistics();

		std::cout << "Statistics time:" << Geoweb3d::SDK_Timers::GetTime( timerhandle ) << std::endl << std::endl; //should be small!!! before the 3 second sleep

		Geoweb3d::SDK_Timers::DestroyStopWatchTimer( timerhandle );

		DumpLicenseDetails();

		std::cout<< "\n\nPress enter to continue . . .\n\n"<<std::endl;
		getchar();

		DisplaySupportedVectorDataTypes( sdk_context );

		std::cout<< "\n\nPress enter to continue . . .\n\n"<<std::endl;
		getchar();

		DisplaySupportedRasterDataTypes( sdk_context );

		std::cout<< "\n\nPress enter to continue . . .\n\n"<<std::endl;
		getchar();

		DisplaySupportedVectorRepresentations( sdk_context );

		std::cout<< "\n\nPress enter to continue . . .\n\n"<<std::endl;
		getchar();

		DisplaySupportedRasterRepresentations( sdk_context );

		std::cout<< "\n\nPress enter to continue . . .\n\n"<<std::endl;
		getchar();

		DisplaySupportedStereoTypes( sdk_context );
	}

	std::cout<< "\n\nPress enter to continue . . .\n\n"<<std::endl;

	getchar();
}

int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling();

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
			std::cout<< "\nSDK Context Creation Error! Press enter to continue...\n"<<std::endl;
            getchar();
        }
    }
    else
    {
        return -1;
    }

	return 0;
}

