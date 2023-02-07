#pragma once
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DWindowCallback.h"
#include "Geoweb3dCore/SystemExports.h"
#include "../Common/NavigationHelper.h"
#include "../Common/MiniEngineCore.h"
#include "Geoweb3dCore/LayerParameters.h"
#include <deque>


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
	struct prop_state
	{
		float min_pt_size;
		float max_pt_size;
		float point_scale_dist_;
		Geoweb3d::Vector::PointCloudProperties::PointCloudRenderModes render_mode;
		float min_shade_height;
		float max_shade_height;
		float z_offset;
		Geoweb3d::Vector::PointCloudProperties::PointCloudPixelSizeModes pixel_mode;
		float min_node_size;
		int max_render_level;
		float user_min_spacing;
	};

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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Helper to populate the transistion properties of a selected feature.</summary>
	///
	void PopulateCurrentStateProps(const long current_fid);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Helper to grab the properties of the Feature in the selected state.</summary>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetProps(Geoweb3d::IGW3DPropertyCollectionPtr defaults, prop_state prop_s);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Represents the layer with the default properties.</summary>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Geoweb3d::IGW3DVectorRepresentationWPtr RepresentAsPointCloud(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DVectorLayerWPtr layer, Geoweb3d::IGW3DPropertyCollectionPtr defaults);

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

	/// <summary>	The extruded representation. </summary>
	Geoweb3d::IGW3DVectorRepresentationWPtr point_cloud_;
	Geoweb3d::IGW3DVectorRepresentationHelperCollectionPtr point_cloud_reps_;
	Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_;

	typedef struct
	{
		int c[32];
	} Classifications;
	Classifications classifications_;
	typedef struct
	{
		int r[8];
	} Returns;
	Returns returns_;

	bool props_dirty_;
	prop_state default_state_;
	prop_state specific_state_;
	long current_fid_;

	Geoweb3d::IGW3DPropertyCollectionPtr properties_;



}; //engine end of class