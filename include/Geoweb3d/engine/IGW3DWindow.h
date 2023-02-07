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
#include "../core/GW3DInterFace.h"
#include "../../Geoweb3dCore/Geoweb3dTypes.h"

//#ifndef _D3D9_H_
//typedef struct IDirect3DSurface9 *LPDIRECT3DSURFACE9, *PDIRECT3DSURFACE9;
//#endif

/* Primary namespace */
namespace Geoweb3d
{

#define GW3D_OVERLAPPED       0x00000000L	//defines the OVERLAPPED window style flag for create_3DWindow
#define GW3D_POPUP            0x80000000L	//defines the POPUP window style flag for create_3DWindow
#define GW3D_CHILD            0x40000000L	//defines the CHILD window style flag for create_3DWindow
#define GW3D_MINIMIZE         0x20000000L	//defines the MINIMIZE window style flag for create_3DWindow
#define GW3D_VISIBLE          0x10000000L	//defines the VISIBLE window style flag for create_3DWindow
#define GW3D_DISABLED         0x08000000L	//defines the DISABLED window style flag for create_3DWindow
#define GW3D_CLIPSIBLINGS     0x04000000L	//defines the CLIPSIBLINGS window style flag for create_3DWindow
#define GW3D_CLIPCHILDREN     0x02000000L	//defines the CLIPCHILDREN window style flag for create_3DWindow
#define GW3D_MAXIMIZE         0x01000000L	//defines the MAXIMIZE window style flag for create_3DWindow
#define GW3D_CAPTION          0x00C00000L	//defines the CAPTION window style flag for create_3DWindow
#define GW3D_BORDER           0x00800000L	//defines the BORDER window style flag for create_3DWindow
#define GW3D_DLGFRAME         0x00400000L	//defines the DLGFRAME window style flag for create_3DWindow
#define GW3D_VSCROLL          0x00200000L	//defines the VSCROLL window style flag for create_3DWindow
#define GW3D_HSCROLL          0x00100000L	//defines the HSCROLL window style flag for create_3DWindow
#define GW3D_SYSMENU          0x00080000L	//defines the SYSMENU window style flag for create_3DWindow
#define GW3D_THICKFRAME       0x00040000L	//defines the THICKFRAME window style flag for create_3DWindow
#define GW3D_GROUP            0x00020000L	//defines the GROUP window style flag for create_3DWindow
#define GW3D_TABSTOP          0x00010000L	//defines the TABSTOP window style flag for create_3DWindow

#define GW3D_EXTERNAL_CONTEXT 0x00000001L	//defines if using an external opengl context

#define GW3D_MINIMIZEBOX      0x00020000L	//defines the MINIMIZEBOX window style flag for create_3DWindow
#define GW3D_MAXIMIZEBOX      0x00010000L	//defines the MAXIMIZEBOX window style flag for create_3DWindow

#define GW3D_TILED            GW3D_OVERLAPPED	//defines the TILED window style flag for create_3DWindow
#define GW3D_ICONIC           GW3D_MINIMIZE	//defines the ICONIC window style flag for create_3DWindow
#define GW3D_SIZEBOX          GW3D_THICKFRAME	//defines the SIZEBOX window style flag for create_3DWindow
#define GW3D_TILEDWINDOW      GW3D_OVERLAPPEDWINDOW	//defines the TILEDWINDOW window style flag for create_3DWindow

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A 3D window for viewing the 3D scene. </summary>
///
/// <remarks>	A window must have at least one camera in order for the scene to be rendered. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DWindow
    {
        virtual ~IGW3DWindow() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the window coordinates for a camera </summary>
        ///
        /// <remarks>	If you set IGW3DCamera::put_NormalizeWindowCoordinates on a camera, and you need
        /// 			to know the real window coordinates on the window you can use this API. </remarks>
        ///
        /// <param name="camera">	The camera. </param>
        /// <param name="x">	   	[out] window x. </param>
        /// <param name="y">	   	[out] window y. </param>
        /// <param name="width"> 	[out] window width. </param>
        /// <param name="height">	[out] window height. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_CameraWindowCoordinates(IGW3DCameraWPtr camera, int& x, int& y, unsigned int& width, unsigned int& height) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the window position. </summary>
        ///
        /// <param name="pixel_x">	The pixel x coordinate. </param>
        /// <param name="pixel_y">	The pixel y coordinate. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_WindowPosition(int pixel_x, int pixel_y) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the window size. </summary>
        ///
        /// <param name="pixel_x">	   	The pixel x coordinate. </param>
        /// <param name="pixel_y">	   	The pixel y coordinate. </param>
        /// <param name="pixel_width"> 	Width in pixels. </param>
        /// <param name="pixel_height">	Height in pixels. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_WindowSize(int pixel_x, int pixel_y, unsigned int pixel_width, unsigned int pixel_height) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the window size. </summary>
        ///
        /// <param name="x">	 	[out] The pixel x coordinate. </param>
        /// <param name="y">	 	[out] The pixel y coordinate. </param>
        /// <param name="width"> 	[out] The width in pixels. </param>
        /// <param name="height">	[out] The height in pixels. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_WindowSize(int& x, int& y, unsigned int& width, unsigned int& height) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Returns the camera that would be rendered at the top most at this pixel location. </summary>
        ///
        /// <param name="pixel_x">	The pixel x coordinate. </param>
        /// <param name="pixel_y">	The pixel y coordinate. </param>
        ///
        /// <returns>	null camera if nothing, or camera if something at this location </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DCameraWPtr get_TopCamera(int pixel_x, int pixel_y) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Returns the camera that would be rendered at the top most at this pixel location. </summary>
        ///
        /// <param name="pixel_x">	The pixel x coordinate. </param>
        /// <param name="pixel_y">	The pixel y coordinate. </param>
        ///
        /// <returns>	null camera if nothing, or camera if something at this location </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DCameraWPtr get_TopCamera(int pixel_x, int pixel_y) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the camera collection. </summary>
        ///
        /// <remarks>	Gets a collection containing all cameras that currently exist for this window.
        /// 			Cameras are also created and closed via the collection. </remarks>
        ///
        /// <returns>	the camera collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DCameraCollection* get_CameraCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the window name. </summary>
        ///
        /// <remarks>	Gets the window name that was set at window creation. </remarks>
        ///
        /// <returns>	the name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_Name() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the anti-aliasing quality. </summary>
        ///
        /// <remarks>	Value should be a power of two.  Higher values will result in better visual quality,
        /// 			but with a performance cost.  Transparency is not supported when this is set to 0. </remarks>
        ///
        /// <param name="quality">	The quality. </param>
        ///
        /// <returns>	GW3D_sOk is succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	GW3DResult	put_Quality(unsigned quality) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the anti-aliasing quality. </summary>
        ///
        /// <returns>	The quality. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	unsigned	get_Quality() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets operating system window handle. </summary>
        ///
        /// <remarks>	This is used for associating the 3D window with a parent GUI window. </remarks>
        ///
        /// <returns>	The operating system window handle. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual OSWinHandle get_OSWinHandle() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets operating system device that the OpenGL context was created on. </summary>
        ///
        /// <remarks>	With Windows 7 and beyond, it is almost certain this will be the primary monitor
        /// 			(as per the Windows Control Panel) </remarks>
        ///
        /// <returns>	the details of this monitor. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayDetailsPtr get_Details() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets stereo driver </summary>
        ///
        /// <remarks>	Gets the stereo driver that the window was constructed with.</remarks>
        ///
        /// <returns>	the stereo driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DStereoDriverWPtr get_StereoDriver() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a video. </summary>
        ///
        /// <remarks>	Creates a video recording of the window. </remarks>
        ///
        /// <param name="file">  	The file. </param>
        /// <param name="driver">	[in] The codec driver. </param>
        ///
        /// <returns>	The new video. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual IGW3DMediaCenterTaskPtr create_Video( const char *file, IGW3DCodecDriverWPtr &driver ) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets back buffer no reference. </summary>
        ///
        /// <remarks>	For those that want to use Direct3D.
        ///
        /// 			Do not use: not fully implemented. </remarks>
        ///
        /// <returns>	null if it fails, else the back buffer no reference. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual IDirect3DSurface9 *get_BackBufferNoRef() = 0;

    };
}