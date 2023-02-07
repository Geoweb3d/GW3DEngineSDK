#pragma once
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DWindowCallback.h"
#include "../Common/NavigationHelper.h"
#include "engine/GW3DVector.h"
#include "SpatialDB.h"
//#include "IntersectionTestStream.h"

#include <deque>
struct PrePostDrawTasks;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DWindowCallback
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="sdk_context">	Context for the sdk. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Destructor. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual ~MiniEngine();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Updates this object. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Update();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Loads a configuration. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="example_name">	Name of the example. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool LoadConfiguration(const char *example_name);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Loads the data. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="rootpath">	The rootpath. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool LoadData(const char *rootpath);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a 3D window	</summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="sdk_context">  	Context for the sdk. </param>
	/// <param name="title">			The title. </param>
	/// <param name="window_events">	(optional) [in,out] If non-null, the window events. </param>
	///
	/// <returns>	The new window creation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Geoweb3d::IGW3DWindowWPtr Create3DWindow(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback *window_events = 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a camera. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="windowptr">  	The windowptr. </param>
	/// <param name="cameratitle">	The cameratitle. </param>
	///
	/// <returns>	The new camera. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Geoweb3d::IGW3DCameraWPtr CreateCamera(Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the camera. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <returns>	the camera. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Geoweb3d::IGW3DCameraWPtr GetCamera();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the pre draw work operation. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void DoPreDrawWork();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the post draw work operation. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void DoPostDrawWork();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets global light color. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetGlobalLightColor();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	call back for 2d drawing/client side. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
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

	virtual void OnCreate()
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Called just before drawing starts</summary>
	///
	/// <remarks>	If a client draws or uses the framebuffer at this point, it will get overwritten
	///             when the sdk starts to draw.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void OnDrawBegin()
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Called just after all active cameras have been drawn</summary>
	///
	/// <remarks>	Note, for same frame 2D drawing per camera, its suggested to use OnDraw2D
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void OnDrawEnd()
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="win_event">	The window event. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void ProcessEvent(const Geoweb3d::WindowEvent& win_event);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	OS event system, raw data directly from the windows message pump. </summary>
	///
	/// <remarks>	Geoweb3d, 4/7/2020. </remarks>
	///
	/// <param name="hWndParent">	The window parent. </param>
	/// <param name="message">   	The message. </param>
	/// <param name="wParam">	 	The wParam field of the message. </param>
	/// <param name="lParam">	 	The lParam field of the message. </param>
	///
	/// <returns>	. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual LRESULT CALLBACK ProcessLowLevelEvent( OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam);

	void SetExtrudedPolyDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults);
	void SetColoredLineDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults);
	void SelectById(long fid);
	void ClearSelected();

private:
	/// <summary>	Helper utility for 3D navigation. </summary>
	NavigationHelper* navHelper_;
	/// <summary>	Context for the sdk. </summary>
	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
	/// <summary>	The window. </summary>
	Geoweb3d::IGW3DWindowWPtr window_;
	/// <summary>	The camera. </summary>
	Geoweb3d::IGW3DCameraWPtr camera_;
	/// <summary>	The camera controller. </summary>
	Geoweb3d::IGW3DCameraController *camera_controller_;
	/// <summary>	The datasource bounds. </summary>
	Geoweb3d::GW3DEnvelope env_;
	/// <summary>	The datasource layer that works by with callbacks. </summary>
	Geoweb3d::IGW3DVectorLayerWPtr callback_layer_;
	/// <summary>	The extruded representation. </summary>
	Geoweb3d::IGW3DVectorRepresentationWPtr extruded_poly_rep_;
	Geoweb3d::IGW3DVectorRepresentationWPtr line_rep_;
	/// <summary>	Used as the ray for intersections tests in the scene. </summary>
	Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_;
	/// <summary>	Spatial database that the callback layer with query when requested. </summary>
	SpatialDB spatial_db_;
	// <summary>	Used to revert back to after a selection is over with
	//				If multiple selections are need than you can make a history for each
	//				feature inside the spatial_db class
	// </summary>
	Geoweb3d::IGW3DPropertyCollectionPtr previous_rep_props_;
	/// <summary> Attribute mapping helper </summary>
	ExtrudedFeatureConstructionCb attribute_mapping_callback_;

	long selected_fid_;

}; //engine end of class


