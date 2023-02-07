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
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Camera stereo settings </summary>
    ///
    /// <remarks>	The stereo settings for a camera.  Note that a window must be created with a
    /// 			stereo driver for stereoscopic 3D to be supported.  Anaglyph stereo is available
    /// 			by default without the need for a stereo driver.</remarks>
    ///
    /// <see cref="IGW3DWindowCollection::create_3DWindow"/>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DCameraStereoSettings
    {
        virtual ~IGW3DCameraStereoSettings() {}

        enum TrackingOrigin
        {
            ///	<summary>	HMD Tracking is relative to the seated zero pose.	</summary>
            SEATED = 0,
            ///	<summary>	 HMD Tracking is relative to the safe bounds configured by the user.	</summary>
            STANDING = 1
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enable / disable stereo for the camera. </summary>
        ///
        /// <remarks>	When disabled, the camera will not be displayed in stereo. Note, if the window is
        /// 			using stereoscopic 3D you may still have a performance hit.  Its better to create
        /// 			the window without a stereo driver.
        ///
        /// 			As for anaglyph, this can be enabled/disabled freely as it does not require the
        /// 			OS to be configured at window creation. &lt;returns&gt; </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_Enable(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if stereo is enabled for the camera. </summary>
        ///
        /// <returns>	true if enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_Enable()  const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the eye swap. </summary>
        ///
        /// <remarks>	For stereoscopic 3D, it may be necessary to swap the eyes in order for the
        /// 			stereo image to be properly viewed. This is a hardware issue not related to
        /// 			Geoweb3d.
        ///
        /// 			This API has no effect with anaglyph stereo, which does not rely on graphics
        /// 			hardware. </remarks>
        ///
        /// <param name="swapeyes">	true to swap eyes. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_SwapEyes(bool swapeyes) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if eyes are swapped. </summary>
        ///
        /// <returns>	true if swapped, false if not swapped. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_SwapEyes() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the eye seperation. </summary>
        ///
        /// <remarks>	Sets the eye seperation setting. </remarks>
        ///
        /// <param name="seperation">	The eye seperation. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	GW3DResult put_EyeSeperation(float seperation) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the eye seperation. </summary>
        ///
        /// <remarks>	Gets the eye seperation setting. </remarks>
        ///
        /// <returns>	The eye seperation. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	float get_EyeSeperation() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the focal length. </summary>
        ///
        /// <param name="focallength">	The focal length. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	GW3DResult put_FocalLength(float focallength) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the eye focal length. </summary>
        ///
        ///	<returns>	the focal length. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	float get_FocalLength() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the vignette border width. </summary>
        ///
        /// <remarks>	To help reduce visual disturbance when objects hit the edge of the display area,
        /// 			Geoweb3d draws a vignette around the perimeter of the camera. </remarks>
        ///
        /// <param name="width">	The width of the vignette in pixels. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_VignetteBorderWidth(float width) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the vignette border width. </summary>
        ///
        /// <returns>	The vignette border width in pixels. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_VignetteBorderWidth() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the focal fine tune scalar. </summary>
        ///
        /// <remarks>	The focal plane will lie close to the nearest object in the scene.  The fine tune
        /// 			scalar will allow the user to adjust the focal plane around that object as a
        /// 			percentage (i.e., 10% will do focal length + focal length * 10%), where pushing
        /// 			the focal length out into the screen causes more of a 'pop out' effect </remarks>
        ///
        /// <param name="focal_adjustment">	The focal adjustment. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_FocalFineTuneScalar(float focal_adjustment) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the focal fine tune scalar. </summary>
        ///
        /// <returns>	The focal fine tune scalar. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_FocalFineTuneScalar() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the parallax fine tune scalar. </summary>
        ///
        /// <remarks>	Allows user to adjust the parallax of the scene by a small percentage on the
        /// 			original parallax set, similar to the focal fine tune scalar.  This is a percentage
        ///				the number of pixels of the display an object can have between it on the left/right
        ///				view will be:
        ///
        ///				A = maximum parallax * render_surface_width_in_pixels
        ///				#Pixels = A + A * parallax_fine_tune_scalar  </remarks>
        ///
        /// <param name="parallax_adjustment">	The parallax adjustment. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_ParallaxFineTuneScalar(float parallax_adjustment) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the parallax fine tune scalar. </summary>
        ///
        /// <returns>	The parallax fine tune scalar. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_ParallaxFineTuneScalar() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the maximum allowable parallax. </summary>
        ///
        /// <remarks>	The amount of possible parallax for the furthest object in the scene ( such that no
        /// 			object closer will have more parallax ), implemented as a percentage of the
        /// 			rendering area.  So, for 0.03 will allow a 3% parallax of the rendering area or a
        /// 			maximum separation between objects of ( viewport_width_in_pixels * 0.03 ) pixels.
        /// 			</remarks>
        ///
        /// <param name="percentage">	The percentage maximum allowable parallax. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_MaximumAllowableParallax(float percentage) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets maximum allowable parallax. </summary>
        ///
        /// <returns>	The percentage maximum allowable parallax. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_MaximumAllowableParallax() const = 0;


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Tracking Origin for a HMD. </summary>
        ///
        /// <returns>	The Tracking Origin. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual TrackingOrigin get_TrackingOrigin() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the Tracking Origin for a HMD. </summary>
        ///
        /// <remarks>	IF active stereo driver does not support this api then GW3D_eNotimpl is returned. </remarks>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_TrackingOrigin(TrackingOrigin origin) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the zero pose for the seated tracker coordinate system to the current position 
        ///             and yaw of the HMD. </summary>
        ///
        /// <remarks>	This function overrides the user's previously saved seated zero pose and should only 
        ///             be called as the result of a user action. Users are also able to set their seated 
        ///             zero pose via the HMD Dashboard.
        ///
        ///             IF active stereo driver does not support this api then GW3D_eNotimpl is returned. </remarks>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult reset_SeatedZeroPose() = 0;

    };
}