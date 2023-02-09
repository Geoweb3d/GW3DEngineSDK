#include "engine/IGW3DGeoweb3dSDK.h"

#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DWindowCallback.h"

#include "Geoweb3dCore/SystemExports.h"

#include "../Common/NavigationHelper.h"
#include "../Common/MiniEngineCore.h"

#include "engine/GW3DVector.h"

#include "SpotlightController.h"

#include <deque>

class IntersectionTestHandler;
class SpotlightControllerTask;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Pre post draw tasks. </summary>
///
/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrePostDrawTasks
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw() = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
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

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="sdk_context">	Context for the sdk. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ~MiniEngine();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Updates this object. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Update();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads a configuration. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="example_name">	Name of the example. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadConfiguration(const char *example_name );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Loads a data. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="rootpath">	The rootpath. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool LoadData( const char *rootpath );
   
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a 3D window	</summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="sdk_context">  	Context for the sdk. </param>
	/// <param name="title">			The title. </param>
	/// <param name="window_events">	(optional) [in,out] If non-null, the window events. </param>
	///
	/// <returns>	The new window creation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Geoweb3d::IGW3DWindowWPtr Create3DWindow( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0 );

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

	Geoweb3d::IGW3DCameraWPtr CreateCamera( Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle );


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the camera. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <returns>	the camera. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	Geoweb3d::IGW3DCameraWPtr GetCamera();

private:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw work operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DoPreDrawWork();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw work operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DoPostDrawWork();    

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	call back for 2d drawing/client side. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="camera">	The camera. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void OnDraw2D(const Geoweb3d::IGW3DCameraWPtr &camera);

	////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	GPU Context created</summary>
    ///
    /// <remarks>	The SDK calls the OnCreate callback once the rendering context is created.  
	///				This is a place clients can initialize their gpu specific items as well.
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void OnCreate( )
	{}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Called just before drawing starts</summary>
    ///
    /// <remarks>	If a client draws or uses the frame buffer at this point, it will get overwritten
    ///             when the sdk starts to draw.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void OnDrawBegin( )
	{}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Called just after all active cameras have been drawn</summary>
    ///
    /// <remarks>	Note, for same frame 2D drawing per camera, its suggested to use OnDraw2D
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	 virtual void OnDrawEnd( )
	 {}

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

    double linearInterpolation( double startvalue, double endvalue, double value);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ///
    /// <param name="win_event">	The window event. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void ProcessEvent( const Geoweb3d::WindowEvent& win_event );

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

    virtual LRESULT CALLBACK ProcessLowLevelEvent(OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam );


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

    virtual void OnPagingEvent( Geoweb3d::IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const Geoweb3d::GW3DEnvelope& bounds, unsigned int value );

    virtual void OnStereoEvent( Geoweb3d::IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if 'searchingfor' does string contain. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="searchingfor">	   	[in,out] The searchingfor. </param>
	/// <param name="stringofinterest">	[in,out] The stringofinterest. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool DoesStringContain( std::string &searchingfor,  std::string &stringofinterest );




private:


	NavigationHelper* navHelper_;
   
    /// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
    /// <summary>	The window. </summary>
    Geoweb3d::IGW3DWindowWPtr window_;
    /// <summary>	The camera. </summary>
    Geoweb3d::IGW3DCameraWPtr camera_;
    /// <summary>	The camera controller. </summary>
    Geoweb3d::IGW3DCameraController *camera_controller_;

    /// <summary>	The light layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr light_layer;    

    /// <summary>	The frame tasks. </summary>
    std::deque<PrePostDrawTasks*> frame_tasks_;

public:

	/// <summary>	The light point representation. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr spotlight_representation_;
    /// <summary>	The extruded representation. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr extruded_representation_;
    /// <summary>	The custom representation. </summary>
    Geoweb3d::IGW3DVectorRepresentationWPtr custom_representation_;
	

}; //engine end of class

///---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Spotlight controller task. </summary>
///
/// <remarks>	Geoweb3d, 06/13/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SpotlightControllerTask : public PrePostDrawTasks
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ///
    /// <param name="rep">	  	The rep. </param>
    /// <param name="pengine">	[in,out] If non-null, the pengine. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SpotlightControllerTask(Geoweb3d::IGW3DVectorRepresentationWPtr rep, MiniEngine *pengine) 
		: stream_ (rep), 
		pengine_(pengine),
		time_stamp_minutes_old_(0),
		time_stamp_seconds_old_(0)
    {
        layer_ = rep.lock()->get_VectorLayer();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ~SpotlightControllerTask() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the pre draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPreDraw( ) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Executes the post draw operation. </summary>
    ///
    /// <remarks>	Geoweb3d, 06/13/2016. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void RunPostDraw()
    {

		struct tm timeinfo;

		// Get the current time and date
		time( &rawtime );
		localtime_s( &timeinfo, &rawtime );

        // Toggle every five seconds or so
		if((timeinfo.tm_sec >= (time_stamp_seconds_old_ + 2))  
		||(timeinfo.tm_min > time_stamp_minutes_old_ && (timeinfo.tm_sec < (time_stamp_seconds_old_)))) 
        {
				layer_.lock()->Stream( &stream_ ); 
				stream_.FinalizeUtilityPoleSetup();

				// Place the current time in minutes and seconds into memory
				time_stamp_minutes_old_ = timeinfo.tm_min;
				time_stamp_seconds_old_ = timeinfo.tm_sec;
		}
    }

private:
    /// <summary>	The stream. </summary>
    SpotlightController stream_;
    /// <summary>	The layer. </summary>
    Geoweb3d::IGW3DVectorLayerWPtr layer_;
    /// <summary>	The pengine. </summary>
    MiniEngine *pengine_;
	/// <summary>   The time object. </summary>
	time_t rawtime;


	/// <summary> The minutes time stamp of the last post draw session </summary>
    int time_stamp_minutes_old_;

	/// <summary> The seconds time stamp of the last post draw session </summary>
	int time_stamp_seconds_old_;
};
