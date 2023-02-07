//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

/** @file DisplayDevices.h
 *  @brief  class 'DisplayDevices'...
 */

#pragma once
#include "../core/GW3DInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Interface for querying the available display devices. </summary>
    ///
    /// <remarks>	This interface only contains information relating to visible display monitors. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DDisplayDevices
    {
        virtual ~IGW3DDisplayDevices() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the primary display. </summary>
        ///
        /// <returns>	The primary display. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayDetailsPtr get_PrimaryDisplay() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets nearest display to a given rectangle. </summary>
        ///
        /// <param name="left">  	The left. </param>
        /// <param name="top">   	The top. </param>
        /// <param name="right"> 	The right. </param>
        /// <param name="bottom">	The bottom. </param>
        ///
        /// <returns>	The nearest display to the given rectangle. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayDetailsPtr get_NearestDisplay(int left, int top, int right, int bottom) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets nearest display to a given screen coordinates. </summary>
        ///
        /// <param name="x">	The x coordinate. </param>
        /// <param name="y">	The y coordinate. </param>
        ///
        /// <returns>	The nearest display to the given coordinates. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayDetailsPtr get_NearestDisplay(int x, int y) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets nearest display to the given window. </summary>
        ///
        /// <param name="hwnd">	The OS window handle of the window. </param>
        ///
        /// <returns>	The nearest display to the window. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayDetailsPtr get_NearestDisplay(const OSWinHandle hwnd) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	returns the rectangle that is the union of all active monitors. </summary>
        ///
        /// <param name="left">  	[out] The left. </param>
        /// <param name="top">   	[out] The top. </param>
        /// <param name="right"> 	[out] The right. </param>
        /// <param name="bottom">	[out] The bottom. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_VirtualDesktopRect(int& left, int& top, int& right, int& bottom) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Count of visible display monitors. </summary>
        ///
        /// <returns>	The count. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_Count() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the details of a display by index. </summary>
        ///
        /// <param name="screen_index">	Zero-based index of the screen. </param>
        ///
        /// <returns>	The details. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayDetailsPtr get_Details(unsigned screen_index) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Check whether all display monitors have the same color format. </summary>
        ///
        /// <remarks>	True if all the display monitors have the same color format, otherwise, false. Two
        /// 			displays can have the same bit depth, but different color formats. For example, the
        /// 			red, green, and blue pixels can be encoded with different numbers of bits, or those
        /// 			bits can be located in different places in a pixel color value. </remarks>
        ///
        /// <returns>	true if all monitors share the display format, otherwise false. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool AllMonitorsShareDisplayFormat() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Determine if the given coordinate is visible on any monitor. </summary>
        ///
        /// <param name="x">	The x coordinate. </param>
        /// <param name="y">	The y coordinate. </param>
        ///
        /// <returns>	true if on screen, false if not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool IsOnScreen(int x, int y) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Determine if the given window is visible on any monitor. </summary>
        ///
        /// <param name="hwnd">	The OS window handle. </param>
        ///
        /// <returns>	true if on screen, false if not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool IsOnScreen(const OSWinHandle hwnd) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Determine if the given rectangle is visible on any monitor. </summary>
        ///
        /// <param name="left">  	The left. </param>
        /// <param name="top">   	The top. </param>
        /// <param name="right"> 	The right. </param>
        /// <param name="bottom">	The bottom. </param>
        ///
        /// <returns>	true if on screen, false if not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool IsOnScreen(int left, int top, int right, int bottom) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	A macro that defines monitor center rect. </summary>
        ///
        /// <see cref="ClipOrCenterRectToDisplay"/>
        /// <see cref="ClipOrCenterWindowToDisplay"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

#define MONITOR_CENTER   0x0001

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines monitor clip rect. </summary>
///
/// <see cref="ClipOrCenterRectToDisplay"/>
/// <see cref="ClipOrCenterWindowToDisplay"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define MONITOR_CLIP     0x0000

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines monitor work area. </summary>
///
/// <see cref="ClipOrCenterRectToDisplay"/>
/// <see cref="ClipOrCenterWindowToDisplay"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define MONITOR_WORKAREA 0x0002

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines entire monitor area. </summary>
///
/// <see cref="ClipOrCenterRectToDisplay"/>
/// <see cref="ClipOrCenterWindowToDisplay"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define MONITOR_AREA     0x0000

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Clip or center rectangle to display. </summary>
        ///
        /// <remarks>	The most common problem apps have when running on a multimonitor system is that
        /// 			they "clip" or "pin" windows based on the SM_CXSCREEN and SM_CYSCREEN system
        /// 			metrics. Because of app compatibility reasons these system metrics return the
        /// 			size of the primary monitor. This enables you use the multi-monitor functions
        /// 			to do the same thing.
        ///
        /// 			NOTE! left, top, right, bottom modify in place (clip) your inputs! </remarks>
        ///
        /// <param name="left">  	[in,out] The left. </param>
        /// <param name="top">   	[in,out] The top. </param>
        /// <param name="right"> 	[in,out] The right. </param>
        /// <param name="bottom">	[in,out] The bottom. </param>
        /// <param name="flags"> 	(optional) the flags. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void ClipOrCenterRectToDisplay(int& left, int& top, int& right, int& bottom, unsigned flags = MONITOR_CENTER) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Clip or center window to display. </summary>
        ///
        /// <remarks>	The most common problem apps have when running on a multimonitor system is that
        /// 			they "clip" or "pin" windows based on the SM_CXSCREEN and SM_CYSCREEN system
        /// 			metrics. Because of app compatibility reasons these system metrics return the
        /// 			size of the primary monitor. This enables you use the multi-monitor functions
        /// 			to do the same thing.
        ///
        /// <param name="hwnd"> 	The OS window handle. </param>
        /// <param name="flags">	(optional) the flags. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void ClipOrCenterWindowToDisplay(const OSWinHandle hwnd, unsigned flags = MONITOR_CENTER) = 0;
    };
}