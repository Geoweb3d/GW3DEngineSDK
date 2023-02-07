
#include "../Common/NavigationHelper.h"
#include "../Common/IdentifyVector.h"
#include "../Common/MiniEngineCore.h"

#include "common/IGW3DString.h"
#include "common/IGW3DPropertyCollection.h"
#include "core/GW3DInterface.h"
#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DVector.h"

#include "GeoWeb3dCore/SystemExports.h"

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

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



Geoweb3d::IGW3DVectorLayerWPtr CreateDynamicdWayPoints(Geoweb3d::IGW3DGeoweb3dSDKPtr &sdk_context)
{
	Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

	if (driver.expired())
	{
		printf("Could  not find the driver requested!  For a list of supported drivers, run the Print Capabilites example");
		return Geoweb3d::IGW3DVectorLayerWPtr();
	}

	if (!driver.lock()->get_IsCapable(isCreateDataSourceSupported))
	{
		printf("This driver is not capable of creating new datasources!\n");
		return Geoweb3d::IGW3DVectorLayerWPtr();
	}


	Geoweb3d::IGW3DVectorDataSourceWPtr created_datasource = driver.lock()->get_VectorDataSourceCollection()->create("Dynamic_Features.geoweb3d");


	Geoweb3d::GeometryType geotype = Geoweb3d::gtLINE_STRING_25D;

	//Showing a little bit of attribute mapping from the datasource too.  So we create our custom datasource, where in the field
	//we add the texture to use when we represent this feature.
	Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
	field_definition->add_Property("counter time?", Geoweb3d::PROPERTY_DOUBLE, "just something to add.");

	// create some layers
	Geoweb3d::IGW3DVectorLayerWPtr waypoints = created_datasource.lock()->get_VectorLayerCollection()->create("WayPoints", geotype, field_definition);
	waypoints.lock()->put_GeometryEditableMode(true);

	// create some features in one of the layers
	Geoweb3d::IGW3DPropertyCollectionPtr field_values(field_definition->create_PropertyCollection());

	Geoweb3d::GW3DPoint pa(18.491890828685225, -33.945519782777708, 0.04950790405273);
	Geoweb3d::GW3DPoint pb(18.491890828685225, -33.945519782777708, 117.74950790405273);


	Geoweb3d::GW3DLineString linestring;

	linestring.add_Point(&pa);
	linestring.add_Point(&pb);

	return waypoints;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DEventStream, public Geoweb3d::IGW3DWindowCallback, public IdentifyVector, public MiniEngineCore
{
public:

	Geoweb3d::IGW3DCameraWPtr current_camera_;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <param name="sdk_context">	Context for the sdk. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context ) 
		: sdk_context_(sdk_context) 
		, nav_helper_ ( new NavigationHelper() )
		, IdentifyVector(sdk_context)
	{}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~MiniEngine()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Updates this object. </summary>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Update()
    {		
		if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
        {
			displayVectorRepresentationInterceptReport();

            return true;
        }

        //return that we did not draw. (app closing?)
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads a configuration. </summary>
    ///
    /// <param name="example_name">	Name of the example. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadConfiguration(const char *example_name )
    {
		window_ = create_Window( sdk_context_, example_name, this );
		if(window_.expired())return false;

		const float spacing = .001f;

		//change eventually.  This is why stereo is commented out.
		camera1_ = create_Camera(window_,"1 Camera");
		camera1_.lock()->put_NormalizeWindowCoordinates(0.f,			0.5f - spacing, 0.5f + spacing,	1.0f);
		camera1_.lock()->get_CameraController()->put_Elevation(1000.0);
		camera1_.lock()->get_CameraController()->put_Location(-77.045, 38.89);
		camera1_.lock()->get_CameraController()->put_Rotation(0, 90, 0);
		//bottom right corner
		camera2_ = create_Camera(window_,"2 Camera");
		camera2_.lock()->put_NormalizeWindowCoordinates(0.5f + spacing, 1.0f,			0.5f + spacing,	1.0f);
		camera2_.lock()->get_CameraController()->put_Elevation(1000.0);
		camera2_.lock()->get_CameraController()->put_Location(-77.035, 38.89);
		camera2_.lock()->get_CameraController()->put_Rotation(0, 90, 0);
		//upper left corner
		camera3_ = create_Camera(window_,"3 Camera");
		camera3_.lock()->put_NormalizeWindowCoordinates(0.0f,			0.5f - spacing, 0.0f,			0.5f - spacing);
		camera3_.lock()->get_CameraController()->put_Elevation(1000.0);
		camera3_.lock()->get_CameraController()->put_Location(-77.025, 38.89);
		camera3_.lock()->get_CameraController()->put_Rotation(0, 90, 0);
		//upper right corner
		camera4_ = create_Camera(window_,"4 Camera");
		camera4_.lock()->put_NormalizeWindowCoordinates(0.5f + spacing, 1.0f,			0.0f,			0.5f - spacing);
		camera4_.lock()->get_CameraController()->put_Elevation(1000.0);
		camera4_.lock()->get_CameraController()->put_Location(-77.015, 38.89);
		camera4_.lock()->get_CameraController()->put_Rotation(0, 90, 0);
		//Add cameras to the navigation helper
		nav_helper_->add_Camera(camera1_);
		nav_helper_->add_Camera(camera2_);
		nav_helper_->add_Camera(camera3_);
		nav_helper_->add_Camera(camera4_);		
		return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads the data. </summary>
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

		Geoweb3d::GW3DEnvelope envelope;

		if( CreateImageryRep( sdk_context_, imagerypath.c_str(), 6, 0, true, envelope ).expired() )
        {
            printf("CreateImageryRep Error\n");
			return false;
        }

		///////  Display a linestring as a ColoredLine

		//Vector Engine, Geoweb3d Datasource.  You could use KML/GML/GeoJSON etc as well.
		Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context_->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

		if (driver.expired())
		{
			printf("Could  not find the driver requested!  For a list of supported drivers, run the Print Capabilites example");
		}

		if (!driver.lock()->get_IsCapable(isCreateDataSourceSupported))
		{
			printf("This driver is not capable of creating new datasources!\n");
		}


		Geoweb3d::IGW3DVectorDataSourceWPtr created_datasource = driver.lock()->get_VectorDataSourceCollection()->create("Dynamic_Features.geoweb3d");


		Geoweb3d::GeometryType geom_type = Geoweb3d::gtLINE_STRING_25D;

		// Just showing how to create an attribute definition
		Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
		field_definition->add_Property("countertime", Geoweb3d::PROPERTY_DOUBLE, "just something to add.");

		// create some layers
		Geoweb3d::IGW3DVectorLayerWPtr waypoints = created_datasource.lock()->get_VectorLayerCollection()->create("WayPoints", geom_type, field_definition);
		//waypoints.lock()->put_GeometryEditableMode(true);


		//now add some features to the layer
		Geoweb3d::IGW3DPropertyCollectionPtr field_values(field_definition->create_PropertyCollection());

		//lets use the envelope of the imagery....fake some altitude
		Geoweb3d::GW3DPoint pa(envelope.MaxX, envelope.MaxY, 0.04950790405273);
		Geoweb3d::GW3DPoint pb(envelope.MinX, envelope.MinY, 117.74950790405273);

		Geoweb3d::GW3DLineString linestring;

		linestring.add_Point(&pa);
		linestring.add_Point(&pb);

		//just showing we can do other things.  Plus it makes the sphere/lights go along the line too.
		linestring.segmentize(.004);


		Geoweb3d::GW3DResult result;
		if (Geoweb3d::Succeeded(field_values->put_Property(field_definition->get_IndexByName("countertime"), 0.0)))
		{
			printf("Feature created with ID: %d \n", waypoints.lock()->create_Feature(field_values, &linestring, result));
		}
		else
		{
			printf("Failed to create feature!\n");
		}
	

		//now take the vector engine's layer "waypoints" and toss that to the 3D Engine world
		

		//the 3D map engine colored line driver
		{
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr repdriver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("ColoredLine");

			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			Geoweb3d::IGW3DVectorRepresentationWPtr rep = repdriver.lock()->get_RepresentationLayerCollection()->create(waypoints, params);
		}

		//Hey! lets add spheres to those endpoints as well...
		{
			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr repdriver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("Model");
			const Geoweb3d::IGW3DPropertyCollectionPtr properties = repdriver.lock()->get_PropertyCollection()->create_Clone();
			properties->put_Property(properties->get_DefinitionCollection()->get_IndexByName("URL"), "C:/6M Open Flight/6M Open Flight/6M.FLT");
			params.representation_default_parameters = properties; //note you can set specific properties after the fact too.
			Geoweb3d::IGW3DVectorRepresentationWPtr rep = repdriver.lock()->get_RepresentationLayerCollection()->create(waypoints, params);
			rep.lock()->get_ClampRadiusFilter()->put_Enabled(false);
		}


		//Lets load a shapefile
		{
			Geoweb3d::GW3DResult res;
			Geoweb3d::IGW3DVectorDataSourceWPtr data_source = sdk_context_->get_VectorDriverCollection()->auto_Open("../examples/media/Building Footprints.shp", res);
			Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
			Geoweb3d::IGW3DVectorRepresentationDriverWPtr repdriver = sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver("ExtrudedPolygon");

			Geoweb3d::IGW3DVectorRepresentationWPtr rep = repdriver.lock()->get_RepresentationLayerCollection()->create(
				data_source.lock()->get_VectorLayerCollection()->get_AtIndex(0), params);

		}

		//add all the representations to get added to the identify task
		addAllVectorRepresentationToTest();

        return true;
    }
private:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	call back for 2d drawing/client side. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="camera">	The camera. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void OnDraw2D( const Geoweb3d::IGW3DCameraWPtr &camera ){}
	virtual void OnCreate( ) {}
	virtual void OnDrawBegin( ) {}
	virtual void OnDrawEnd( ) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
    ///
    /// <param name="win_event">	The window event. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
    {		
		current_camera_ = nav_helper_->ProcessEvent(win_event, window_);	
		IdentifyVector::ProcessEvent(win_event, window_);
	}
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, raw data directly from the windows message pump. </summary>
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
    /// <summary> Pipeline messages </summary>
    ///
    /// <remarks> Because we are a globe and can load data everywhere, we are never *done*
    ///			  loading anything as if you move away and come back, its going to load the area again.
	///			  </remarks>
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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates window . </summary>
	///
	/// <param name="sdk_context">  	Context for the sdk. </param>
	/// <param name="title">			The title. </param>
	/// <param name="window_events">	(optional) [in,out] If non-null, the window events. </param>
	///
	/// <returns>	The new window creation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Geoweb3d::IGW3DWindowWPtr create_Window( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0 )
	{
		Geoweb3d::IGW3DWindowCollection* wcol = sdk_context->get_WindowCollection(  );

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
	/// <summary>	Creates a camera. </summary>
	///
	/// <param name="windowptr">  	The window. </param>
	/// <param name="cameratitle">	The camera title. </param>
	///
	/// <returns>	The new camera. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	 

	Geoweb3d::IGW3DCameraWPtr create_Camera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle )
	{
		Geoweb3d::IGW3DCameraCollection *cameracollection = windowptr.lock()->get_CameraCollection();
		Geoweb3d::IGW3DCameraWPtr camera =	cameracollection->create( cameratitle );

		return camera;
	}

private:

    /// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    /// <summary>	The window. </summary>
    Geoweb3d::IGW3DWindowWPtr window_;
	Geoweb3d::IGW3DCameraWPtr camera1_;
	Geoweb3d::IGW3DCameraWPtr camera2_;
	Geoweb3d::IGW3DCameraWPtr camera3_;
	Geoweb3d::IGW3DCameraWPtr camera4_;

	NavigationHelper* nav_helper_;
}; //engine end of class


///////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the application operation. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="sdk_context">	Context for the sdk. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{

    MiniEngine *pengine = new MiniEngine( sdk_context );

    if(pengine->LoadConfiguration("Navigation Example"))
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

		Geoweb3d::GW3DResult sdk_result = sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 );
        if( Geoweb3d::Succeeded( sdk_result ) )
        {
			if ( sdk_result == Geoweb3d::GW3D_sValidLicenseUnsupportedOpenGlVersion )
			{
				printf("\nSDK Context Created, but the system is not 3D capable! Press any key to continue... \n");
				getchar();
				return -2;
			}

			// The system supports 3D
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

void my_fatal_function(const char *msg)
{
    printf("Fatal Info: %s", msg);
}

/*! General info function, this will be used to print any general info and non-fatal warnings */
void my_info_function(const char *msg)
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