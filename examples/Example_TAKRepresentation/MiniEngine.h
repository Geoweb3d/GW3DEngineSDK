#pragma once
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DEventStream.h"
#include "engine/IGW3DWindowCallback.h"
#include "Geoweb3dCore/SystemExports.h"
#include "../Common/NavigationHelper.h"
#include "../Common/MiniEngineCore.h"
#include "Geoweb3dCore/LayerParameters.h"

#include "engine/IGW3DTAKClientCallback.h"
#include "engine/IGW3DTAKClientContext.h"
#include "engine/IGW3DFont.h"


#include <deque>
#include <atomic>
#include <array>

struct PrePostDrawTasks;
class IntersectionTestHandler;

namespace Geoweb3d
{
	class IGW3DTAKClientContext;
}

struct CommandLineArgs
{
	std::string	server_ip;
	std::string callsign;
	std::string user_name;
	std::string password;

	int server_port = -1;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Mini engine. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MiniEngine : public Geoweb3d::IGW3DWindowCallback
	, public Geoweb3d::IGW3DTAKClientCallback, MiniEngineCore
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
	///
	/// <param name="sdk_context">	Context for the sdk. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MiniEngine(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const CommandLineArgs& args );

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
	bool LoadTAK();
	bool UnLoadTAK();

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

	void OnDraw2D(const Geoweb3d::IGW3DCameraWPtr& camera, Geoweb3d::IGW3D2DDrawContext* drawcontext) override;
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


	//----Geoweb3d::IGW3DTAKClientCallback callback implementation.

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	 
	/// Invoked whenever a TAK contact is added or removed from the server.
	///</summary>
	/// <remarks>  
	/// </remarks>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////

	 void onLayerContactChanged(const Geoweb3d::IGW3DTAKClientContext* context, Geoweb3d::IGW3DCollection<long>& modified_features, Geoweb3d::IGW3DVectorLayer* layer) override;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	 
	/// Invoked whenever a TAK Cursor on Target(CoT) chat message is received.
	///</summary>
	/// <remarks>  
	/// </remarks>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////

	 void onChatMessageReceived(const Geoweb3d::IGW3DTAKClientContext* context, const char* sender_callsign, const char* sender_uid, const char* message) override;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Invoked whenever a file transfer to the client is initiated. 
	///</summary>
	/// <remarks>  
	/// Implementer should return true to confirm/accept the transfer, or false to cancel the transfer.
	/// </remarks>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	 bool onFileTransferRequested(const Geoweb3d::IGW3DTAKClientContext* context, const char* sender_callsign, const char* local_file_path ) override;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	 
	/// Invoked whenever an accepted file transfer is completed.
	///</summary>
	/// <remarks>  
	/// local_file_path is the absolute path the received mission package on the local file system.
	/// </remarks>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	 void onFileTransferCompleted(const Geoweb3d::IGW3DTAKClientContext* context, const char* local_file_path) override;

	 void onFileTransferFailed(const Geoweb3d::IGW3DTAKClientContext* context, const char* local_file_path, const Geoweb3d::GW3DResult reason) override;

	 void onFileTransferSendCompleted(const Geoweb3d::IGW3DTAKClientContext* context, const char* recipient_uid)override;

	 void onFileTransferSendFailed(const Geoweb3d::IGW3DTAKClientContext* context, const char* recipient_uid, const Geoweb3d::GW3DResult reason)override;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	 
	/// Invoked whenever connection to the server is active.
	///</summary>
	/// <remarks>  
	///	Communication with the server should only occur when the server connection is active. 
	/// </remarks>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	 void onServerConnectionActive(const Geoweb3d::IGW3DTAKClientContext* context) override;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Invoked whenever connection to the server is inactive or otherwise disabled.
	///</summary>
	/// <remarks>  
	///	Communication with the server should only occur when the server connection is active. 
	/// </remarks>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	 void onServerConnectionInActive(const Geoweb3d::IGW3DTAKClientContext* context ) override;

	 void onSANotificationBegin(const Geoweb3d::IGW3DTAKClientContext* context) override {}

	 void onSANotificationEnd(const Geoweb3d::IGW3DTAKClientContext* context) override {}

	 void onError( const Geoweb3d::IGW3DTAKClientContext* context, const Geoweb3d::GW3DResult error, const char* error_msg) override {}

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

	Geoweb3d::IGW3DVectorDriverWPtr				tak_vector_driver_;
	Geoweb3d::IGW3DVectorDataSourceWPtr			tak_vector_ds_;
	Geoweb3d::IGW3DPropertyCollectionPtr		data_driver_props_;
	Geoweb3d::IGW3DVectorRepresentationHelperCollectionPtr	rep_collect_;


	Geoweb3d::IGW3DTAKClientContext*	tak_client_context_ = nullptr;
	Geoweb3d::IGW3DFontPtr				font_;

	std::string							selected_contact_uid_;
	std::string							selected_contact_callsign_;

	std::string							chat_src_contact_uid_;
	std::string							chat_src_contact_callsign_;

	std::string							last_chat_message_;
	std::string							mp_filepath_;
	std::string							server_url_;

	std::deque< std::pair<bool, std::string > >	chat_msg_queue_;

	static const int NUM_CHAT_MESSAGE = 4;
	static const int NUM_CALLSIGNS	  = 4;
	static const int NUM_ROLES	      = 8;
	static const int NUM_TEAMS        = 14;

	std::array< std::string, NUM_CHAT_MESSAGE > chat_messages_;
	std::array< std::string, NUM_CALLSIGNS >	callsigns_;
	std::array< Geoweb3d::IGW3DTAKClientContext::Role, NUM_ROLES >		roles_;
	std::array< Geoweb3d::IGW3DTAKClientContext::Team, NUM_TEAMS >		teams_;

	int last_message_index_ = -1;
	int chat_message_index_ = -1;

	int callsign_index_		= 0;
	int role_index_			= 0;
	int team_index_			= 0;

	int fade_time_second_   = 5;
	int server_port_ = -1;

	bool								accept_file_transfers_			= true;

	std::atomic<bool>					tak_server_connection_active_	= true;

	std::atomic<bool>					file_download_in_progress_		= false;
	std::atomic<bool>					file_upload_in_progress_		= false;

	std::atomic<bool>					file_download_completed_ = false;
	std::atomic<bool>					file_upload_completed_ = false;
}; //engine end of class