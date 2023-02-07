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
#include <deque>
#include <string>
#include <locale>
#include <algorithm>
#include <set>
#include <map>
#include <ctype.h>
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

#include "engine/IGW3DEventStream.h"
//interfaces that will be going away.
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"
#include "SmokeController.h"

#include "../Common/NavigationHelper.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();


Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );
Geoweb3d::IGW3DVectorDataSourceWPtr CreateGeoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *name );

class MiniEngine;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Pre post draw tasks. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrePostDrawTasks
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw() = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPostDraw() =0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Smoke controller task. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SmokeControllerTask : public PrePostDrawTasks
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="rep">	  	The rep. </param>
    /// <param name="pengine">	[in,out] If non-null, the pengine. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SmokeControllerTask(Geoweb3d::IGW3DVectorRepresentationWPtr rep, MiniEngine *pengine): stream_ (rep ),pengine_(pengine)
    {
        layer_ = rep.lock()->get_VectorLayer();
        smoke_elevation_offset_ = .1f;

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ~SmokeControllerTask() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw( )
    {

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPostDraw()
    {
        layer_.lock()->Stream( &stream_ );
        smoke_elevation_offset_ += .05f;
        stream_.SetSmokeElevationOffset( smoke_elevation_offset_ );
    }


private:
    /// <summary>	The smoke elevation offset. </summary>
    float smoke_elevation_offset_;
    /// <summary>	The stream. </summary>
    SmokeController stream_;
    /// <summary>	The layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr layer_;
    /// <summary>	The pengine. </summary>
    MiniEngine *pengine_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DEventStream, public Geoweb3d::IGW3DWindowCallback
{
public:

	Geoweb3d::IGW3DCameraWPtr current_camera_;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="sdk_context">	Context for the sdk. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) 
		: sdk_context_(sdk_context), 
		nav_helper_(new NavigationHelper())
	{}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~MiniEngine()
    {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Updates this object. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Update()
    {
        DoPreDrawWork();

        if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
        {
            DoPostDrawWork();
            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads a configuration. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="example_name">	Name of the example. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadConfiguration(const char *example_name )
    {
        window_ = CreateWindowCreation( sdk_context_, example_name, this );

        if(window_.expired())return false;

        camera_ = CreateCamera(window_,"Camera 1");
        camera_controller_ = camera_.lock()->get_CameraController();

		camera_controller_->put_EnableOverride(true);  //allow the camera manipulator functions to override the camera location
        if(camera_.expired())return false;

        sdk_context_->put_EnableEventStream( true );

        //start up in space over Binghamton, NY.
        camera_start_location.put_X(-75.9833);
        camera_start_location.put_Y(42.2167);
        camera_start_location.put_Z(410383.0); //elevation (meters) the space station is above earth

		//Add camera to the navigation helper
		nav_helper_->add_Camera(camera_);
        return true;
    }

    ////////////////////////////////////////(//////////////////////////////////////////////////////////
    /// <summary>	Loads a data. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="rootpath">	The rootpath. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadData( const char *rootpath )
    {

        //imagery has yet to be brought over to the C++ api.
        std::string imagerypath(rootpath);
        imagerypath += "/DC_1ft_demo.ecw";
  
		//lets try to get a starting point for the camera by looking at all the vector data
        //and zoom to the center area of its bounds. This is more of an SDK test too, as
        //we go through all the vector drivers and gather all the layers etc. to find
        //the overall 2D bounding box.
        Geoweb3d::GW3DEnvelope env;

		Geoweb3d::IGW3DRasterDriverCollection *raster_drivers = sdk_context_->get_RasterDriverCollection();
        Geoweb3d::GW3DResult res;
        //NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
        //having to guess which data source driver is needed to open up a particular dataset.
		Geoweb3d::IGW3DRasterDataSourceWPtr rdatasource = raster_drivers->auto_Open(imagerypath.c_str(), res);

		if(!rdatasource.expired())
		{
			Geoweb3d::IGW3DRasterLayerCollection* layer_collection = rdatasource.lock()->get_RasterLayerCollection();
			Geoweb3d::IGW3DRasterLayerWPtr layer = layer_collection->get_AtIndex(0);

			if (!layer.expired())
			{
				Geoweb3d::IGW3DRasterRepresentationDriverCollection* raster_repdrivers = sdk_context_->get_RasterLayerRepresentationDriverCollection();
				Geoweb3d::IGW3DRasterRepresentationDriverWPtr imagery_driver = raster_repdrivers->get_Driver("Imagery");
				Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
				params.page_level = 0;
				params.priority = 0;
				params.representation_layer_activity = true;
				Geoweb3d::IGW3DRasterRepresentationWPtr imagery_rep = imagery_driver.lock()->get_RepresentationLayerCollection()->create(layer, params);

				Geoweb3d::GW3DEnvelope layer_bounds = layer.lock()->get_Envelope();
				env.merge(layer_bounds);
			}
		}
		else
		{
			printf("Imagery not found!\n");
			env.put(-77.0, -77.1, 38.9, 40.0, 0.0, 0.0);
		}

        //you set the end locatoin, as this is where you want to end up when
        //the animation is completed
		camera_start_location.put_X((env.MinX + env.MaxX) * 0.5);
		camera_start_location.put_Y((env.MinY + env.MaxY) * 0.5);
        camera_start_location.put_Z( 150.0);

		camera_.lock()->get_CameraController()->put_Location(camera_start_location.get_X(), camera_start_location.get_Y());
		camera_.lock()->get_CameraController()->put_Elevation(camera_start_location.get_Z(), Geoweb3d::IGW3DPositionOrientation::Relative);

		// create the moving 'smoke':
		features_datasource = CreateGeoweb3dDataSource( sdk_context_, "Dynamic_Features.geoweb3d");

		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());

        smoke_dynamic_layer = features_datasource.lock()->get_VectorLayerCollection()->create("Smoke_Dynamic", Geoweb3d::gtPOINT_25D, field_definition );
		
		smoke_static_layer = features_datasource.lock()->get_VectorLayerCollection()->create("Smoke_Static", Geoweb3d::gtPOINT_25D, field_definition );

		
        const unsigned number_of_points  = 1;
        double delta = .0001;        
        double x = -(number_of_points*0.5*delta);
		double y = -(number_of_points*0.5*delta);		

		Geoweb3d::IGW3DPropertyCollectionPtr field_values ( field_definition->create_PropertyCollection() );

        for(unsigned i = 0; i < number_of_points; ++i)
        {           
            x += delta;   
			
            //y += delta;
			Geoweb3d::GW3DPoint geom = Geoweb3d::GW3DPoint(camera_start_location.get_X() - x, camera_start_location.get_Y(), 2.0);		
			Geoweb3d::GW3DPoint geom_2 = Geoweb3d::GW3DPoint(camera_start_location.get_X() + x, camera_start_location.get_Y(), 2.0);		

            //layer was created as a point type, so only add 1 at a time (Geoweb3d::gtPOINT)
            const unsigned num_of_geometry_vertex = 1;

            unsigned feature_id_retval = 0; //feature id would be the feature ID like that in a shapefile or geodatabase.  In excel or access, I think its called a key.           

			Geoweb3d::GW3DResult result;

			long feature_id = smoke_dynamic_layer.lock()->create_Feature( field_values, &geom, result);
			if( !Geoweb3d::Succeeded(result) )
            {
                printf("create_FeatureWithinMemoryFile Error\n");
            }
			long feature_id_2 = smoke_static_layer.lock()->create_Feature( field_values, &geom_2, result);
            if( !Geoweb3d::Succeeded(result) )
            {
                printf("create_FeatureWithinMemoryFile Error\n");
            }
        }



		//Nothing is stopping you from putting a layer into a mode where 
		//the geometry is editable before adding the data.  It depends 
		//on your use case.  As once the commit is supported, if you take
		//this layer out of edit mode, all the fids will be back 
		//to this location added above, and we can start all over.  However
		//this wastes more memory as we need to maintain the original
		//data, so if you are never going to commit your data to static,
		//or make a new "starting point" to replay, its probably more efficent
		//if you set the layer into an editable mode before you add
		//any features.
		smoke_dynamic_layer.lock()->put_GeometryEditableMode( true );

        //now represent it..
        Geoweb3d::IGW3DVectorRepresentationDriverWPtr smoke_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "Smoke" );		

        Geoweb3d::GW3DResult validitychk;

        if(!Geoweb3d::Succeeded( validitychk =  smoke_driver.lock()->get_CapabilityToRepresent( smoke_dynamic_layer ) ))
        {
            printf("not able to mix this geometry type with the rep..\n");
        }
        else
        {
            //good to go!
            Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			params.page_level = 6;

			//representing the dynamic layer
            smokerepresentation = smoke_driver.lock()->get_RepresentationLayerCollection()->create( smoke_dynamic_layer, params );
		  
			//representing the static layer
			smoke_driver.lock()->get_RepresentationLayerCollection()->create( smoke_static_layer, params );

			smokerepresentation.lock()->get_ClampRadiusFilter()->put_MaxRadius(3000.0);
			{
				//just a test
				const Geoweb3d::IGW3DDefinitionCollection* definition = smokerepresentation.lock()->get_PropertyCollection()->get_DefinitionCollection();
				Geoweb3d::IGW3DPropertyCollectionPtr properties = definition->create_PropertyCollection();
				if (definition->count() != properties->count())
				{
					printf("Geoweb3d software bug: properties has %d fields, when the definition that created it has %d definitions!\n"
						, properties->count()
						, definition->count());
				}
				else
				{
					printf("properties has %d fields and the definition that created it has %d definitions!\n"
						, properties->count()
						, definition->count());
				}
			}
        }

        SmokeControllerTask *p= new SmokeControllerTask(smokerepresentation, this);
        this->frame_tasks_.push_back(p);

        return true;
    }

private:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw work operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DoPreDrawWork()
    {
        //process all the vector pipeline events
        sdk_context_->run_EventStream( *this );

        for(unsigned i = 0; i < frame_tasks_.size(); ++i)
        {
            frame_tasks_[i]->RunPreDraw();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw work operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DoPostDrawWork()
    {

        for(unsigned i = 0; i < frame_tasks_.size(); ++i)
        {
            frame_tasks_[i]->RunPostDraw();
        }
    }

   
private:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	call back for 2d drawing/client side. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="camera">	The camera. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera )
    {
        // Draw icons in 2D or just to find out whats in view. If you are drawing in OpenGL, at this
        // point we have an OpenGL 3.3 render context inside the SDK. 
       // CameraFrustumContentsTest frustum_intersection;
       // sdk_context_->get_VectorRepresentationDriverCollection()->Stream( &frustum_intersection );
    }

	virtual void OnCreate( ) {};
	virtual void OnDrawBegin( ) {}; 
	virtual void OnDrawEnd( ) {};	

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Linear interpolation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="startvalue">	The startvalue. </param>
    /// <param name="endvalue">  	The endvalue. </param>
    /// <param name="value">	 	The value. </param>
    ///
    /// <returns>	. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static double linearInterpolation( double startvalue, double endvalue, double value)
    {
        if (value < startvalue) return startvalue;
        if (value > endvalue)  return endvalue;

        return (endvalue - value) * startvalue + value * endvalue;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="win_event">	The window event. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event ) override
    {

		current_camera_ = nav_helper_->ProcessEvent(win_event, window_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, raw data directly from the windows message pump. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="hWndParent">	The window parent. </param>
    /// <param name="message">   	The message. </param>
    /// <param name="wParam">	 	The wParam field of the message. </param>
    /// <param name="lParam">	 	The lParam field of the message. </param>
    ///
    /// <returns>	. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual int ProcessLowLevelEvent( OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam )
    {
        // We don't forward the WM_CLOSE message to prevent the internal SDK from closing us (click on X)
        if (message == WM_CLOSE)
        {
            printf("ProcessLowLevelEvent: WM_Close detected and prevented!\n");
            return 0;
        }
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// Pipeline messages Because we are a globe and can load data everywhere, we are never *done*
    /// loading anything as if you move away and come back, its going to load the area again.
    /// </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="representation">	The representation. </param>
    /// <param name="etype">		 	The etype. </param>
    /// <param name="unique_id">	 	Unique identifier. </param>
    /// <param name="bounds">		 	The bounds. </param>
    /// <param name="value">		 	The value. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void OnPagingEvent( Geoweb3d::IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const Geoweb3d::GW3DEnvelope& bounds, unsigned int value )
    {
        if(etype ==  Geoweb3d::IGW3DEventStream::QuickCacheCreateFinished)
        {
            printf("Cache for this Envelope/unique_id finished : %s\n", representation.lock()->get_Driver().lock()->get_Name());
        }
        else if(etype ==  Geoweb3d::IGW3DEventStream::DataSourceFinishedLoading)
        {
            printf("Loading for this Envelope/unique_id finished : %s\n", representation.lock()->get_Driver().lock()->get_Name());
        }
        else if(etype ==  Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete || etype ==  Geoweb3d::IGW3DEventStream::DataSourceWorkEstimationComplete  )
        {
            printf("Percent for this area [%s] %d \n", representation.lock()->get_Driver().lock()->get_Name(), value );
        }
    }

    virtual void OnStereoEvent( Geoweb3d::IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype )
    {
        if ( etype == Geoweb3d::IGW3DEventStream::HMD_CONNECTED )
        {
            printf( "A HMD has been connected. \n" );
        }
        else if ( etype == Geoweb3d::IGW3DEventStream::HMD_DISCONNECTED )
        {
            printf( "A HMD has been disconnected. \n" );
        }
    }

private:
  

    /// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    /// <summary>	The window. </summary>
    Geoweb3d::IGW3DWindowWPtr window_;
    /// <summary>	The camera. </summary>
    Geoweb3d::IGW3DCameraWPtr camera_;
    /// <summary>	The camera controller. </summary>
    Geoweb3d::IGW3DCameraController *camera_controller_;

    /// <summary>	experimental linearslerp for moving around.  Use at your own risk. </summary>
	Geoweb3d::GW3DPoint  camera_start_location;

    /// <summary>	The features datasource. </summary>
    Geoweb3d::IGW3DVectorDataSourceWPtr  features_datasource;
    /// <summary>	The smoke static layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr smoke_static_layer;
	/// <summary>	The smoke dynamic layer. </summary>
	Geoweb3d::IGW3DVectorLayerWPtr smoke_dynamic_layer;
    /// <summary>	The smokerepresentation. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr  smokerepresentation;

	NavigationHelper* nav_helper_;

    std::deque<PrePostDrawTasks*> frame_tasks_;

}; //engine end of class

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates window creation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">  	Context for the sdk. </param>
/// <param name="title">			The title. </param>
/// <param name="window_events">	(optional) [in,out] If non-null, the window events. </param>
///
/// <returns>	The new window creation. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0 )
{
    Geoweb3d::IGW3DWindowCollection * wcol = sdk_context->get_WindowCollection(  );

    Geoweb3d::IGW3DWindowWPtr window = wcol->create_3DWindow(title,GW3D_OVERLAPPED,50,50,1280,720,0,Geoweb3d::IGW3DStereoDriverPtr(),window_events);
	window.lock()->put_Quality(/*0 nvidia has a driver but with 0*/ 1 );


    if(window.expired())
    {
        printf("Error Creating window: [%s]!\n", title );

        return Geoweb3d::IGW3DWindowWPtr();
    }

    return window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates geoweb 3D data source. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">   	Context for the sdk. </param>
/// <param name="datasourcename">	The datasourcename. </param>
///
/// <returns>	The new geoweb 3D data source. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DVectorDataSourceWPtr CreateGeoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *datasourcename )
{
    Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

    if(!driver.expired())
    {
        Geoweb3d::IGW3DVectorDataSourceCollection *datasource_collection = driver.lock()->get_VectorDataSourceCollection();

        return datasource_collection->create( datasourcename  );
    }
    else
    {
        printf("SDK Bug Detected\n");
    }

    return Geoweb3d::IGW3DVectorDataSourceWPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates a camera. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="windowptr">  	The windowptr. </param>
/// <param name="cameratitle">	The cameratitle. </param>
///
/// <returns>	The new camera. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
 

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

    if(pengine->LoadConfiguration("Smoke Representations"))
    {
        ////******* CHANGE THIS TO YOUR SAMPLE DATA LOCATION ******/
        const char * root_path = "../examples/media";
        //  download the Geoweb3d DC sample project here: http://www.geoweb3d.com/download/sample-projects/
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
    printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
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
