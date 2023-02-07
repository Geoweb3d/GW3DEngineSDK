#pragma once
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DWindowCallback.h"
#include "Geoweb3dCore/SystemExports.h"
#include "../Common/NavigationHelper.h"
#include "../Common/MiniEngineCore.h"
#include "Geoweb3dCore/LayerParameters.h"
#include "DrapedRingController.h"
#include <deque>

struct PrePostDrawTasks;
class IntersectionTestHandler;
class FannedViewshedHandler;

void PrintAboutExample();
void PrintHelpMenu();
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DWindowCallback, MiniEngineCore
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="sdk_context">	Context for the sdk. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context);

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

	bool LoadConfiguration(const char* example_name);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Loads a data. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="rootpath">	The rootpath. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool LoadData(const char* rootpath);

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

	Geoweb3d::IGW3DWindowWPtr Create3DWindow(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* title, Geoweb3d::IGW3DWindowCallback* window_events = 0);

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

	Geoweb3d::IGW3DCameraWPtr CreateCamera(Geoweb3d::IGW3DWindowWPtr windowptr, const char* cameratitle);


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
	/// <summary>	Sets global light color. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetGlobalLightColor();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	call back for 2d drawing/client side. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="camera">	The camera. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera);

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

	void PopulateCurrentDrapedRingStateProps(Geoweb3d::IGW3DVectorRepresentationWPtr extruded_rep, const long current_fid);
	void PopulateCurrentDrapedRingDynamicStateProps(Geoweb3d::IGW3DVectorRepresentationWPtr extruded_rep, const long current_fid);

	void SetCurrentProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults);
	void SetDefaultProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults);

	void SetCurrentPropsDynamic(Geoweb3d::IGW3DPropertyCollectionPtr defaults);
	void SetDefaultPropsDynamic(Geoweb3d::IGW3DPropertyCollectionPtr defaults);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	OS event system, basically a helper off ProcessLowLevelEvent. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="win_event">	The window event. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void ProcessEvent(const Geoweb3d::WindowEvent& win_event);

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

	virtual LRESULT CALLBACK ProcessLowLevelEvent(OSWinHandle hWndParent, unsigned int message, WPARAM wParam, LPARAM lParam);


private:

	NavigationHelper* navHelper_;
	/// <summary>	Context for the sdk. </summary>
	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
	/// <summary>	The window. </summary>
	Geoweb3d::IGW3DWindowWPtr window_;
	/// <summary>	The camera. </summary>
	Geoweb3d::IGW3DCameraWPtr camera_;
	/// <summary>	The camera controller. </summary>
	Geoweb3d::IGW3DCameraController* camera_controller_;



	Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_;

	struct prop_state
	{
		Geoweb3d::Vector::OutlineMode outline_mode_;
		int ss_width_;
		float alpha_;
		float inner_rad_;
		float outer_rad_;
		float texture_blend_;
		float red_;
		float green_;
		float blue_;
		Geoweb3d::Vector::DrapedRingProperties::DrapedRingLightingMode lighting_;
	};

	bool props_dirty_;
	long current_fid_;
	int property_mode_; // determines which reps props we are currently working with

	prop_state default_state_draped_ring_prop_;
	prop_state specific_state_draped_ring_prop_;
	prop_state default_state_draped_ring_dynamic_prop_;
	prop_state specific_state_draped_ring_dynamic_prop_;


	/// <summary>	The extruded representation. </summary>
	Geoweb3d::IGW3DVectorRepresentationWPtr draped_ring_rep_;
	Geoweb3d::IGW3DVectorRepresentationWPtr draped_ring_dynamic_rep_;

	Geoweb3d::IGW3DPropertyCollectionPtr draped_ring_properties_;
	Geoweb3d::IGW3DPropertyCollectionPtr draped_ring_properties2_;

	Geoweb3d::IGW3DVectorDataSourceWPtr dynamic_ring_datasource_;
	Geoweb3d::IGW3DVectorLayerWPtr dynamic_ring_layer_;
	DrapedRingController dynamic_ring_stream_;

	int texture_propertycollection_id = -1;
	bool drag_mode_readied_ = false;
	bool working_new_feature = false;



}; //engine end of class