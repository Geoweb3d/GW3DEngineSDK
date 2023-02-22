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
#include <vector>
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
#include "engine/GW3DFrameAnalysis.h"
#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DLineSegmentIntersectionTestResultsCollection.h"
#include "engine/IGW3DAnimationController.h"
#include "engine/IGW3DAnimationPath.h"
#include "engine/IGW3DSceneGraphContext.h"
#include "engine/IGW3DImageCollection.h"
#include "engine/IGW3DImage.h"

//interfaces that will be going away.
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"

#include "EntityController.h"
#include "../Common/NavigationHelper.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


/// <summary>	The myinfobuf[ GW3D_MAX_IO_BUFFERSIZE]. </summary>

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

static unsigned ANIMATION_FRAMES = 100;
static int MAX_ENTITIES = 20;

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );
Geoweb3d::IGW3DVectorDataSourceWPtr CreateGeoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *name );

class MiniEngine;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	utility function. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="x0">	The x coordinate 0. </param>
/// <param name="x1">	The first x value. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

double closed_interval_rand( double x0, double x1 )
{
    return x0 + (x1 - x0) * rand() / ((double) RAND_MAX);
}

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
/// <summary>
/// now, whats important is that u set the layer to be in editing mode! this is because the
/// feature ID's new geometry positions need to be updated through all representations..
///     people_layer.lock()->todohack_editmode();
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="searchingfor">	   	[in,out] The searchingfor. </param>
/// <param name="stringofinterest">	[in,out] The stringofinterest. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool DoesStringContain( std::string &searchingfor,  std::string &stringofinterest )
{
    std::transform(searchingfor.begin(), searchingfor.end(), searchingfor.begin(), toupper );
    std::transform(stringofinterest.begin(), stringofinterest.end(), stringofinterest.begin(), toupper );

    if(stringofinterest.find( searchingfor.c_str() )!=std::string::npos)
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DEventStream, public Geoweb3d::IGW3DWindowCallback
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="sdk_context">	Context for the sdk. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) 
        : sdk_context_(sdk_context)
        , nav_helper_ (new NavigationHelper())
    {
        animation_controller_ = sdk_context->get_AnimationController();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~MiniEngine()
    {
    }

    bool doubleEquals(double left, double right, double epsilon) {
        return (fabs(left - right) < epsilon);
    };

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

        camera_start_location.put_X(-77.029487418224051);
        camera_start_location.put_Y(38.890999972745576);
        camera_start_location.put_Z(427.69545963499695);

        camera_controller_->put_Rotation( 349.390259f, 49.2000046f, 0.0 );

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

        //playback_tour_index_
        animation_paths_.emplace_back(animation_controller_->create_AnimationPathFromFile("../examples/media/tours/ENTITY1.g3tour"));
        animation_paths_.emplace_back(animation_controller_->create_AnimationPathFromFile("../examples/media/tours/ENTITY2.g3tour"));
        animation_paths_.emplace_back(animation_controller_->create_AnimationPathFromFile("../examples/media/tours/ENTITY3.g3tour"));

        auto GetStartPointFromPath = [&](const int path_index, Geoweb3d::GW3DPathPoint& point) -> bool
        {
            if (animation_paths_[path_index])
            {
                auto first_time = animation_paths_[path_index]->get_FirstTime();

                if (animation_paths_[path_index]->get_InterpolatedControlPoint(first_time, point))
                {
                    return true;
                }
            }

            return false;
        };

        Geoweb3d::GW3DPathPoint start_point;

        camera_.lock()->get_CameraController()->put_Location(camera_start_location.get_X(), camera_start_location.get_Y());
        camera_.lock()->get_CameraController()->put_Elevation(camera_start_location.get_Z(), Geoweb3d::IGW3DPositionOrientation::Relative);

        CreateDynamicEntityLayer_("BillBoard");

        srand(17);
        int path_index = 0;
        //Create a few editable features in the layer and register an animation playback object with each feature.
        //The path used by each playback object is just a simple modulus of its index and the number of available 
        //paths( in this case 3 )
        for (int i = 0; i < MAX_ENTITIES; ++i)
        {
            path_index = i % 3; //only 3 path file
            animation_playback_objects_.emplace_back(animation_controller_->create_AnimationPathPlayback(animation_paths_[path_index], &entity_stream_));

            GetStartPointFromPath(path_index, start_point);

            int entity_id = SpawnDynamicEntity_(start_point);

            if (entity_id != -1 )
            {
                if (i == 0)
                {
                    entity_representation_.lock()->put_Enabled(true);
                }

                entity_stream_.RegisterEntityPlaybackObject(entity_id, animation_playback_objects_[i]);

                TourPathPoint path_point = {};
                path_point.longitude = start_point.longitude;
                path_point.latitude = start_point.latitude;
                path_point.elevation = start_point.elevation;

                entity_stream_.SeEntityPosition(entity_id, path_point);
            }
        }

        return true;
    }

    int SpawnDynamicEntity_(Geoweb3d::GW3DPathPoint& start_point)
    {
        if (entity_layer_.expired())
            return -1;

        Geoweb3d::GW3DPoint geom(start_point.longitude, start_point.latitude, start_point.elevation);

        Geoweb3d::GW3DResult result;

        Geoweb3d::IGW3DPropertyCollectionPtr field_values(entity_layer_.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection());

        long feature_id = entity_layer_.lock()->create_Feature(field_values, &geom, result);

        if (!Geoweb3d::Succeeded(result))
        {
            printf("create_FeatureWithinMemoryFile Error\n");
            return -1;
        }

        return static_cast<int>(feature_id);
    }


    void CreateDynamicEntityLayer_( const char* representation_driver_name )
    {
        if (!entity_datasource_.expired())
            return;

        Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");
        if (!driver.expired())
        {
            Geoweb3d::IGW3DVectorDataSourceCollection* datasource_collection = driver.lock()->get_VectorDataSourceCollection();
            std::string data_source_name = ":Dynamic_Entities.geoweb3d";
            entity_datasource_ = datasource_collection->create(data_source_name.c_str());
        }
        else
        {
            printf("SDK Bug Detected\n");
            return;
        }

        Geoweb3d::IGW3DVectorRepresentationDriverWPtr rep_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver(representation_driver_name);

        Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());

        Geoweb3d::IGW3DPropertyCollectionPtr billboard_default_properties = rep_driver.lock()->get_PropertyCollection()->create_Clone();

        billboard_default_properties->put_Property( Geoweb3d::Vector::BillboardProperties::TEXTURE_PALETTE_INDEX, 0);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("TRANSLATION_Z_OFFSET"), 0.0);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("RED"), 1.0);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("GREEN"), 1.0);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("BLUE"), 1.0);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("ALPHA"), 0.25);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("TEXTURE_TO_COLOR_BLEND"), 1.0);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("TEXTURE_PALETTE_INDEX"), 0);

        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("SIZE_X"), 40.0);
        billboard_default_properties->put_Property(billboard_default_properties->get_DefinitionCollection()->get_IndexByName("SIZE_Y"), 40.0);

        entity_layer_ = entity_datasource_.lock()->get_VectorLayerCollection()->create("Entity_Dynamic", Geoweb3d::gtPOINT_25D, field_definition);
        if (!entity_layer_.expired())
        {
            entity_layer_.lock()->put_GeometryEditableMode(true);


            Geoweb3d::GW3DResult validitychk;
            if (!Geoweb3d::Succeeded(validitychk = rep_driver.lock()->get_CapabilityToRepresent(entity_layer_)))
            {
                printf("not able to mix this geometry type with the rep..\n");
            }
            else
            {
                //good to go!
                Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
                params.page_level = 8;
                params.representation_layer_activity = false;

                auto image_collection = sdk_context_->get_SceneGraphContext()->create_ImageCollection();

                image_collection->create("../examples/media/test_images/one.png");
                image_collection->create("../examples/media/test_images/two.png");
                image_collection->create("../examples/media/test_images/three.png");

                params.finalization_token = image_collection->create_FinalizeToken();

                params.representation_default_parameters = billboard_default_properties;
                entity_representation_ = rep_driver.lock()->get_RepresentationLayerCollection()->create(entity_layer_, params);
                entity_stream_.SetRepresentation(entity_representation_);
            }
        }
    }


private:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw work operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DoPreDrawWork()
    {
        entity_layer_.lock()->Stream(&entity_stream_);

        //process all the vector pipeline events
        sdk_context_->run_EventStream( *this );

		if(camera_animation_timer_ < 1) //we know the animation was completed as a time 0.0 to 1.0 where 1 is completed (end position)
        {
            camera_animation_timer_ += (1.f/60.f);
        }

        for(unsigned i = 0; i < frame_tasks_.size(); ++i)
        {
            frame_tasks_[i]->RunPreDraw();
        }


        static int last_mouse_X = 0;
        static int last_mouse_Y = 0;

        static Geoweb3d::IGW3DBearingHelperPtr bearinghelper = Geoweb3d::IGW3DBearingHelper::create();
        static Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr center_pixel_query;

        Geoweb3d::IGW3DCameraController* controller = camera_.lock()->get_CameraController();

        if (x_ != 0 || y_ != 0)
        {
            if (!center_pixel_query.expired())
            {
                if (center_pixel_query.lock()->get_IsValid())
                {
                    //What is the current range/bearing...

                    //from camera
                    bearinghelper->put_SourceLocation(controller->get_Location()->get_X(), controller->get_Location()->get_Y(), controller->get_Location()->get_Z());

                    // to center pixel
                    const Geoweb3d::GW3DPoint* center_pt = center_pixel_query.lock()->get_GeodeticResult();
                    bearinghelper->put_TargetLocation(center_pt->get_X(), center_pt->get_Y(), center_pt->get_Z());

                    double bearing, pitch, distance;
                    bearinghelper->calculate_result(bearing, pitch, distance);

                    if (!doubleEquals(controller->get_Heading(), bearing, 0.0001))
                    {
                        printf("The camera heading is %5.2f, but the calculated bearing to the camera focal point is %5.2f\n",
                            controller->get_Heading(), bearing);
                    }

                    if (!doubleEquals(controller->get_Pitch(), pitch, 0.0001))
                    {
                        printf("The camera pitch is %5.2f, but the calculated pitch to the camera focal point is %5.2f\n",
                            controller->get_Pitch(), pitch);
                    }

                    double target_bearing = bearing + x_;
                    double target_pitch = pitch + y_;

                    //now, orbit around center pixel location, keeping the same range
                    controller->orbit(target_bearing, target_pitch, distance, center_pt);

                    // see what the new range/bearing is...

                    //from camera
                    bearinghelper->put_SourceLocation(controller->get_Location()->get_X(), controller->get_Location()->get_Y(), controller->get_Location()->get_Z());

                    // to center pixel
                    double end_bearing, end_pitch, end_distance;
                    bearinghelper->put_TargetLocation(center_pt->get_X(), center_pt->get_Y(), center_pt->get_Z());
                    bearinghelper->calculate_result(end_bearing, end_pitch, end_distance);

                    bool fail = false;
                    //now we would expect the range/bearing from the camera to be the same as what we just set
                    if (!doubleEquals(target_bearing, end_bearing, 0.0001))
                    {
                        fail = true;
                        printf("Orbit to bearing: [%5.4f] but new bearing is: [%5.4f]\n", target_bearing, end_bearing);
                    }
                    if (!doubleEquals(target_pitch, end_pitch, 0.0001))
                    {
                        fail = true;
                        printf("Orbit to pitch : [%5.4f] but new pitch is: [%5.4f]\n", target_pitch, end_pitch);
                    }
                    if (!doubleEquals(distance, end_distance, 0.0001))
                    {
                        fail = true;
                        printf("Orbit to range: [%5.4f] but new range is: [%5.4f]\n", distance, end_distance);
                    }

                    if (fail)
                    {
                        //system("pause");
                    }

                    //now point the camera back at the previous center pixel location
                    controller->look_At(center_pt);

                }
            }
        }

        x_ = 0;
        y_ = 0;

        //Sequentially start all playback for each item after a random number of frames
        if (num_playback_to_start != 0)
        {
            static bool first_frame = true;

            if (next_playback_frame_count == 0)
            {
                if (!first_frame)
                {
                    animation_playback_objects_[playback_tour_index_].lock()->Play();
                    --num_playback_to_start;
                    ++playback_tour_index_;
                }

                if (num_playback_to_start == 0)
                {
                    playback_tour_index_ = 0;
                }

                next_playback_frame_count = rand() % 300;
            }

            first_frame = false;
            --next_playback_frame_count;
        }
        else
        {
            playback_tour_index_ = 0;
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

       if(sdk_context_->get_LineSegmentIntersectionTestCollection ()->count())
            sdk_context_->get_LineSegmentIntersectionTestCollection ()->get_AtIndex(0).lock()->put_Enabled ( false ); //only do a one shot

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

        switch (win_event.Type)
        {
            case win_event.KeyPressed:
            {
                switch (win_event.Key.code)
                {
                    case Geoweb3d::Key::I:
                    {
                        y_ = -1;
                    }
                    break;
                    case Geoweb3d::Key::K:
                    {
                        y_ = 1;
                    }
                    break;
                    case Geoweb3d::Key::J:
                    {
                        x_ = 1;
                    }
                    break;
                    case Geoweb3d::Key::L:
                    {
                        x_ = -1;
                    }
                    break;
                    case Geoweb3d::Key::P:
                    {
                        if (!animation_playback_objects_.empty())
                        {
                            animation_playback_objects_[playback_tour_index_].lock()->Play();
                        }
                    }
                    break;
                    case Geoweb3d::Key::S:
                    {
                        if (!animation_playback_objects_.empty())
                        {
                            animation_playback_objects_[playback_tour_index_].lock()->Stop();
                        }
                    }
                    break;
                    default:
                    break;
                }
            }
        }
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
    }

    virtual void OnStereoEvent( Geoweb3d::IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype )
    {
    }

private:
    friend class CameraMoveToAnimationTask;

    Geoweb3d::IGW3DCameraWPtr current_camera_;

    /// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    /// <summary>	The window. </summary>
    Geoweb3d::IGW3DWindowWPtr window_;
    /// <summary>	The camera. </summary>
    Geoweb3d::IGW3DCameraWPtr camera_;
    /// <summary>	The camera controller. </summary>
    Geoweb3d::IGW3DCameraController *camera_controller_;

    Geoweb3d::IGW3DAnimationController* animation_controller_ = nullptr;

	/// <summary>	The camera animation timer. </summary>
    float camera_animation_timer_;
 
    /// <summary>	experimental linearslerp for moving around.  Use at your own risk. </summary>
	Geoweb3d::GW3DPoint  camera_start_location;

	Geoweb3d::GW3DPoint camera_animation_target;

    EntityController entity_stream_;
    Geoweb3d::IGW3DVectorDataSourceWPtr entity_datasource_;
    Geoweb3d::IGW3DVectorLayerWPtr entity_layer_;
    Geoweb3d::IGW3DVectorRepresentationWPtr entity_representation_;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// Geoweb3d::IGW3DVectorLayerWPtr light_layer;
    /// Geoweb3d::IGW3DVectorRepresentationWPtr lightpoint_representation_;
    /// Geoweb3d::IGW3DVectorRepresentationWPtr extruded_representation_;
    /// Geoweb3d::IGW3DVectorRepresentationWPtr custom_representation_;
    /// </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    std::deque<PrePostDrawTasks*> frame_tasks_;

    std::vector<Geoweb3d::IGW3DAnimationPathPtr> animation_paths_;
    std::vector<Geoweb3d::IGW3DAnimationPathPlaybackWPtr> animation_playback_objects_;

    NavigationHelper* nav_helper_;

    double x_ = 0.0;
    double y_ = 0.0;

    int playback_tour_index_ = 0;
    int num_playback_to_start = MAX_ENTITIES;
    int next_playback_frame_count = 0;

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

    if(pengine->LoadConfiguration("Animation Playback"))
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
