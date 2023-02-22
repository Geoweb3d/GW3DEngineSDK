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

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"

//The Geoweb3dCore APIs are from the previous C-like API
//and will eventual be fully replaced in the C++ API and deprecated
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#include <iostream>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )


void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();


/**
 *  The purpose of this example application is to create a vector layer in memory with user
 * defined points.
 */

Geoweb3d::IGW3DVectorDataSourceWPtr create_Geoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char* datasourcename )
{
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );

    if( !driver.expired() )
    {
        Geoweb3d::IGW3DVectorDataSourceCollection *datasource_collection = driver.lock()->get_VectorDataSourceCollection();

        return datasource_collection->create( datasourcename );
    }
    else
    {
		std::cout<< "Error creating vector data source..." << std::endl;
    }

    return Geoweb3d::IGW3DVectorDataSourceWPtr();
}

//This function just demonstrates the validity of Geoweb3d SDK pointer comparisons
void ValidateEqualOperatorAndSamePointers( Geoweb3d::IGW3DVectorDataSourceWPtr ds1 )
{
    if( ds1.expired() )
	{
		std::cout<< "Invalid datasource specified, aborting test(s)." << std::endl;
        return;
    }

    Geoweb3d::IGW3DVectorDataSourceCollection* datasource_collection  =
        ds1.lock()->get_Driver().lock()->get_VectorDataSourceCollection();

    Geoweb3d::IGW3DVectorDataSourceWPtr datasource;

    //just to validate its created, loop through the collection and print this out.
    while( datasource_collection->next( &datasource ) )
    {
		std::cout << "Datasources Name: [" << datasource.lock()->get_Name() << "]" << std::endl;

		std::cout<< "\tUsing Driver: [" << ds1.lock()->get_Driver().lock()->get_Name() << "] " << std::endl;


        //fast, the SDK will always try to keep the same objects
        //through its queries from collections, unless stated otherwise
        if( datasource.lock() == ds1.lock() )
			std::cout << "\tDatasource matched!!!" << std::endl;
        else
			std::cout << "\tDatasource mismatched ( reference different data )!!!" << std::endl;

		//Validate we are using the same drivers.
        if( ds1.lock()->get_Driver().lock() != datasource.lock()->get_Driver().lock() )
			std::cout<< "Test failed due to datasource mismatch." << std::endl;

        //Validate both datasource collections these belong to are the same.
        if( datasource_collection != datasource.lock()->get_Driver().lock()->get_VectorDataSourceCollection() )
			std::cout<< "Test failed due to datasource's collections mismatch" << std::endl;
    }
}

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

	// Request the Geoweb3d_Datasource vector driver, which is an efficient driver for storing and manipulating data in memory
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );

    if( driver.expired() )
    {
		std::cout << "Could not find the requested driver! For a list of supported drivers, run the System Utilities example" << std::endl;
        return;
    }

	// Check whether the driver supports datasource creation
	if( !driver.lock()->get_IsCapable( isCreateDataSourceSupported ) )
	{
		std::cout << "This driver is not capable of creating new datasources!" << std::endl;
		return;
	}

	// Create a named datasource with the requested vector driver
    Geoweb3d::IGW3DVectorDataSourceWPtr created_datasource = create_Geoweb3dDataSource( sdk_context, "Dynamic_Features.geoweb3d" );

	// Not necessary, but demonstrates the comparison of API-provided pointers
    ValidateEqualOperatorAndSamePointers( created_datasource );

	// Specify the 2D point geometry type
    Geoweb3d::GeometryType geotype = Geoweb3d::gtPOINT;

	// Create a new field definition for the new layers
    Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());

	// Add a new property to the field definition, to store information about each object/feature in the layer
    field_definition->add_Property( "Color", Geoweb3d::PROPERTY_STR, "Description of the Object's Color" );

	// create some layers
    Geoweb3d::IGW3DVectorLayerWPtr Airplanes	= created_datasource.lock()->get_VectorLayerCollection()->create( "Airplanes", geotype, field_definition );
    Geoweb3d::IGW3DVectorLayerWPtr Cars			= created_datasource.lock()->get_VectorLayerCollection()->create( "Cars",      geotype, field_definition );
    Geoweb3d::IGW3DVectorLayerWPtr BullDozers	= created_datasource.lock()->get_VectorLayerCollection()->create( "BullDozers",geotype, field_definition );
    Geoweb3d::IGW3DVectorLayerWPtr Boats		= created_datasource.lock()->get_VectorLayerCollection()->create( "Boats",     geotype, field_definition );
    Geoweb3d::IGW3DVectorLayerWPtr Trucks		= created_datasource.lock()->get_VectorLayerCollection()->create( "Trucks",    geotype, field_definition );
    Geoweb3d::IGW3DVectorLayerWPtr Birds		= created_datasource.lock()->get_VectorLayerCollection()->create( "Birds",     geotype, field_definition );

	// Demonstrate that the created layers are now members of the datasource
   
	Geoweb3d::IGW3DVectorLayerCollection* vlayercollection = created_datasource.lock()->get_VectorLayerCollection();

    Geoweb3d::IGW3DVectorLayerWPtr tester;

    while( vlayercollection->next( &tester) )
   		std::cout<< "Vector Layer: [" << tester.lock()->get_Name() << "]" << std::endl;
   
	// create some point features in one of the layers

    Geoweb3d::IGW3DPropertyCollectionPtr field_values (field_definition->create_PropertyCollection());

    Geoweb3d::GW3DPoint p1( 38.89000, -77.02000, 0);

	Geoweb3d::GW3DResult result;

    field_values->put_Property( 0, "yellow" );
	std::cout << "FeatureID created: " <<  Birds.lock()->create_Feature( field_values, &p1, result ) << std::endl;

	// There is some overhead involved in creating geometry objects, so reuse objects when possible

	p1.put_Y( 38.89100 );
	p1.put_X(-77.02000 );

    field_values->put_Property( 0, "red" );
	std::cout << "FeatureID created: " << Birds.lock()->create_Feature( field_values, &p1, result ) << std::endl;

	p1.put_Y( 38.89200 );
	p1.put_X(-77.02000 );

    field_values->put_Property( 0, "green" );
	std::cout << "FeatureID created: " << Birds.lock()->create_Feature( field_values, &p1, result ) << std::endl;

	system("pause");
}

int _tmain( int argc, _TCHAR* argv[] )
{

	// Set up information handling, so that SDK messages, warnings and errors can be displayed

    SetInformationHandling();

	// Create the SDK context

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
		// configure the SDK prior to initializing
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();

		// Initialize the SDK
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
			std::cout << "\nError creating SDK context! Press any key to continue...\n" << std::endl;
            getchar();
        }
    }
    else
    {
        return -1;
    }

	// When the SDK context gets destroyed, all pointers owned by the SDK will be destroyed.
    sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

    return 0;
}


/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg )
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
