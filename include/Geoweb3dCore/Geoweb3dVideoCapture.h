//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Geoweb3d/core/GW3DInterFace.h>
#include <Geoweb3dCore/Geoweb3dTypes.h>

/* Primary namespace */
namespace Geoweb3d
{

	typedef WebViewSourceHandle_Ptr WebViewSourceHandle;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	An webview window callback. </summary>
	///
	/// <remarks>	Experimental ONLY. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DWebViewWindowCallback
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the title change event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnTitleChange(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the before resource load event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnBeforeResourceLoad(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the before plugin load event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnBeforePluginLoad(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the render process terminated event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnRenderProcessTerminated(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the before popup event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnBeforePopup(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the before download event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnBeforeDownload(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the download updated event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDownloadUpdated(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the load start event. </summary>
		/// 
		/// <param name="browser">	The browser. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnLoadStart(WebViewSourceHandle browser) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the load end event. </summary>
		///
		/// <param name="browser">	The browser. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnLoadEnd(WebViewSourceHandle browser) = 0;
	};
	//typedef HWND     Window;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Web initialize. </summary>
	///
	/// <remarks>	Experimental ONLY. </remarks>
	///
	/// <param name="locales">	The locales. </param>
	/// <param name="pak">	  	The pak. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL void Web_Initialize(const char *locales, const char *pak);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Web shutdown. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL void Web_Shutdown();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a web view window. </summary>
	///
	/// <remarks>	Experimental ONLY. </remarks>
	///
	/// <param name="hWndParent"> 	The window parent. </param>
	/// <param name="webviewname">	The webview name. </param>
	/// <param name="pCallback">  	[in] If non-null, the callback. </param>
	/// <param name="nWidth">	  	The width. </param>
	/// <param name="nHeight">	  	The height. </param>
	///
	/// <returns>	The new webview window. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL WebViewSourceHandle create_WebViewWindow( OSWinHandle hWndParent, const char *webviewname, IGW3DWebViewWindowCallback *pCallback, int nWidth,  int nHeight );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets an image pointer for a web view. </summary>
	///
	/// <remarks>	Experimental ONLY, for displaying the web view as a texture on a representation. </remarks>
	///
	/// <param name="device">	 	The web view. </param>
	/// <param name="collection">	[in] The collection to which the image should be added. </param>
	/// <param name="result">	 	[out] The result. </param>
	///
	/// <returns>	The web view. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL const IGW3DImageWPtr get_WebView( WebViewSourceHandle device,  IGW3DImageCollectionPtr &collection, GW3DResult &result );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Closes web view window. </summary>
	///
	/// <param name="device">	[in,out] The device. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL void close_WebViewWindow(WebViewSourceHandle &device);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Set the web view URL. </summary>
	///
	/// <param name="device">	The web view. </param>
	/// <param name="url">   	URL of the document. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL void put_WebViewWindow_URL(WebViewSourceHandle device, const char *url);

//SKIP-CODE-BEGIN

	//mouse keybrd *note, you will get link errors if you use any of the below right now. .they are placeholders
//	GW3D_DLL void put_WebViewWindow_JS_Call(WebViewSourceHandle device, const char *name);
//
//	GW3D_DLL void get_WebViewWindow_ToScreen(WebViewSourceHandle device, int &w, int &h);
//	GW3D_DLL void put_WebViewWindow_Focus(WebViewSourceHandle device, bool focus);
//
//	GW3D_DLL void inject_WebViewWindow_MouseMove(WebViewSourceHandle device, int x, int y);
//	GW3D_DLL void inject_WebViewWindow_MouseWheel(WebViewSourceHandle device, int x, int y);
//	GW3D_DLL void inject_WebViewWindow_MouseButton(WebViewSourceHandle device, int kind, bool up);
//	GW3D_DLL void inject_WebViewWindow_Key(WebViewSourceHandle device, int scancode, int asymb, const char *uni, int unilen, bool up);
//;

	/////////////////////
	//WebBrowser End
	/////////////////////

//SKIP-CODE-END

//SKIP-CODE-BEGIN

	/////////////////////
	//IP CAMERA POC START
	/////////////////////

//SKIP-CODE-END

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates an IP camera. </summary>
	///
	/// <param name="cameraName">	Name of the camera. </param>
	/// <param name="handle">	 	[out] The handle. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL void create_Camera( const char* cameraName, IPCameraSourceHandle &handle );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Destroys the camera described by handle. </summary>
	///
	/// <param name="handle">	[in,out] The handle. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL void destroy_Camera( IPCameraSourceHandle &handle );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets image object for an IP Camera. </summary>
	///
	/// <remarks>	If called multiple times with the same handle and collection, this function will
	///	 			return a pointer to the same image.  This image can be used as a texture for
	///				representations such as planes and projected images. </remarks>
	///
	/// <param name="device">	 	The device. </param>
	/// <param name="collection">	[in] The collection to which the image should be added. </param>
	/// <param name="result">	 	[out] The result. </param>
	///
	/// <returns>	The image. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL const IGW3DImageWPtr get_Image_IP( IPCameraSourceHandle device,  IGW3DImageCollectionPtr &collection, GW3DResult &result );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Connect to camera. </summary>
	///
	/// <param name="handle">	The handle. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL void put_Connect( IPCameraSourceHandle handle );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Disconnect from camera. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_Disconnect( IPCameraSourceHandle handle );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Wait for disconnect. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void wait_ForDisconnect(IPCameraSourceHandle handle );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the number frames received. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The number of frames received. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL unsigned long get_NumFramesReceived(IPCameraSourceHandle handle); 

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the number of bytes received. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The number of bytes received. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL unsigned long get_NumBytesReceived( IPCameraSourceHandle handle );  

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets bit rate. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The bit rate. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL float get_BitRate(IPCameraSourceHandle handle );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets camera name. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	the camera name. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL const char* get_CameraName( IPCameraSourceHandle handle );

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets the URI. </summary>
    ///
    /// <remarks>	Sets the URI of the IP camera. </remarks>
    ///
    /// <param name="handle">	The handle. </param>
    /// <param name="uri">		The URI. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_URI(IPCameraSourceHandle handle, const char* uri);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the URI. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	null if it fails, else the URI. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL const char* get_URI(IPCameraSourceHandle handle );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the host segment of the URI. </summary>
	///
	/// <param name="handle">	The handle. </param>
	///
	/// <returns>	null if it fails, else the host. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL const char* get_Host(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets a query segment of the URI. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	null if it fails, else the query. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL const char* get_Query(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the port segment of the URI. </summary>
    /// 
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The port. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL int get_Port(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the fragment segment of the URI. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	null if it fails, else the fragment. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL const char* get_Fragment(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets a cookie. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    /// <param name="key">   	The key. </param>
    /// <param name="value"> 	The value. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_Cookie(IPCameraSourceHandle handle, const char* key, const char* value);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Erase a cookie. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    /// <param name="key">   	The key. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void erase_Cookie(IPCameraSourceHandle handle, const char* key);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets a cookie value. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    /// <param name="key">   	The key. </param>
    ///
    /// <returns>	null if it fails, else the cookie. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL const char* get_Cookie(IPCameraSourceHandle handle, const char* key);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets the username for the IP camera. </summary>
    ///
    /// <param name="handle">  	The handle. </param>
    /// <param name="username">	The username. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_Username(IPCameraSourceHandle handle, const char* username);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets the password for the IP camera. </summary>
    ///
    /// <param name="handle">  	The handle. </param>
    /// <param name="password">	The password. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_Password(IPCameraSourceHandle handle, const char* password);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the username. </summary>
	///
	/// <param name="handle">	The handle. </param>
	///
	/// <returns>	null if it fails, else the username. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL const char* get_Username(IPCameraSourceHandle handle);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the password. </summary>
	///
	/// <param name="handle">	The handle. </param>
	///
	/// <returns>	null if it fails, else the password. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	GW3D_DLL const char* get_Password(IPCameraSourceHandle handle);

	// proxy server

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets whether to use a proxy server. </summary>
    /// 
	/// <remarks>	The proxy details are setup via another API where if set up, we will use it by 
	/// 			default. You can turn it off here. </remarks>
    ///
    /// <param name="handle">  	The handle. </param>
    /// <param name="useProxy">	true to use proxy. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_UseProxy(IPCameraSourceHandle handle, bool useProxy);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets whether proxy is used. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	true if used, false if not used. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL bool get_UseProxy(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Query if IP Camera is connected. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	true if connected, false if not. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL bool is_Connected(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Query if IP Camera connection has failed. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	true if connection failed, false if not. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL bool has_ConnectionFailed(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets reconnect timeout. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    /// <param name="ms">	 	The timeout in milliseconds. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_ReconnectTimeout(IPCameraSourceHandle handle, unsigned long ms);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets reconnect timeout. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The reconnect timeout in milliseconds. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL unsigned long get_ReconnectTimeout(IPCameraSourceHandle handle); 

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Query if needs reconnect. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	true if it needs reconnect, false if not. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL bool get_NeedsReconnect(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets automatic reconnect. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	true if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL bool get_AutoReconnect(IPCameraSourceHandle handle) ;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets reconnect count. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The reconnect count. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL  unsigned long get_ReconnectCount(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets maximum reconnects. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The maximum reconnects. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL unsigned long get_MaxReconnects(IPCameraSourceHandle handle);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets maximum reconnects. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    /// <param name="num">   	Number of reconnects allowed. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_MaxReconnects(IPCameraSourceHandle handle, unsigned long num);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets automatic retry delay. </summary>
    ///
    /// <returns>	The automatic retry delay. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL unsigned long get_AutoRetryDelay(); 

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Sets automatic retry delay. </summary>
    ///
    /// <param name="handle">  	The handle. </param>
    /// <param name="delay_ms">	The delay in milliseconds. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL void put_AutoRetryDelay(IPCameraSourceHandle handle, unsigned long delay_ms);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets the next automatic retry time. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The next automatic retry time. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL unsigned long get_NextAutoRetryTime(IPCameraSourceHandle handle ); 

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Gets time until next automatic retry. </summary>
    ///
    /// <param name="handle">	The handle. </param>
    ///
    /// <returns>	The time until next automatic retry. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GW3D_DLL unsigned long get_TimeTillNextAutoRetry(IPCameraSourceHandle handle); 

//SKIP-CODE-BEGIN

	/////////////////////
	//IP CAMERA POC END
	/////////////////////

	/** @namespace Geoweb3d::VideoCapture 
	*	@brief Functions for controlling video capture devices
	*
	*	(For use with Plane representations)
	*
	*	Example Usage:
	*
	* ScanVideoDevices : Lists available devices and returns num of devices found.  Note, if u plugin a USB camera for example, call this to rescan and find it
	* GetDeviceFriendlyName: Given the index of cameras created from ScanVideoDevices, u can get a human readable name of the device
	*
	* int device1 = 0;  //this could be any deviceID that shows up in list Devices generated from ScanVideoDevices
	* int device2 = 1;  //this could be any deviceID that shows up in list Devices generated from ScanVideoDevices
	*
	* SetIdealFramerate Given the index of cameras created from ScanVideoDevices, if you want to capture at a different frame rate (default is 30) specify it here,
	* you are not guaranteed to get this fps though.
	*
	* setup the first device - there are a number of options:
	* SetupDevice(device1); 					// setup the first device with the default settings
	* SetupDevice(device1, GW_COMPOSITE); 		// or setup device with specific connection type
	* SetupDevice(device1, 320, 240);			// or setup device with specified video size
	* SetupDevice(device1, 320, 240, GW_COMPOSITE); //or setup device with video size and connection type
	* SetFormat(device1, GW_NTSC_M);			// if your card doesn't remember what format it should be
	*											// call this with the appropriate format listed above
	*											// NOTE: must be called after setupDevice!
	*
	* optionally setup a second (or third, fourth ...) device - same options as above
	* SetupDevice(device2); 						  
	*
	* to get a settings dialog for the device
	* ShowSettingsWindow(device1);
	*
	* Shut down devices properly
	* StopDevice(device1);
	* StopDevice(device2);
	*
	* As requested width and height can not always be accommodated, 
	* make sure to check the size once the device is set up!
	*/

//SKIP-CODE-END

	/// Functions for controlling video capture devices
	namespace VideoCapture
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Information about the system capture device. </summary>
		///
		/// <see cref="ScanVideoDevices."/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		struct SystemCaptureDeviceInfo
		{
			unsigned number_of_devices;
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Connection types for SetupDevice. </summary>
		///
		/// <remarks>	Tuner is not as well supported as composite and s-video. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum PhysicalConnection
		{
			GW_COMPOSITE = 0,
			GW_S_VIDEO  = 1,
			GW_TUNER    = 2,
			GW_USB      = 3,
			GW_1394		= 4,
			ENDPC		= 0xFF
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Video format types for SetFormat. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum VideoFormats
		{
			GW_NTSC_M=	0,
			GW_PAL_B=	1,
			GW_PAL_D=	2,
			GW_PAL_G=	3,
			GW_PAL_H=	4,
			GW_PAL_I=	5,
			GW_PAL_M=	6,
			GW_PAL_N=	7,		 
			GW_PAL_NC=	8,
			GW_SECAM_B=	9,
			GW_SECAM_D=	10,
			GW_SECAM_G=	11,
			GW_SECAM_H=	12,
			GW_SECAM_K=	13,
			GW_SECAM_K1=14,
			GW_SECAM_L=	15,
			GW_NTSC_M_J=16,
			GW_NTSC_433=17,
			ENDVT = 0xFF
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets an image object for a video device. </summary>
		///
		/// <remarks>	If called multiple times with the same device index and collection, this function will
		/// 			return a pointer to the same image.  This image can be used as a texture for
		/// 			representations such as planes and projected images. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="collection">	[in] The collection to which the image should be added. </param>
		/// <param name="result">	 	[out] The result. </param>
		///
		/// <returns>	The image. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL const IGW3DImageWPtr get_Image( unsigned device_index,  IGW3DImageCollectionPtr &collection, GW3DResult &result );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Scan the local machine for video capture devices. </summary>
		///
		/// <remarks>	ScanVideoDevices must be called prior to any other functions in the VideoCapture
		/// 			namespace. </remarks>
		///
		/// <param name="system_info">	[out] Information about video capture devices. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult ScanVideoDevices( /*out*/SystemCaptureDeviceInfo &system_info);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets device friendly name. </summary>
		///
		/// <remarks>	This must be called after ScanVideoDevices, otherwise returns NULL. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		///
		/// <returns>	null if it fails, else the device friendly name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL const char	*GetDeviceFriendlyName(unsigned device_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets device path. </summary>
		///
		/// <remarks>	Query the unique path of a video capture device. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		///
		/// <returns>	null if it fails, else the device path. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL const char	*GetDevicePath(unsigned device_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the ideal framerate. </summary>
		///
		/// <remarks>	This function sets the ideal video framerate for a video capture device. 
		/// 			DirectShow will try and maintain the closest possible framerate to the one specified. </remarks>
		///
		/// <param name="device_index">  	Zero-based index of the device. </param>
		/// <param name="idealFramerate">	The ideal framerate. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult SetIdealFramerate( unsigned device_index, int idealFramerate );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets up the device. </summary>
		///
		/// <remarks>	Set up a device for use in the Geoweb3d SDK. 
		/// 			
		/// 			The device will start its streaming behind the scenes.  If you do not plan on using 
		/// 			a particular device, do not call SetupDevice on it, or at least stop the device 
		/// 			after you set it up. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL	GW3DResult SetupDevice(unsigned device_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets up the device. </summary>
		///
		/// <remarks>	Set up a device for use in the Geoweb3d SDK.
		///
		/// 			The device will start its streaming behind the scenes.  If you do not plan on using
		/// 			a particular device, do not call SetupDevice on it, or at least stop the device
		/// 			after you set it up.</remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="w">		   	The width. </param>
		/// <param name="h">		   	The height. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL	GW3DResult SetupDevice(unsigned device_index, int w, int h);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets up the device. </summary>
		///
		/// <remarks>	Set up a device for use in the Geoweb3d SDK.
		///
		/// 			The device will start its streaming behind the scenes.  If you do not plan on using
		/// 			a particular device, do not call SetupDevice on it, or at least stop the device
		/// 			after you set it up.
		/// 			
		/// 			As requested width and height can not always be accommodated, make sure to check the 
		/// 			size once the device is set up.  Note: Do not specify the connection type for USB 
		/// 			and Firewire cameras.  This overload is only needed for capture cards. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="connection">  	The connection. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL	GW3DResult SetupDevice(unsigned device_index, PhysicalConnection connection);	

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets up the device. </summary>
		///
		/// <remarks>	Set up a device for use in the Geoweb3d SDK.
		///
		/// 			The device will start its streaming behind the scenes.  If you do not plan on using
		/// 			a particular device, do not call SetupDevice on it, or at least stop the device
		/// 			after you set it up.
		///
		/// 			Note: Do not specify the connection type for USB and Firewire cameras.  This 
		/// 			overload is only needed for capture cards.</remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="w">		   	The width. </param>
		/// <param name="h">		   	The height. </param>
		/// <param name="connection">  	The connection. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL	GW3DResult SetupDevice(unsigned device_index, int w, int h, PhysicalConnection connection); 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets up the device from video URL. </summary>
		///
		/// <remarks>	Set up a video file as a device for use in the Geoweb3d SDK.
		/// 			
		/// 			Setup a video device using a filepath of an uncompressed AVI.
		/// 			
		/// 			Note this is proof of concept and should not be used.  This will change! </remarks>
		///
		/// <param name="path">		   	Full pathname of the file. </param>
		/// <param name="device_index">	[in,out] Zero-based index of the device. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL	GW3DResult SetupDeviceFromVideoUrl(const char* path, unsigned& device_index); 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Specify the video format preference for a video capture device. </summary>
		///
		/// <remarks>	Calling this function is not necessarily required. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="format">	   	Describes the format to use. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		/// 
		/// <see> SetupDevice"/>
		///	<see> GetCurrentWidthAndHeight"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult SetFormat(unsigned device_index, VideoFormats format);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if device is set up. </summary>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		///
		/// <returns>	true if device set up, false if not. </returns>
		/// 
		/// <see> SetupDevice"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL	bool isDeviceSetup( unsigned device_index  );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Stops a device. </summary>
		///
		/// <remarks>	Completely stop and free a video capture device. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		/// 
		/// <see> RestartDevice"/>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult StopDevice( unsigned device_index );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Restart device. </summary>
		///
		/// <remarks>	Restart a video capture device while maintaining its existing settings. 
		/// 			
		/// 			Same as StopDevice, but then sets it up with the same settings.</remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		/// 
		/// <see cref="StopDevice"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult RestartDevice( unsigned device_index );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determine if we can step. </summary>
		///
		/// <remarks>	Check if device can step a number of frames equal to num_frames using the Step 
		/// 			function.
		/// 			
		/// 			If num_frames is 0 and method returns true, the device can step one frame at a 
		/// 			time. If num_frames is greater than zero and method returns true, the device can 
		/// 			step that many frames at a time. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="num_frames">  	Number of frames. </param>
		/// <param name="reserved">	   	Reserved for future use - set to zero. </param>
		///
		/// <returns>	true if we can step, false if not. </returns>
		/// 
		/// <see cref="Step"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL bool CanStep( unsigned device_index, int num_frames, int reserved );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Step a video device forward a given number of frames. </summary>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="num_frames">  	Number of frames to step forward. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		/// 
		/// <see cref="CanStep"/>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult Step( unsigned device_index,int num_frames );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Shows the settings window. </summary>
		///
		/// <remarks>	Launch the settings pop-up window for a video capture device. </remarks>
		///
		/// <param name="parent">	   	The parent window for the settings window. </param>
		/// <param name="device_index">	Zero-based index of the device. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult ShowSettingsWindow(OSWinHandle parent, unsigned device_index );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets current width and height. </summary>
		///
		/// <remarks>	Query the current native dimensions of the video stream from a video capture device.
		/// 			
		/// 			The returned values may be different than those provided via SetupDevice.  The 
		/// 			device may not always be able to accommodate the requested dimensions, and the true 
		/// 			final dimensions should be queried using this function. </remarks>
		///
		/// <param name="device_index">	Zero-based index of the device. </param>
		/// <param name="width">	   	[in,out] The width. </param>
		/// <param name="height">	   	[in,out] The height. </param>
		///
		/// <returns>	The current width and height. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult GetCurrentWidthAndHeight(unsigned device_index, unsigned &width, unsigned &height );

	}
}