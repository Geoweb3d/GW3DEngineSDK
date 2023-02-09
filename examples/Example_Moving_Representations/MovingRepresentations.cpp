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
#include "engine/GW3DFrameAnalysis.h"
#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DLineSegmentIntersectionTestResultsCollection.h"
//interfaces that will be going away.
#include "Geoweb3dCore/GeometryExports.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/LayerParameters.h"
#include "PeopleController.h"

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")


void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

static unsigned ANIMATION_FRAMES = 100;

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );
Geoweb3d::IGW3DVectorDataSourceWPtr CreateGeoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *name );

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsLights( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr light_layer );
Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer );
Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsCustom( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer);

void UpdateDefaultPolygonExtrusion(Geoweb3d::IGW3DVectorRepresentationWPtr polygonrepresentation, double new_height = 25.);
void PrintLineSegmentCollisions( Geoweb3d::IGW3DLineSegmentIntersectionTestCollection *collection);

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
/// <summary>	Camera move to animation task. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class CameraMoveToAnimationTask : public PrePostDrawTasks
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="query">  	The query. </param>
    /// <param name="pengine">	[in,out] If non-null, the pengine. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CameraMoveToAnimationTask(Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr query, MiniEngine *pengine) 
		: mouse_wheel_delta_(0),delta_set_(false), query_(query), pengine_(pengine), animation_base_speed_(1.0)
    {
        query_.lock()->put_Enabled( true );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~CameraMoveToAnimationTask()
    {

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Resets the query. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="x">	The x coordinate. </param>
    /// <param name="y">	The y coordinate. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void ResetQuery(int x, int y)
    {
        query_.lock()->put_WindowCoordinate(x, y);
        query_.lock()->put_Enabled( true );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw()
    {
        if(query_.expired()) return;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Mouse wheel delta. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="delta">	The delta. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void MouseWheelDelta( float delta )
    {
        mouse_wheel_delta_ = delta;
        delta_set_ = true;
    }

    virtual void RunPostDraw();

    /// <summary>	The query. </summary>
    Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr query_;
    /// <summary>	The pengine. </summary>
    MiniEngine *pengine_;
    /// <summary>	The mouse wheel delta. </summary>
    float mouse_wheel_delta_;
    /// <summary>	true to delta set. </summary>
    bool delta_set_;
    /// <summary>	base animation speed in meters per frame. </summary>
	double animation_base_speed_;

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	People controller task. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class PeopleControllerTask : public PrePostDrawTasks
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

    PeopleControllerTask(Geoweb3d::IGW3DVectorRepresentationWPtr rep, MiniEngine *pengine): stream_ (rep ),pengine_(pengine)
    {
        layer_ = rep.lock()->get_VectorLayer();
        people_elevation_offset_ = .1f;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ~PeopleControllerTask() {}

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
        people_elevation_offset_ += .05f;
        stream_.SetModelElevationOffset( people_elevation_offset_ );
    }


private:
    /// <summary>	The people elevation offset. </summary>
    float people_elevation_offset_;
    /// <summary>	The stream. </summary>
    PeopleController stream_;
    /// <summary>	The layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr layer_;
    /// <summary>	The pengine. </summary>
    MiniEngine *pengine_;
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
/// <summary>	Camera frustum intersection test. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CameraFrustumContentsTest : public Geoweb3d::IGW3DFrustumAnalysisStream
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Default constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CameraFrustumContentsTest() {};

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~CameraFrustumContentsTest() {};

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	a return of false will cancel the stream. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="camera">	The camera. </param>
    /// <param name="rep">   	The rep. </param>
    /// <param name="result">	[out] if non-null, the result. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool OnStream( Geoweb3d::IGW3DContentsResult *contents, Geoweb3d::IGW3DFeatureFeedbackResultCollection *featurefeedback )
    {

		//The fact we got the event on this camera for this representation means data is paged in within
		//your page level settings. Thus, is visible would mean the camera is looking at the bounding box area
		//of the paged in data. Else the data is paged in, but the camera is not looking/intersecting at the
		//bounding box of the representation data.
		if( contents->get_VectorRepresentationIsVisible() ) 
		{
			//using unsafe_get instead of lock for optimization, as we know with this example
			//we are not removing things to where something will be expired.
			printf("Window: %s\n",    contents->get_Camera().unsafe_get()->get_Window().unsafe_get()->get_Name() );
			printf("\tCamera: %s\n",   contents->get_Camera().unsafe_get()->get_Name());
			printf("\t\tRepresentation: %s\n", contents->get_VectorRepresentation().unsafe_get()->get_Driver().unsafe_get()->get_Name());
			printf("\t\tIs In Frustum: %s\n", contents->get_VectorRepresentationIsVisible() ? "true":"false");
			printf("\t\tVector Layer: %s\n", contents->get_VectorRepresentation().unsafe_get()->get_VectorLayer().unsafe_get()->get_Name());

			return false; //remove this to show more than just the first
			
			if(featurefeedback->get_WasEnabled()) // see IGW3DFeatureFeedbackConfiguration
			{			
				printf("\t\t\tFeature Feedback Results: %d\n", featurefeedback->count() );	

				for(unsigned int i = 0; i < featurefeedback->count();++i)
				{
					Geoweb3d::IGW3DFeatureFeedbackResult *val = featurefeedback->get_AtIndex(i);

					printf("\t\t\tObject ID: %d\n", val->get_ObjectID());
					printf("\t\t\tDistance from camera: %f\n", val->get_DistanceFromCamera());
					printf("\t\t\tIs Occluded by Earth: %s\n", val->get_OccludedByGlobe() ? "true":"false");
					printf("\t\t\tRendered In 3D Scene: %s\n", val->get_Rendered() ? "true":"false");
					printf("\t\t\tAprox number of pixels: %f\n", val->get_ApproximateNumPixelsOnScreen());
			
					if(val->get_FeatureID2DPixelLocationResult()->get_WasEnabled()) // see IGW3DFeatureFeedbackConfiguration
					{
						float nx,ny;
						val->get_FeatureID2DPixelLocationResult()->get_NormalizedCoordinate(nx,ny);
						printf("\t\t\t\tNormalized wc: width %f height %f\n", nx, ny);
						int pixel_x,pixel_y;
						val->get_FeatureID2DPixelLocationResult()->get_WindowCoordinate( pixel_x, pixel_y);
						printf("\t\t\t\tPixel wc: width %d height %d\n", pixel_x, pixel_y );		
					}
				}
			}
			else
			{
				//so we are looking at data now.  lets, to test the apis, to enable feature information to come			
				Geoweb3d::IGW3DFrameAnalysisContentsConfiguration * config  =
				contents->get_VectorRepresentation().unsafe_get()->get_FrameAnalysisContentsConfiguration();				
				config->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->put_Enabled(true);

				config->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration()->get_FeatureID2DPixelLocationConfiguration()->put_Enabled(true);
			}
		}
		else
		{
			printf("\t\tRepresentation: %s: Paged in but not within the configured frustum settings\n", 
				contents->get_VectorRepresentation().unsafe_get()->get_Driver().unsafe_get()->get_Name());

		}
  
        return true; //true means go through all the representations and drivers
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the error action. </summary>
    ///
    /// <remarks>	 </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool OnError( )
    {
		return true;
    }
};

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

    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) : sdk_context_(sdk_context), pcamera_task_(0) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~MiniEngine()
    {
        if(pcamera_task_)
            delete pcamera_task_;
    }

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

        //you set the end location, as this is where you want to end up when
        //the animation is completed
        camera_start_location.put_X( (env.MinX + env.MaxX ) * 0.5);
        camera_start_location.put_Y( (env.MinY + env.MaxY ) * 0.5);
        camera_start_location.put_Z( 500.0);

		camera_.lock()->get_CameraController()->put_Location(camera_start_location.get_X(), camera_start_location.get_Y());
		camera_.lock()->get_CameraController()->put_Elevation(camera_start_location.get_Z(), Geoweb3d::IGW3DPositionOrientation::Relative);

		// create the moving 'people':
		dynamic_features_datasource = CreateGeoweb3dDataSource( sdk_context_, "Dynamic_Features.geoweb3d");

		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
		field_definition->add_Property("PERSON_NAME", Geoweb3d::PROPERTY_STR, "person name");

        people_layer = dynamic_features_datasource.lock()->get_VectorLayerCollection()->create("People", Geoweb3d::gtPOINT_25D, field_definition );

        const unsigned number_of_points  = 10000;

        srand ( 22 );

        double R = .009;
        double Rsquared = R*R;
        double x,y,m;

		Geoweb3d::IGW3DPropertyCollectionPtr field_values ( field_definition->create_PropertyCollection() );
        char buff[32]= {0};

        for(unsigned i = 0; i < number_of_points; ++i)
        {
  							
          do
            {
                //make somewhat a uniform distribution
                x = R * (2 * closed_interval_rand(0,1) -1 );
                y = R * (2 * closed_interval_rand(0,1) -1 );
                m = x*x+y*y;
            }
            while (m > Rsquared);

            Geoweb3d::GW3DPoint geom( camera_start_location.get_X() - x, camera_start_location.get_Y() + y, 2.0);

            //layer was created as a point type, so only add 1 at a time (Geoweb3d::gtPOINT)
            const unsigned num_of_geometry_vertex = 1;

            unsigned feature_id_retval = 0; //feature id would be the feature ID like that in a shapefile or geodatabase.  In excel or access, I think its called a key.


            //index 0 is PERSON_NAME, so  we set the name either to the
            //for loop count or JOHN, as an example only.
            if(i%2)
            {
				field_values->put_Property(0,"JOHN");
            }
            else
            {

                sprintf(buff, "%d", i);
                field_values->put_Property(0,buff);
            }

			Geoweb3d::GW3DResult result;

			long feature_id = people_layer.lock()->create_Feature( field_values, &geom, result);

            if( !Geoweb3d::Succeeded(result) )
            {
                printf("create_FeatureWithinMemoryFile Error\n");
            }

        }

		//Nothing is stopping you from putting a layer into an mode where 
		//the geometry is editable before adding the data.  It depends 
		//on your use case.  As once the commit is supported, if you take
		//this layer out of edit mode, all the fids will be back 
		//to this location added above, and we can start all over.  However
		//this wastes more memory as we need to maintain the original
		//data, so if you are never going to commit your data to static,
		//or make a new "starting point" to replay, its probably more efficient
		//if you set the layer into an editable mode before you add
		//any features.
		people_layer.lock()->put_GeometryEditableMode( true );

        //now represent it..
        Geoweb3d::IGW3DVectorRepresentationDriverWPtr model_driver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "Model" );

        Geoweb3d::GW3DResult validitychk;

        if(!Geoweb3d::Succeeded( validitychk =  model_driver.lock()->get_CapabilityToRepresent( people_layer ) ))
        {
            printf("not able to mix this geometry type with the rep..\n");
        }
        // else
        {
            //good to go!
            Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			params.page_level = 6;
            modelrepresentation = model_driver.lock()->get_RepresentationLayerCollection()->create( people_layer, params );

			Geoweb3d::IGW3DFeatureFeedbackConfiguration* feature_feedback_config = modelrepresentation.lock()->get_FrameAnalysisContentsConfiguration()->get_FrustumAnalysis()->get_FeatureFeedbackConfiguration();
			feature_feedback_config->put_Enabled(false);  //SET THIS TO TRUE TO SEE INFO ON ALL FEATURES IN THE CAMERA FRUSTUM
			feature_feedback_config->get_FeatureID2DPixelLocationConfiguration()->put_Enabled(false);  //SET THIS TO TRUE TO SEE 2D WINDOW PIXEL LOCATIONS FOR ALL FEATURES IN THE CAMERA FRUSTUM

			modelrepresentation.lock()->get_ClampRadiusFilter()->put_MaxRadius(3000.0);
			{
				//just a test
				const Geoweb3d::IGW3DDefinitionCollection* definition = modelrepresentation.lock()->get_PropertyCollection()->get_DefinitionCollection();
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


        PeopleControllerTask *p= new PeopleControllerTask(modelrepresentation, this);
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

		if(camera_animation_timer_ < 1) //we know the animation was completed as a time 0.0 to 1.0 where 1 is completed (end position)
        {
            camera_animation_timer_ += (1.f/60.f);
        }

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

		PrintLineSegmentCollisions( sdk_context_->get_LineSegmentIntersectionTestCollection () );

        if(sdk_context_->get_LineSegmentIntersectionTestCollection ()->count())
            sdk_context_->get_LineSegmentIntersectionTestCollection ()->get_AtIndex(0).lock()->put_Enabled ( false ); //only do a one shot

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets global light color. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void SetGlobalLightColor()
    {
        //if(!lightpoint_representation_.expired())
        //{
        //    //variants are expensive to create and destroy, so reuse the object when you can.
        //    Geoweb3d::IGW3DVariant scratchpad;
        //    Geoweb3d::IGW3DPropertyCollection *properties = lightpoint_representation_.lock()->get_PropertyCollection();
        //    scratchpad = 1.0;
        //    properties->put_Property( Geoweb3d::LightPointProperties::RED, scratchpad );
        //    scratchpad = 0.0;
        //    properties->put_Property( Geoweb3d::LightPointProperties::GREEN, scratchpad );
        //    scratchpad = 1.0;
        //    properties->put_Property( Geoweb3d::LightPointProperties::BLUE, scratchpad );
        //    //the put here is the "commit" transaction in that this will apply the settings to the 3D representation.
        //    lightpoint_representation_.lock()->put_PropertyCollection( properties );
        //}
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
        CameraFrustumContentsTest frustum_intersection;
        sdk_context_->get_VectorRepresentationDriverCollection()->Stream( &frustum_intersection );
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
        switch(win_event.Type)
        {
        case win_event.MouseWheelMoved:
        {
            if(camera_animation_timer_ < 1)
            {
                printf("Scroll Camera Animation movement ignored, as last animation tasks is not completed\n");
            }
            else
            {
                if(!pcamera_task_)
                {
                    pcamera_task_ = new CameraMoveToAnimationTask ( camera_.lock()->get_WindowCoordinateToGeodeticQueryCollection()->create(), this );
					pcamera_task_->MouseWheelDelta((float)win_event.MouseWheel.delta);
                    pcamera_task_->query_.lock()->put_WindowCoordinate( win_event.MouseWheel.x, win_event.MouseWheel.y );
                    frame_tasks_.push_back( pcamera_task_ );
                }
                else
                {
                    pcamera_task_->ResetQuery( win_event.MouseButton.x, win_event.MouseButton.y );
                }
                printf("Scroll Camera Animation Started!\n");
            }

        }
        break;
        case win_event.MouseButtonPressed:
        {

            Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr linesegment;
            if(!sdk_context_->get_LineSegmentIntersectionTestCollection()->count())
            {
                linesegment =
                    sdk_context_->get_LineSegmentIntersectionTestCollection()->create();

                linesegment.lock()->get_VectorRepresentationCollection()->add( modelrepresentation );
            }
            else
            {
                linesegment =
					sdk_context_->get_LineSegmentIntersectionTestCollection()->get_AtIndex(0);

            }

            //PickTask picktask( linesegment, this );

            Geoweb3d::GW3DPoint ray_start;
            Geoweb3d::GW3DPoint ray_stop;
            camera_controller_->get_Ray(win_event.MouseButton.x, win_event.MouseButton.y, /*out*/ ray_start,  /*out*/ ray_stop );

            //we want the ray to start from our eye(camera), but we could use the ray start which would have an offset
            linesegment.lock()->put_StartEnd( *camera_controller_->get_Location(), ray_stop );

            linesegment.lock()->put_Enabled ( true ); //only do a one shot

        }

        break;
        case win_event.MouseButtonDBLClick:
        {
            if(camera_animation_timer_ < 1)
            {
                printf("Dbl Click Camera Animation movement ignored, as last animation tasks is not completed\n");
            }
            else
            {
                if(!pcamera_task_)
                {
                    pcamera_task_ = new CameraMoveToAnimationTask ( camera_.lock()->get_WindowCoordinateToGeodeticQueryCollection()->create(), this );
                    pcamera_task_->query_.lock()->put_WindowCoordinate( win_event.MouseButton.x, win_event.MouseButton.y );
                    frame_tasks_.push_back( pcamera_task_ );
                }
                else
                {
                    pcamera_task_->ResetQuery( win_event.MouseButton.x, win_event.MouseButton.y );
                }


                printf("Dbl Click Camera Animation Started!\n");
            }
		}

        break;

        case win_event.KeyPressed:

            if(!camera_.expired())
            {
                float heading, pitch, roll;
                camera_controller_->get_Rotation( heading, pitch, roll );

                switch(	win_event.Key.code )
                {
                    //Move forward
                case Geoweb3d::Key::W:
                    //moveCamForward
                    break;

                    //Rotate right (yaw)
                case Geoweb3d::Key::D:
                    if(win_event.Key.shift)
                    {
                        heading+=1.f;
                    }
                    else
                    {
                        heading-=1.f;
                    }
                    //rotCamHorizontal
                    break;

                    //Rotate up (pitch)
                case Geoweb3d::Key::Q:
                    //rotCamVertical
                    if(win_event.Key.shift)
                    {
                        pitch+=1.f;
                    }
                    else
                    {
                        pitch-=1.f;
                    }
                    break;
                    //Strafe right
                case Geoweb3d::Key::X:
                    //moveCamRight
                    break;

				 case Geoweb3d::Key::S:
					 {
						 Geoweb3d::IGW3DCameraStereoSettings *settings = camera_.lock()->get_CameraStereoSettings();
						 //just toggle stereo on/off
						 settings->put_Enable( !settings->get_Enable() );
					 }
                }


                camera_controller_->put_Rotation(heading,pitch, roll );

            }


            break;
        default:
            break;
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
    friend class CameraMoveToAnimationTask;

    /// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    /// <summary>	The window. </summary>
    Geoweb3d::IGW3DWindowWPtr window_;
    /// <summary>	The camera. </summary>
    Geoweb3d::IGW3DCameraWPtr camera_;
    /// <summary>	The camera controller. </summary>
    Geoweb3d::IGW3DCameraController *camera_controller_;

	/// <summary>	The camera animation timer. </summary>
    float camera_animation_timer_;

    /// <summary>	experimental linearslerp for moving around.  Use at your own risk. </summary>
	Geoweb3d::GW3DPoint  camera_start_location;

	Geoweb3d::GW3DPoint camera_animation_target;

    /// <summary>	The dynamic features datasource. </summary>
    Geoweb3d::IGW3DVectorDataSourceWPtr  dynamic_features_datasource;
    /// <summary>	The people layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr people_layer;
    /// <summary>	The model representation. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr  modelrepresentation;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// Geoweb3d::IGW3DVectorLayerWPtr light_layer;
    /// Geoweb3d::IGW3DVectorRepresentationWPtr lightpoint_representation_;
    /// Geoweb3d::IGW3DVectorRepresentationWPtr extruded_representation_;
    /// Geoweb3d::IGW3DVectorRepresentationWPtr custom_representation_;
    /// </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    std::deque<PrePostDrawTasks*> frame_tasks_;
    /// <summary>	The pcamera task. </summary>
    CameraMoveToAnimationTask *pcamera_task_;

}; //engine end of class

/////////////////////////////////////////////////////////////////////////////
//					 CameraMoveToAnimationTask::RunPostDraw
/////////////////////////////////////////////////////////////////////////////
void CameraMoveToAnimationTask::RunPostDraw()
{
    if(query_.expired()) return;

    if(query_.lock()->get_Enabled() && query_.lock()->get_IsValid())
    {
        Geoweb3d::GW3DPoint cloc =  *pengine_->camera_controller_->get_Location( );

        //you set the end location, as this is where you want to end up when
        //the animation is completed

		pengine_->camera_animation_target = *query_.lock()->get_GeodeticResult();

		pengine_->camera_animation_timer_ = 0; //this sets us off!

    }

	Geoweb3d::GW3DPoint loc  =  *pengine_->camera_.lock()->get_CameraController()->get_Location();

    const double millimeter	=   0.001;
    const double centimeter	=   0.010;
    const double decimeter	=   0.100;
    const double meter		=   1.000;
    const double dekameter  =  10.000;
    const double hectometer = 100.000;
    const double kilometer	=1000.000;

    double multipiler = 0;
    double abs_altitude = abs(loc.get_Z());

    if(abs_altitude > kilometer)
    {
        multipiler = pengine_->linearInterpolation(kilometer, abs_altitude * 0.5,abs_altitude );
    }
    else if(abs_altitude > hectometer)
    {
        multipiler = pengine_->linearInterpolation(kilometer, hectometer, abs_altitude )/hectometer;
    }
    else if(abs_altitude  > dekameter)
    {
        multipiler = pengine_->linearInterpolation(hectometer,dekameter, abs_altitude )/dekameter;
    }
    else if(abs_altitude  > meter)
    {
        multipiler = pengine_->linearInterpolation( dekameter,meter,abs_altitude )/meter;
    }
    else if(abs_altitude > decimeter)
    {
        multipiler = pengine_->linearInterpolation( meter,decimeter,abs_altitude )/decimeter;
    }
    else if(abs_altitude > centimeter)
    {
        multipiler = pengine_->linearInterpolation( decimeter,centimeter,abs_altitude )/centimeter;
    }
    else//(abs_altitude > millimeter)
    {
        multipiler = pengine_->linearInterpolation(centimeter,millimeter, abs_altitude )/millimeter;
    }


    double distance =  (float)(animation_base_speed_ * multipiler);

    if(pengine_->camera_animation_timer_ < 1) //we know the animation was completed as a time 0.0 to 1.0 where 1 is completed (end position)
    {
        pengine_->camera_animation_timer_ += (1.f/60.f); 
		pengine_->camera_controller_->move_Toward( distance, &pengine_->camera_animation_target);
    }


    if(!query_.expired())
    {
        query_.lock()->put_Enabled(false);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					RepresentAsCustom
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
/// <param name="layer">	  	The layer. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsCustom( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer)
{

    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "Custom" );

    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representation
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a custom property: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name  );
        }


        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					RepresentAsLights
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
/// <param name="layer">	  	The layer. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsLights( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer )
{
    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "LightPoint" );

    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representation
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a lightpoint property: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name  );
        }


        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					RepresentAsExtruded
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
/// <param name="layer">	  	The layer. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsExtruded( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer )
{

    Geoweb3d::IGW3DVectorRepresentationDriverWPtr driver = sdk_context->get_VectorRepresentationDriverCollection()->get_Driver( "ExtrudedPolygon" );


    Geoweb3d::GW3DResult validitychk;

    if(!Geoweb3d::Succeeded( validitychk =  driver.lock()->get_CapabilityToRepresent( layer ) ))
    {
        printf("Asked to represent a vector layer with a representation, but the layer is not compatible!\n");
    }
    else
    {
        // prints the values the layers has to help the user know what they can attribute map into the representation
        const Geoweb3d::IGW3DDefinitionCollection* def = layer.lock()->get_AttributeDefinitionCollection();

        printf("Attribute Names within the layer: [%s] you can map to the a extruded polygon property: \n", layer.lock()->get_Name() );
        for(unsigned i = 0; i < def->count(); ++i )
        {
            printf("\t[%s]\n", def->get_AtIndex(i)->property_name );
        }

        //good to go!
        Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
        return driver.lock()->get_RepresentationLayerCollection()->create( layer, params );
    }

    return Geoweb3d::IGW3DVectorRepresentationWPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					UpdateDefaultPolygonExtrusion
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="polygonrepresentation">	The polygon representation. </param>
/// <param name="new_height">				Height of the new. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void UpdateDefaultPolygonExtrusion(Geoweb3d::IGW3DVectorRepresentationWPtr polygonrepresentation, double new_height)
{
    //variants are expensive to create and destroy, so reuse the object when you can.
    static Geoweb3d::IGW3DVariant scratchpad;

    scratchpad = new_height;

    //here just to show you that you can use a const raw pointer if you know you do not plan to change something
    if(!polygonrepresentation.expired())
    {
		Geoweb3d::IGW3DPropertyCollectionPtr properties = polygonrepresentation.lock()->get_PropertyCollection()->create_Clone();
		properties->put_Property( Geoweb3d::Vector::ExtrudedPolygonProperties::EXTRUSION_HEIGHT,scratchpad );
        polygonrepresentation.lock()->put_PropertyCollection( properties );
    }
    else
    {
        printf("tried to update extruded, put the representation has expired?\n");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					CollisionPrinter
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class CollisionPrinter 
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Default constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CollisionPrinter() {};

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~CollisionPrinter() {};

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Prints the given linesegments. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="linesegments">	[in,out] If non-null, the linesegments. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void  Print( Geoweb3d::IGW3DLineSegmentIntersectionTestResultsCollection *linesegments )
    {
        if(linesegments->count())
        {
            //create GW3DPoint objects once, as they are expensive to have in a loop!
            Geoweb3d::GW3DPoint vert_a;
            Geoweb3d::GW3DPoint vert_b;
            Geoweb3d::GW3DPoint vert_c;

            printf("\n*** Intersection Detected! Linesegment Count:[ %d ]***\n", linesegments->count() );

            //NOTE, OnStream gives the results of all the new collisions, but nothing is stopping you to
            //query the results directly. *However*, calling STREAM is what runs the analysis within the SDK!
            //What this means is, if you are not interested the way to get spatially all the linesegment results,
            //you can just call stream leaving this function empty, and then call get_IntersectionCollection
            //on your linesegment of interest.
            Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr linesegment;

            linesegments->reset();

            while(linesegments->next( &linesegment))
            {
		      Geoweb3d::IGW3DLineSegmentIntersectionReportCollection *intersectioninfo =	linesegment.lock()->get_IntersectionReportCollection();

                Geoweb3d::IGW3DLineSegmentIntersectionReport *intersection;

                intersectioninfo->reset();

                printf("\n*** Representation Intersection Count:[ %d ]***\n", intersectioninfo->count() );

                while(intersectioninfo->next( &intersection ))
                {
                    printf("Representation Driver[%s] vector layer[%s]\n",
                           intersection->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name(),
                           intersection->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name() );

                    Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection *details_collection =
                        intersection->get_IntersectionDetailCollection();

                    Geoweb3d::IGW3DLineSegmentIntersectionDetail *intersection_detail;


                    printf("\n***Intersection within the Representation Count:[ %d ]***\n", details_collection->count() );

                    details_collection->reset();
                    details_collection->do_Sort(  Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection::Ascending );
                    while(details_collection->next( &intersection_detail ))
                    {

                        unsigned object_id =  intersection_detail->get_ObjectID();

						const  Geoweb3d::GW3DPoint * collision_point = intersection_detail->get_IntersectionPoint();
                        double intersectiondistance = intersection_detail->get_IntersectionDistance();

                        //the triangle at this point.  This is the real triangle of the geometry, and is not clipped or modified in any way.
                        intersection_detail->get_TriangleIntersected( vert_a, vert_b, vert_c );

                        //the normal at this point
                        const  Geoweb3d::GW3DPoint *normal = intersection_detail->get_NormalOfIntersection( );

                        printf("ObjectID[%d]\n", object_id);
                        printf("Intersection Distance[%f]\n", intersectiondistance);
                        printf("Intersection longitude[%f] latitude[%f] elevation[%f]\n",
                               collision_point->get_X(),collision_point->get_Y(),collision_point->get_Z());

                        printf("Triangle:\nlongitude[%f] latitude[%f] elevation[%f]\nlongitude[%f] latitude[%f] elevation[%f]\nlongitude[%f] latitude[%f] elevation[%f]\n",
                               vert_a.get_X(),vert_a.get_Y(),vert_a.get_Z(),
                               vert_b.get_X(),vert_b.get_Y(),vert_b.get_Z(),
                               vert_c.get_X(),vert_c.get_Y(),vert_c.get_Z());

                        printf("Normal[%f] [%f] [%f]\n",
                               normal->get_X(),normal->get_Y(),normal->get_Z());
                    }
                }
            }

            printf("*** Intersection Detected INFO END! ***\n\n");
        }//end if anything intersected
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// /
/// 					PrintLineSegmentCollisions
/// /.
/// </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="collection">	[in,out] If non-null, the collection. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintLineSegmentCollisions( Geoweb3d::IGW3DLineSegmentIntersectionTestCollection *collection)
{
    CollisionPrinter prints;
    prints.Print( collection->get_IntersectionResultsCollection() );
};

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
        //go back to using the date and time of this computer
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

    if(pengine->LoadConfiguration("Example 10 - Moving Representations"))
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
