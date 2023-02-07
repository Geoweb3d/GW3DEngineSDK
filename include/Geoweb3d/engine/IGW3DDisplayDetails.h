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

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Details about a display device. </summary>
    ///
    /// <remarks>	Provides details on a single display device. </remarks>
    ///
    /// <see cref="IGW3DDisplayDevices"/>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DDisplayDetails
    {
        virtual ~IGW3DDisplayDetails() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Identify the device name </summary>
        ///
        /// <remarks>	Provides the name of either the adapter device or the monitor device. </remarks>
        ///
        /// <returns>	the device name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_DeviceName() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if this display is primary display. </summary>
        ///
        /// <remarks>	Query if this is the primary device (primary starts at (0,0)). </remarks>
        ///
        /// <returns>	true if primary, false if not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool IsPrimary() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if 'display' is same display. </summary>
        ///
        /// <remarks>	Low level check if the monitor is the same.  Does not compare geometry changes etc.
        /// 			</remarks>
        ///
        /// <param name="display">	The other display. </param>
        ///
        /// <returns>	true if same display, false if not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool IsSameDisplay(const IGW3DDisplayDetailsPtr display) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the device description. </summary>
        ///
        /// <remarks>	This is either a description of the display adapter or of the display monitor.
        /// 			 </remarks>
        ///
        /// <returns>	the device description. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_DeviceDescription() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets device display bounds. </summary>
        ///
        /// <remarks>	Left, Top, Right, Bottom  specifies the display monitor rectangle, expressed
        /// 			in virtual-screen coordinates. Note that if the monitor is not the primary
        /// 			display monitor, some of the rectangle's coordinates may be negative values.
        ///				</remarks>
        ///
        /// <param name="left">  	[out] The left. </param>
        /// <param name="top">   	[out] The top. </param>
        /// <param name="right"> 	[out] The right. </param>
        /// <param name="bottom">	[out] The bottom. </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_DeviceRect(int& left, int& top, int& right, int& bottom) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The work area extents if the display </summary>
        ///
        /// <remarks>	Left, Top, Right, Bottom  specifies the work area rectangle of the display monitor
        /// 			that can be used by applications, expressed in virtual-screen coordinates. Windows
        /// 			uses this rectangle to maximize an application on the monitor. The rest of the area
        /// 			in rcMonitor contains system windows such as the task bar and side bars. Note that
        /// 			if the monitor is not the primary display monitor, some of the rectangle's
        /// 			coordinates may be negative values. </remarks>
        ///
        /// <param name="left">  	[out] The left. </param>
        /// <param name="top">   	[out] The top. </param>
        /// <param name="right"> 	[out] The right. </param>
        /// <param name="bottom">	[out] The bottom. </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_DeviceWorkRect(int& left, int& top, int& right, int& bottom) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the current device mode. </summary>
        ///
        /// <remarks>	Gets the current mode the monitor is in, including resolution and frequency. </remarks>
        ///
        /// <returns>	The current device mode. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayModePtr get_CurrentDeviceMode() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection of all supported device modes. </summary>
        ///
        /// <remarks>	This collection contains all the modes that are supported by this display device. </remarks>
        ///
        /// <param name="p">	(optional) [in] If non-null, the IGW3DDisplayModeQueryFilter* to
        /// 					process. </param>
        ///
        /// <returns>	the device mode collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayModeCollection* get_DeviceModeCollection(IGW3DDisplayModeQueryFilter* p = 0) = 0;
    };
}