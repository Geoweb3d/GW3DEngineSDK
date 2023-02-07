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

#include "GeoWeb3dCore/SystemExports.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
//This enables your application to automatically enable
//high performance GPU on Optimus and AMD devices 
#include "core/GW3DGPUConfig.h"

#include "Geoweb3dCore/GeometryExports.h"

#include <stdio.h>
#include <stdarg.h>

#include <queue>
#include <iostream>
#include <algorithm>    // std::for_each

#pragma comment(lib, "GW3DEngineSDK.lib" )
#pragma comment(lib, "GW3DCommon.lib" )


void my_fatal_function( const char* msg );
void my_info_function( const char* msg );
void SetInformationHandling();

//Window camera callback.
class MyCallbackImplementation : public Geoweb3d::IGW3DWindowCallback
{
	public:
		virtual void OnCreate( ) 
		{
			std::cout << "Camera created!" << std::endl;
		};

		virtual void OnDrawBegin( ) {}; 
		virtual void OnDrawEnd( ) {};	  

		virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
		{
			std::cout<< "...Starting 2D Rendering..." << std::endl;
		}
};

void SimpleAndQuick_CameraAfterWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    Geoweb3d::IGW3DWindowCollection* wcol = sdk_context->get_WindowCollection();
    Geoweb3d::IGW3DWindowWPtr window1 = wcol->create_3DWindow( "SimpleAndQuick_CameraAfterWindowCreation", GW3D_OVERLAPPED, 10, 10, 800, 600, 0 );

	if( !window1.lock() )
    {
		std::cout<< "Error Creating a window!" << std::endl;
        return ;
    }

    Geoweb3d::IGW3DCameraCollection* cameras = window1.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera1 = cameras->create( "SimpleAndQuick_CameraAfterWindowCreation camera 1" );

	std::cout<< "Camera Name Readback: " << camera1.lock()->get_Name();

    if( camera1.expired() )
		return;

    float heading, pitch, roll;

    while( sdk_context->draw( window1 ) == Geoweb3d::GW3D_sOk )
    {
        if( !camera1.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller = camera1.lock()->get_CameraController();

            const Geoweb3d::GW3DPoint* loc = position_controller->get_Location();

            position_controller->put_Location( loc->get_X() + .00001, loc->get_Y() + .00001 );

            //again, this is pointing out the readback and setting capabilities
            position_controller->get_Rotation( heading, pitch, roll );
            position_controller->put_Rotation( heading + 2.f, pitch + 1.1f, roll );
        }
    }
}

void SimpleAndQuick_CameraAfterWindowCreationTwoWindow( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    Geoweb3d::IGW3DWindowCollection* wcol = sdk_context->get_WindowCollection();
    Geoweb3d::IGW3DWindowWPtr window1 = wcol->create_3DWindow( "SimpleAndQuick_CameraAfterWindowCreationTwoWindow Window 1", GW3D_OVERLAPPED, 10, 10, 800,600, 0 );

    if( !window1.lock())
    {
		std::cout<< "Error Creating a window!" << std::endl;

        return;
    }

    Geoweb3d::IGW3DCameraCollection* cameras = window1.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera1 = cameras->create( "SimpleAndQuick_CameraAfterWindowCreationTwoWindow camera 1" );

	std::cout<< "Camera w1 Name Readback: " << camera1.lock()->get_Name() << std::endl;

    Geoweb3d::IGW3DWindowWPtr window2 = wcol->create_3DWindow( "SimpleAndQuick_CameraAfterWindowCreationTwoWindow Window 2", GW3D_OVERLAPPED, 810, 10, 800,600, 0 );
    Geoweb3d::IGW3DCameraCollection* cameras2 = window2.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera2 =	cameras2->create( "SimpleAndQuick_CameraAfterWindowCreationTwoWindow camera 2" );

	std::cout << "Camera w2 Name Readback: " << camera2.lock()->get_Name() << std::endl;

    camera2.lock()->get_DateTime()->put_isUseComputerDate(false);
    camera2.lock()->get_DateTime()->put_isUseComputerTime(false);
    camera2.lock()->get_DateTime()->put_Time( 12,0 );
    
	float heading, pitch, roll;

    while( sdk_context->draw( window1 ) == Geoweb3d::GW3D_sOk && sdk_context->draw( window2 ) == Geoweb3d::GW3D_sOk )
    {
        if( !camera1.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller = camera1.lock()->get_CameraController();

            const Geoweb3d::GW3DPoint* loc = position_controller->get_Location( );
            position_controller->put_Location( loc->get_X() + .00001, loc->get_Y() + .00001 );

            //again, this is pointing out the readback and setting capabilities
            position_controller->get_Rotation( heading, pitch, roll );
            position_controller->put_Rotation( heading + 2.f, pitch + 1.1f, roll );
        }

        if( !camera2.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller = camera2.lock()->get_CameraController();

            const Geoweb3d::GW3DPoint* loc = position_controller->get_Location( );

            position_controller->put_Location( loc->get_X() + .00001, loc->get_Y() + .00001 );

            //again, this is pointing out the readback and setting capabilities
            position_controller->get_Rotation( heading, pitch, roll );
            position_controller->put_Rotation( heading + 2.f, pitch + 1.1f, roll );
        }
    }
}

void SimpleAndQuick_CameraAfterWindowCreationTwoWindow_offset( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );
    Geoweb3d::IGW3DWindowWPtr window1 = wcol->create_3DWindow( "test", GW3D_OVERLAPPED, 10, 10, 800,600, 0 );

    if( !window1.lock() )
    {
		std::cout<< "Error Creating a window!" << std::endl;
        return;
    }

    Geoweb3d::IGW3DCameraCollection* cameras = window1.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera1=	cameras->create( "SimpleAndQuick_CameraAfterWindowCreation window 1" );

	std::cout<< "Camera w1 Name Readback: " << camera1.lock()->get_Name() << std::endl;

    Geoweb3d::IGW3DWindowWPtr window2 = wcol->create_3DWindow( "test2", GW3D_OVERLAPPED, 810, 10, 800,600, 0 );
    Geoweb3d::IGW3DCameraCollection* cameras2 = window2.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera2 =	cameras2->create( "SimpleAndQuick_CameraAfterWindowCreation window 2" );

	std::cout << "Camera w2 Name Readback: " << camera2.lock()->get_Name() << std::endl;

    camera2.lock()->get_DateTime()->put_isUseComputerDate( false );
    camera2.lock()->get_DateTime()->put_isUseComputerTime( false );
    camera2.lock()->get_DateTime()->put_Time( 12, 0 );
    
	float heading, pitch, roll;

	while( sdk_context->draw( window1 ) == Geoweb3d::GW3D_sOk && sdk_context->draw( window2 )== Geoweb3d::GW3D_sOk )
    {
        if( !camera1.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller = camera1.lock()->get_CameraController();

            const Geoweb3d::GW3DPoint* loc = position_controller->get_Location( );

            position_controller->put_Location( loc->get_X() + .00001, loc->get_Y() + .00001 );

            //again, this is pointing out the readback and setting capabilities
            position_controller->get_Rotation( heading, pitch, roll );
            position_controller->put_Rotation( heading + 2.f, pitch + 1.1f, roll );
        }

        if( !camera2.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
		{
            Geoweb3d::IGW3DCameraController* position_controller = camera2.lock()->get_CameraController();

            const Geoweb3d::GW3DPoint* loc = position_controller->get_Location( );
            position_controller->put_Location( loc->get_X() - .00001, loc->get_Y() - .00001 );

            //again, this is pointing out the readback and setting capabilities
            position_controller->get_Rotation( heading, pitch, roll );
            position_controller->put_Rotation( heading - 2.f, pitch - 1.1f, roll - 1.f );
        }
   }
}

void SimpleAndQuick_CameraSideBySide( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_ptr )
{
    // For non-overlapping cameras, insertion order does not matter. The cameras will be 
	// be sorted alpha-numerically.
    // Ex: A Camera with the name 1CAMERA will get rendered before the name 2Camera.

    Geoweb3d::IGW3DWindowCollection* wcol = sdk_ptr->get_WindowCollection();

	OSWinHandle parentWindow = 0;

	Geoweb3d::IGW3DWindowWPtr window1 = wcol->create_3DWindow( "SimpleAndQuick_CameraSideBySide", GW3D_OVERLAPPED, 10, 10, 800,600, parentWindow );

    if( !window1.lock() )
    {
		std::cout<< "Error Creating a window!" << std::endl;
        return;
    }

	Geoweb3d::IGW3DCameraCollection* cameras = window1.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera1=	cameras->create( "1Camera!" );
    camera1.lock()->put_NormalizeWindowCoordinates( 0.0f, 0.5f, 0.0f, 1.0f );

	std::cout<< "1Camera Name Readback: " << camera1.lock()->get_Name() << std::endl;

    ////Create a small 'rear view mirror' camera.

    /////////////////////////////////////////////////////////////////////////
    Geoweb3d::IGW3DCameraWPtr camera2=	cameras->create( "2Camera!" );

	camera2.lock()->put_NormalizeWindowCoordinates( 0.5f, 1.0f, 0.0f, 1.0f );
    camera2.lock()->put_Enabled( true );

	std::cout<< "2Camera Name Readback: " << camera2.lock()->get_Name() << std::endl;

	////////////////////////////////////////////////////////////////////////

    float heading, pitch, roll;

    while( sdk_ptr->draw( window1 ) == Geoweb3d::GW3D_sOk )
    {
        const Geoweb3d::GW3DPoint* loc1 = 0;

        if( !camera1.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller1 = camera1.lock()->get_CameraController();

            loc1 = position_controller1->get_Location( );

            position_controller1->put_Location( loc1->get_X() + .00001, loc1->get_Y() + .00001 );

            //again, this is pointing out the readback and setting capabilities

            position_controller1->get_Rotation( heading, pitch, roll );
            position_controller1->put_Rotation( heading = heading + 2.f, pitch = pitch + 1.1f, roll = roll + 1.f );
        }

        if( !camera2.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller2 = camera2.lock()->get_CameraController();

            if( !loc1 )
                loc1 = position_controller2->get_Location( );

            position_controller2->put_Location( loc1->get_X() + .00001, loc1->get_Y() + .00001 );
            position_controller2->put_Rotation( -heading, -pitch, roll );
        }
    }

    //loop through cameras test...
    Geoweb3d::IGW3DCameraWPtr test;

	std::cout << "...Beginning Camera Iteration Test..." << std::endl;

    while( cameras->next( &test ) )
    {
		std::cout << "Camera name: " << test.lock()->get_Name() << std::endl;
    }

	std::cout << "...Beginning Camera Removal Test..." << std::endl;

    Geoweb3d::IGW3DCameraCollection::close( camera1 ); //remove the first camera...

    while( cameras->next( &test ) )
    {
		std::cout << "Iterator Camera Name Readback: " << test.lock()->get_Name() << std::endl;
    }
}

void SimpleAndQuick_CameraWithinCamera( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_ptr )
{
    // For non-overlapping cameras, insertion order does not matter. The cameras will be 
	// be sorted alpha-numerically.
    // Ex: A Camera with the name 1CAMERA will get rendered before the name 2Camera

    Geoweb3d::IGW3DWindowCollection* wcol = sdk_ptr->get_WindowCollection(  );
    Geoweb3d::IGW3DWindowWPtr window1 = wcol->create_3DWindow( "SimpleAndQuick_CameraWithinCamera", GW3D_OVERLAPPED, 10, 10, 800,600, 0 );

    if( !window1.lock() )
    {
		std::cout << "Error Creating a window!" << std::endl;
        return ;
    }

    Geoweb3d::IGW3DCameraCollection* cameras = window1.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera1=	cameras->create( "1Camera!" );
	std::cout <<"1Camera Name Readback: " << camera1.lock()->get_Name() << std::endl;

    ////Create a small 'rear view mirror' camera.

    /////////////////////////////////////////////////////////////////////////
    Geoweb3d::IGW3DCameraWPtr camera2=	cameras->create( "2Camera!" );
    camera2.lock()->put_NormalizeWindowCoordinates( 0.25f, 0.75f, 0.0f, 0.25f );
    camera2.lock()->put_Enabled( true );

	std::cout << "2Camera Name Readback: " << camera2.lock()->get_Name() << std::endl;

	////////////////////////////////////////////////////////////////////////

    float heading, pitch, roll;

    while( sdk_ptr->draw( window1 ) == Geoweb3d::GW3D_sOk )
    {
        const Geoweb3d::GW3DPoint* loc1 = 0;

        if( !camera1.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
		{
            Geoweb3d::IGW3DCameraController* position_controller1 = camera1.lock()->get_CameraController();

            loc1 = position_controller1->get_Location( );

            position_controller1->put_Location( loc1->get_X() + .00001, loc1->get_Y() + .00001 );

            //again, this is pointing out the readback and setting capabilities
            position_controller1->get_Rotation( heading, pitch, roll );
            position_controller1->put_Rotation( heading = heading + 2.f, pitch = pitch + 1.1f, roll = roll + 1.f );
        }

        if( !camera2.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller2 = camera2.lock()->get_CameraController();

            if( !loc1 )
                loc1 = position_controller2->get_Location( );

            position_controller2->put_Location( loc1->get_X(), loc1->get_Y()  );
            position_controller2->put_Rotation( -heading, -pitch, roll );
        }
    }

    //loop through cameras test...
    Geoweb3d::IGW3DCameraWPtr test;

	std::cout << "...Beginning Camera Iteration Test..." << std::endl;

	while( cameras->next( &test ) )
    {
		std::cout << " Camera Name: " << test.lock()->get_Name() << std::endl;
    }

	std::cout << "...Beginning Camera Removal Test..." << std::endl;

    Geoweb3d::IGW3DCameraCollection::close( camera1 ); //remove the first camera...

    while( cameras->next( &test ) )
    {
		std::cout << "Camera Name: " << test.lock()->get_Name() << std::endl;
    }
}

void SimpleAndQuick_CameraWithinCamera_WithCallback( Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_ptr )
{
    MyCallbackImplementation* pcallbackclass = new MyCallbackImplementation();

	// If you have a picture in picture setup, the NAME OF THE CAMERA IS IMPORTANT!!!
	// Cameras are prioritized in alphanumeric order and not by order of insertion.
    // I.e. a Camera with the name 1CAMERA will get rendered before the name 2Camera, so it
    // does not matter what order you add them in.
	OSWinHandle parentWindow = 0;

    Geoweb3d::IGW3DWindowCollection* wcol = sdk_ptr->get_WindowCollection(  );
    Geoweb3d::IGW3DWindowWPtr window1 = wcol->create_3DWindow( "SimpleAndQuick_CameraWithinCamera_WithCallback", GW3D_OVERLAPPED, 10, 10, 800,600, parentWindow, Geoweb3d::IGW3DStereoDriverPtr(), pcallbackclass );

    if( !window1.lock() )
    {
		std::cout<< "Error Creating a window!" << std::endl;
        return;
    }

    Geoweb3d::IGW3DCameraCollection* cameras = window1.lock()->get_CameraCollection();

    Geoweb3d::IGW3DCameraWPtr camera1 =	cameras->create( "1Camera!" );
	std::cout<< "Camera Name Readback: " << camera1.lock()->get_Name() << std::endl;

    ////Create a small 'rear view mirror' camera.

    ///////////////////////////////////////////////////////////////////////

    Geoweb3d::IGW3DCameraWPtr camera2 =	cameras->create( "2Camera!" );
    camera2.lock()->put_NormalizeWindowCoordinates( 0.25f, 0.75f, 0.0f , 0.25f );

    //////////////////////////////////////////////////////////////////////

    float heading, pitch, roll;

	while( sdk_ptr->draw( window1 ) == Geoweb3d::GW3D_sOk )
    {
        const Geoweb3d::GW3DPoint* loc1 = 0;

        if( !camera1.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController* position_controller1 = camera1.lock()->get_CameraController();

            loc1 = position_controller1->get_Location( );

            position_controller1->put_Location( loc1->get_X() + .00001, loc1->get_Y() + .00001 );

            //again, this is pointing out the readback and setting capabilities

            position_controller1->get_Rotation( heading, pitch, roll );
            position_controller1->put_Rotation( heading = heading + 2.f, pitch = pitch + 1.1f, roll = roll + 1.f );
        }

        if( !camera2.expired() ) //This is just showing a best practice. Just in case the window this camera belongs to was deleted.
        {
            Geoweb3d::IGW3DCameraController *position_controller2 = camera2.lock()->get_CameraController();

            if( !loc1 )
                loc1 = position_controller2->get_Location( );

            position_controller2->put_Location( loc1->get_X(), loc1->get_Y() );
            position_controller2->put_Rotation( -heading, -pitch, roll );
        }
    }

	if( pcallbackclass )	
		delete pcallbackclass;

    pcallbackclass = 0;
}

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_ptr )
{
    SimpleAndQuick_CameraSideBySide( sdk_ptr );
    SimpleAndQuick_CameraWithinCamera( sdk_ptr );
    SimpleAndQuick_CameraWithinCamera_WithCallback( sdk_ptr );
    SimpleAndQuick_CameraAfterWindowCreation( sdk_ptr );

    SimpleAndQuick_CameraAfterWindowCreationTwoWindow( sdk_ptr );
    SimpleAndQuick_CameraAfterWindowCreationTwoWindow_offset( sdk_ptr );
}

int _tmain( int argc, _TCHAR* argv[])
{
    SetInformationHandling( );

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if( sdk_context )
    {
		//create the initialization configuration
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
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
	std::cout << "General Info: " << msg;
}

/*! Information handling is not required */
void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}