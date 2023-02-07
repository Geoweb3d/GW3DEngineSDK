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
#include "../core/IGW3DClientUserData.h"
#include "IGW3DPositionOrientation.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	A camera. </summary>
    ///
    /// <remarks>	A camera is a view into the 3D scene from a defined position. A window may have
    /// 			just one camera (typically covering the entire window) or multiple cameras,
    /// 			where cameras may even overlap (for example, a picture-in-picture). </remarks>
    ///
    /// <see cref="IGW3DWindow"/>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DCamera : public IGW3DClientUserData
    {
        virtual ~IGW3DCamera() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the name. </summary>
        ///
        /// <remarks>	Gets the name of the camera, as specified when the camera was created. </remarks>
        ///
        /// <returns>	the name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_Name() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enable/disable the camera. </summary>
        ///
        /// <remarks>	When disabled, the camera will not render and terrain and other data will not page
        /// 			in around it. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Enabled(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if the camera is enabled. </summary>
        ///
        /// <returns>	true if enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_Enabled()  const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Get the normalized device coordinates that can be use to construct an orthographic 
        ///				matrix to use with the window. </summary>
        ///
        /// <remarks>	Range is 0.0 to 1.0. </remarks>
        ///
        /// <param name="left">  	[in] The left edge. </param>
        /// <param name="right"> 	[in] The right edge. </param>
        /// <param name="top">   	[in] The top edge. </param>
        /// <param name="bottom">	[in] The bottom edge. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_NormalizeWindowCoordinates(float& left, float& right, float& top, float& bottom) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the normalized coordinates of the camera within its window. </summary>
        ///
        /// <remarks>	Range is 0.0 to 1.0. </remarks>
        ///
        /// <param name="left">  	[in] The left edge. </param>
        /// <param name="right"> 	[in] The right edge. </param>
        /// <param name="top">   	[in] The top edge. </param>
        /// <param name="bottom">	[in] The bottom edge. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_NormalizeWindowCoordinates(float left, float right, float top, float bottom) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the date and time interface. </summary>
        ///
        /// <remarks>	Gets the interface through which the date and time of a camera can be queried and
        /// 			set.</remarks>
        ///
        /// <returns>	the date and time interface. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDateTime* get_DateTime() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets camera controller interface. </summary>
        ///
        /// <remarks>	Gets the interface for controlling the camera field of view as well as querying
        /// 			the ray defined by a single pixel in screenspace. </remarks>
        ///
        /// <returns>	the camera controller interface. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DCameraController* get_CameraController() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets camera controller interface. </summary>
        ///
        /// <remarks>	Gets the interface for controlling the camera field of view as well as querying
        /// 			the ray defined by a single pixel in screenspace. </remarks>
        ///
        /// <returns>	the camera controller interface. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DCameraController* get_CameraController() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets camera stereo settings interface. </summary>
        ///
        /// <remarks>	Gets the interface for configuring the stereo settings of the camera. </remarks>
        ///
        /// <returns>	the stereo settings interface. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DCameraStereoSettings* get_CameraStereoSettings() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the environment effects configuration for the camera. </summary>
        ///
        /// <returns>	null if it fails, else the environment effects. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DEnvironmentEffects* get_EnvironmentEffects() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets geodetic query collection. </summary>
        ///
        /// <remarks>	Geodetic queries report the geographic coordinates at a given window coordinate. </remarks>
        ///
        /// <returns>	null if it fails, else the geodetic query collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DWindowCoordinateToGeodeticQueryCollection* get_WindowCoordinateToGeodeticQueryCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the window that owns this camera. </summary>
        ///
        /// <returns>	The window. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DWindowWPtr get_Window() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the stereo configuration for the camera. </summary>
        ///
        /// <remarks>	This will return null if the window was not created with a stereo driver! </remarks>
        ///
        /// <returns>	null if it fails, else the stereo configuration collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStereoConfigurationCollection* get_StereoConfigurationCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the configuration for the globe's latitude/longitude grid. </summary>
        ///
        /// <returns>	will never return null. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DLatitudeLongitudeGrid* get_LatitudeLongitudeGrid() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Apply a mask to define if a representation will render or not in a camera. </summary>
        ///
        /// <remarks>	Changes the set of bits that represent the subset of data (such as representations)
        ///				this camera will render if none of its draw mask bits intersect with this camera
        ///				mask's bits.
        ///
        ///				These masks can be used to selectively hide and show different parts of the scene
        ///				graph (todo if approved), features (todo if approved) and representations from
        ///				different cameras that are viewing the same scene. </remarks>
        ///
        ///	<see cref="IGW3DVectorRepresentation::put_CullMask"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_CameraMask(GW3DCullMask mask) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Returns the set of bits that represent the subset of representations this camera
        ///				will render. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DCullMask get_CameraMask() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Returns the degrees per pixel this camera is capable of displaying at a certain
        ///				height and geodetic location above the ellipsoid. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_DegreesPerPixel(const double& longitude, const double& latitude, const double& height_above_ellipsoid) const = 0;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	A camera offset. </summary>
    ///
    /// <remarks>	A real world camera relative offset. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DCameraOffset : public IGW3DOffsetPositionOrientation
    {
        virtual ~IGW3DCameraOffset() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the state of the offset </summary>
        ///
        /// <remarks>	If state is enabled it will be appled to camera. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableOffsetMode(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the present state. </summary>
        ///
        /// <returns>	The state - whether the offset is being applied.  </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_OffsetMode() = 0;
    };
    
}