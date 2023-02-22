#include "../Common/NavigationHelper.h"
#include "../Common/MiniEngineCore.h"

#include "core/GW3DInterface.h"

#include "engine/IGW3DEventStream.h"
#include "common/IGW3DString.h"

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DVector.h"

#include "GeoWeb3dCore/SystemExports.h"

void my_fatal_function(const char *msg );
void my_info_function(const char *msg );
void SetInformationHandling();

Geoweb3d::IGW3DWindowWPtr CreateWindowCreation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events );
Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );

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
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DEventStream, public Geoweb3d::IGW3DWindowCallback, MiniEngineCore
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
		: sdk_context_(sdk_context) 
		, nav_helper_ ( new NavigationHelper() )
		, x_(0)
		, y_(0)
	{}

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

		static int last_mouse_X = 0;
		static int last_mouse_Y = 0;

		static Geoweb3d::IGW3DBearingHelperPtr bearinghelper = Geoweb3d::IGW3DBearingHelper::create();
		static Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr center_pixel_query;

		Geoweb3d::IGW3DCameraController* controller = camera1_.lock()->get_CameraController();

		if (x_ != 0 || y_ != 0)
		{
			if (!center_pixel_query.expired())
			{
				if (center_pixel_query.lock()->get_IsValid())
				{
					//What is the current range/bearing...

					//from camera
					bearinghelper->put_SourceLocation(controller->get_Location()->get_X(),controller->get_Location()->get_Y(),controller->get_Location()->get_Z());
					
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
					bearinghelper->put_SourceLocation(controller->get_Location()->get_X(),controller->get_Location()->get_Y(),controller->get_Location()->get_Z());

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

		if( sdk_context_->draw( window_ ) == Geoweb3d::GW3D_sOk )
        {
			if (center_pixel_query.expired())
			{
				//init
				center_pixel_query = camera1_.lock()->get_WindowCoordinateToGeodeticQueryCollection()->create();
				center_pixel_query.lock()->put_Enabled(true);
				int window_x, window_y;
				unsigned window_width, window_height;
				camera1_.lock()->get_Window().lock()->get_WindowSize(window_x, window_y, window_width, window_height);
				center_pixel_query.lock()->put_WindowCoordinate(window_width / 2, window_height / 2);
			}
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

		const float spacing = .001f;

		camera1_ = CreateCamera(window_,"1 Camera");
		camera1_.lock()->get_CameraController()->put_Elevation(1000.0);
		camera1_.lock()->get_CameraController()->put_Location(-77.0157765578, 38.8893656466);
		camera1_.lock()->get_CameraController()->put_Rotation(0, 90, 0);
		//Add camera to the navigation helper
		nav_helper_->add_Camera(camera1_);

		return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
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

        if( CreateImageryRep( sdk_context_, imagerypath.c_str(), 6, 0, true ).expired() )

        {
            printf("CreateImageryRep Error\n");
        }
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
	virtual void OnCreate( ) {};
	virtual void OnDrawBegin( ) {}; 
	virtual void OnDrawEnd( ) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="win_event">	The window event. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event )
	{		
		current_camera_ = nav_helper_->ProcessEvent(win_event, window_);	

		switch( win_event.Type)
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
	Geoweb3d::IGW3DCameraWPtr camera1_;
	NavigationHelper* nav_helper_;

	double x_;
	double y_;

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

    if(pengine->LoadConfiguration("Orbit Example"))
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