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
    /// <summary>	Tests whether a display mode supports Geoweb3d or advanced stereo. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DDisplayModeTest
    {
        virtual ~IGW3DDisplayModeTest() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if capable of supporting a Geoweb3d window. </summary>
        ///
        /// <remarks>	Check to see if the display mode is capable of supporting a Geoweb3d window. </remarks>
        ///
        /// <returns>	true if it is capable, false if it is not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsGeoweb3dCapable() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if capable of supporting advanced stereo. </summary>
        ///
        /// <remarks>	Check to see if the display mode is capable of supporting advanced stereo viewing.
        /// 			If false, the display can still support the anaglyph stereo mode. </remarks>
        ///
        /// <returns>	true if it is capable, false if it is not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsAdvancedStereoCapable() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the detected OpenGL major version. </summary>
        ///
        /// <remarks>	Reports the detected OpenGL major version, up to that required by Geoweb3d.
        /// 			Note that an insufficient OpenGL version may be result of out-of-date drivers
        /// 			rather than insufficient hardware. </remarks>
        ///
        /// <returns>	The OpenGL major version. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int	get_OpenGL_major_version() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the detected OpenGL minor version. </summary>
        ///
        /// <remarks>	Reports the detected OpenGL minor version, up to that required by Geoweb3d.
        /// 			Note that an insufficient OpenGL version may be result of out-of-date drivers
        /// 			rather than insufficient hardware. </remarks>
        ///
        /// <returns>	The OpenGL minor version. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int	get_OpenGL_minor_version() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Queries the maximum multisamples supported by this GPU. </summary>
        ///
        /// <remarks>	Return the OpenGL-returned max multisamples this GPU supports. </remarks>
        ///
        /// <returns>	The maximum supported multisamples. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int	get_OpenGL_max_multisamples() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the GPU vendor. </summary>
        ///
        /// <remarks>	Returns the company responsible for this GL implementation.
        ///				This name is typically specific to a particular configuration of a hardware
        ///				platform. It does not change from release to release.
        ///				Because OpenGL does not include queries for the performance characteristics of an
        ///				implementation, some applications are written to recognize known platforms and
        ///				modify their GL usage based on known performance characteristics of these platforms.
        ///				Strings VENDOR and RENDERER together uniquely specify a platform. </remarks>
        ///
        /// <returns>	the GPU vendor as reported by OpenGL. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_OpenGL_vendor() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the GPU renderer. </summary>
        ///
        /// <remarks>	Returns the renderer responsible for this GL implementation.
        ///				This name is typically specific to a particular configuration of a hardware
        ///				platform. It does not change from release to release.
        ///				Because OpenGL does not include queries for the performance characteristics of an
        ///				implementation, some applications are written to recognize known platforms and
        ///				modify their GL usage based on known performance characteristics of these platforms.
        ///				Strings VENDOR and RENDERER together uniquely specify a platform. </remarks>
        ///
        /// <returns>	the GPU renderer as reported by OpenGL. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_OpenGL_render() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Reserved. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_Reserved1() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Reserved. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_Reserved2() const = 0;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	A display mode. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DDisplayMode
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the width. </summary>
        ///
        /// <remarks>	Gets the width of the display mode in pixels. </remarks>
        ///
        /// <returns>	The width. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_Width() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the height. </summary>
        ///
        /// <remarks>	Gets the height of the display mode in pixels. </remarks>
        ///
        /// <returns>	The height. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_Height() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the desktop position x coordinate. </summary>
        ///
        /// <remarks>	Gets the x coordinate of position of the display within the desktop when in this
        /// 			mode. </remarks>
        ///
        /// <returns>	The position x coordinate. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_PositionX() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the desktop position y coordinate. </summary>
        ///
        /// <remarks>	Gets the y coordinate of position of the display within the desktop when in this
        /// 			mode. </remarks>
        ///
        /// <returns>	The position y coordinate. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_PositionY() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the bits per pel. </summary>
        ///
        /// <returns>	The bits per pel. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_BitsPerPel() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets display frequency (refresh rate). </summary>
        ///
        /// <returns>	The display frequency when in this mode. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_DisplayFrequency() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Applies this display mode as the active display mode. </summary>
        ///
        /// <remarks>	This mode can be applied outside of the window creation of the sdk.  This mode
        ///				is only active for the lifetime of the sdk. </remarks>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult Apply() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Tests this mode for Geoweb3d abilities. </summary>
        ///
        /// <remarks>	Tests this mode for abilities required by Geoweb3d.  This will cause the screen
        /// 			to flicker as it must temporarily activate the display mode.  </remarks>
        ///
        /// <param name="fullscreen">	(optional) test in fullscreen. </param>
        ///
        /// <returns>	A display mode test, containing the results of the test. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDisplayModeTestPtr TestModeForGeoweb3dAbilities(bool fullscreen = false) = 0;
    };
}