// identify.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "../Common/NavigationHelper.h"
#include "../Common/IdentifyVector.h"
#include "../Common/MiniEngineCore.h"

#include <stdio.h>
#include <iostream>

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DVector.h"

// include the error handeling 
#include "Geoweb3dCore/SystemExports.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

// definition of buff and function prototypes

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();


Geoweb3d::IGW3DWindowWPtr CreateWindowAndCamera( Geoweb3d::IGW3DGeoweb3dSDKPtr p );

// will load the vector data from the source and populate the sdk
Geoweb3d::GW3DEnvelope LoadandRepresentVectorData(Geoweb3d::IGW3DGeoweb3dSDKPtr t_sdk_context)
{  
	Geoweb3d::GW3DResult res;
	//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
	//having to guess which data source driver is needed to open up a particular dataset.
	Geoweb3d::IGW3DVectorDataSourceWPtr t_data_source = t_sdk_context->get_VectorDriverCollection()->auto_Open("../examples/media/Building Footprints.shp", res);
	
	Geoweb3d::GW3DEnvelope w_env;

	if (!t_data_source.expired())
	{
		Geoweb3d::IGW3DVectorRepresentationDriverWPtr w_rep_driver = t_sdk_context->get_VectorRepresentationDriverCollection()->get_Driver("ExtrudedPolygon");
		if (!w_rep_driver.expired())
		{
			for (int ii =0; ii < (int)t_data_source.lock()->get_VectorLayerCollection()->count(); ii++)
			{
			
				if ( Succeeded(w_rep_driver.lock()->get_CapabilityToRepresent(t_data_source.lock()->get_VectorLayerCollection()->get_AtIndex(ii) )))
				{
					Geoweb3d::Vector::RepresentationLayerCreationParameter w_RLCP;

					//w_rep_driver.lock()->get_PropertyCollection(
					w_rep_driver.lock()->get_RepresentationLayerCollection()->create(t_data_source.lock()->get_VectorLayerCollection()->get_AtIndex(ii),w_RLCP);
					w_env.merge(t_data_source.lock()->get_VectorLayerCollection()->get_AtIndex(ii).lock()->get_Envelope());					

				}
				else
				{ // non compatable ExtrudedPolygon
					std::cout << " not able to represent vector layer [" << t_data_source.lock()->get_VectorLayerCollection()->get_AtIndex(ii).lock()->get_Name() << "] \n";
				}
			}
		}
		else
		{
			std::cout << "ERROR: getting representation driver \n";
		}
	}
	else
	{
		std::cout << "ERROR: vector data did not load \n";
	}
	return w_env;
}


class WindowCallback : public Geoweb3d::IGW3DWindowCallback, public IdentifyVector, public NavigationHelper, public MiniEngineCore			
{ // windows call back class for use wih the windows call back and the navagation helper class

public:
	WindowCallback(Geoweb3d::IGW3DGeoweb3dSDKPtr t_sdk_context) : IdentifyVector(t_sdk_context)
	{}
	virtual ~WindowCallback()
	{}
	virtual void OnCreate( )
	{}
	virtual void OnDrawBegin( )
	{}
	virtual void OnDrawEnd( )
	{}
	virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
	{}
	virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera, Geoweb3d::IGW3D2DDrawContext *drawcontext )
	{}

	void put_window(Geoweb3d::IGW3DWindowWPtr t_window_ptr)
	{
		t_window_ptr_ = t_window_ptr;
	}

	virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event  )
	{			
		NavigationHelper::ProcessEvent( win_event, t_window_ptr_ );
		IdentifyVector::ProcessEvent(   win_event, t_window_ptr_ );
	}
	void LoadData( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
	{
		LoadDemoImageryDataset(sdk_context);
	}
private:
	Geoweb3d::IGW3DWindowWPtr t_window_ptr_;
};


int _tmain(int argc, _TCHAR* argv[])
{

	 SetInformationHandling( );

// first step to interact with the SDK is to construct and SDK context,
// this is the primary context of communication between the sdk and host application
// the sdk context is -- <put a good descition here>

// currently only one sdk context is allowed to be instatiated at any one time.

// to create an image you have to use the create through the image collection
// to destroy that image, you have to use the mage collection

// all management is owned by the sdk
// all collections are management objects and are maticly created and managed by the sdk context this is why thay are raw pointers.
// the client (user of the sdk) will create and destroy member of the collections 


// all access of data mambers is encapulated with in the collections, all create and remove is done throught the
//  the mechanisum of a collection


	// creation of the sdk context
	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context =  Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface();

	if ( sdk_context ) 
	{	
		// the initial and only configuration of the sdk context.  this must be done before the sdk context can be used.
		// this configuration 
		Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
		sdk_init->put_ESRILicenseCheckout(false); //If you have an ESRI license and want to be able to load data using their drivers, remove this line

		// configuration of the sdk to use an authorized license as it can be seen from the Geoweb3d License Activation Center ( Geoweb3dLicenseTool.exe )
		// the string under product is the name of the license
        if( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
		{
			// run the application function
			// creation of the window
			//NavigationHelper w_navhelper;
			//WindowCallback w_windowCallBack(window_ptr);
			WindowCallback w_windowCallBack(sdk_context);			

			Geoweb3d::IGW3DWindowWPtr window_ptr = sdk_context->get_WindowCollection()->create_3DWindow("window title",GW3D_OVERLAPPED,0,0,1280,720,0,Geoweb3d::IGW3DStereoDriverWPtr(),&w_windowCallBack);
			w_windowCallBack.put_window ( window_ptr );
			
			Geoweb3d::GW3DEnvelope w_env;

			if (!window_ptr.expired())
			{
				// creation of the camera
				Geoweb3d::IGW3DCameraWPtr camera_ptr = window_ptr.lock()->get_CameraCollection()->create("Main camera");

				// adding camera 
				if (!camera_ptr.expired())
				{

					w_windowCallBack.LoadData(sdk_context);
					// load vector data source
					w_env = LoadandRepresentVectorData(sdk_context);

		
					double	longitude = (w_env.MinX + w_env.MaxX ) * 0.5;
					double	latitude  = (w_env.MinY + w_env.MaxY ) * 0.5;
					camera_ptr.lock()->get_CameraController()->put_Location( longitude,latitude); 
					camera_ptr.lock()->get_CameraController()->put_Elevation(50);
					camera_ptr.lock()->get_CameraController()->put_Rotation(300.0, 15.0, 0.0);

					w_windowCallBack.add_Camera(camera_ptr);	//add the camera to allow navigation, make sure this is done after adding the camera location (above)
						
					//add all the representations to get added to the identify task
					w_windowCallBack.addAllVectorRepresentationToTest();
					
					// reset the rep driver collection 
					Geoweb3d::IGW3DVectorRepresentationDriverWPtr repDriver;
					sdk_context->get_VectorRepresentationDriverCollection(true);
					
					// process loop 
					while(Geoweb3d::Succeeded( sdk_context->draw(window_ptr ) ))
					{
						w_windowCallBack.displayVectorRepresentationInterceptReport();										
					}
				}
				else
				{
					std::cout << "ERROR: the camera failed to create \n";
				}
			}
			else
			{
				std::cout << " ERROR: Window failed to create \n";
			}
		}
		else
		{	// error condition for the failure to get a valid license 
			std::cout << "ERROR: a valid license could not be accessed \n";
		}
	}
	else
	{ // error condition for the failure of instantiation of the sdk
		return -1;
	}
	
	 sdk_context = Geoweb3d::IGW3DGeoweb3dSDKPtr();

	return 0;
}

// the following functions are needed for the procesing of the lowest level error handeling
// this covers the error handeling for the creation of the SDK 

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char *msg )
{
    printf("General Info: %s", msg);
}
/*! Function handler for fatal errors inside the engine.  For the purposes of this demo, we only print to the end user what went wrong */
void my_fatal_function(const char *msg )
{
    printf("Fatal Info: %s", msg);
}
/*! Information handling is not required */
void SetInformationHandling()
{
    /*! Tell the engine about our error function handling */
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information,	my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning,		my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal,		    my_fatal_function);
}