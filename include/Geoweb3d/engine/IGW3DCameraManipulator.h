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
#include <Geoweb3dCore/Geoweb3dTypes.h>

/* Primary namespace */
namespace Geoweb3d
{
	struct IGW3DAnimationPathPlaybackEventCallback;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> TODO </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DBearingHelper
	{
		virtual ~IGW3DBearingHelper() {};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the location. </summary>
		///
		/// <param name="longitude">	The longitude. </param>
		/// <param name="latitude">		The latitude. </param>
		/// <param name="altitude">		The altitude. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_SourceLocation(double longitude, double latitude, double altitude) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the location. </summary>
		///
		/// <param name="longitude">	[out] The longitude. </param>
		/// <param name="latitude"> 	[out] The latitude. </param>
		/// <param name="altitude"> 	[out] The altitude. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_SourceLocation(double& longitude, double& latitude, double& altitude) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the rotation. </summary>
		///
		/// <param name="heading">	The heading. </param>
		/// <param name="pitch">  	The pitch. </param>
		/// <param name="roll">   	The roll. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_SourceRotation(double heading, double pitch, double roll) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the rotation. </summary>
		///
		/// <param name="heading">	[out] The heading. </param>
		/// <param name="pitch">  	[out] The pitch. </param>
		/// <param name="roll">   	[out] The roll. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_SourceRotation(double& heading, double& pitch, double& roll) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the location. </summary>
		///
		/// <param name="longitude">	The longitude. </param>
		/// <param name="latitude"> 	The latitude. </param>
		/// <param name="altitude"> 	The altitude. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_TargetLocation(double longitude, double latitude, double altitude) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the location. </summary>
		///
		/// <param name="longitude">	[out] The longitude. </param>
		/// <param name="latitude"> 	[out] The latitude. </param>
		/// <param name="altitude"> 	[out] The altitude. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_TargetLocation(double& longitude, double& latitude, double& altitude) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Run the math and get the bearing, pitch, and distance needed for the source to look at the target. </summary>
		///
		/// <param name="bearing"> TODO </param>
		/// <param name="pitch"> TODO </param>
		/// <param name="distance"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void calculate_result(double& bearing, double& pitch, double& distance) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Create a bearing helper object. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static IGW3DBearingHelperPtr create(unsigned interface_version = GEOWEB3D_INTERFACE_VERSION);
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A camera manipulator. </summary>
	///
	/// <remarks>	A class for manipulating a camera position, for example when panning and moving. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DCameraManipulator
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent how heading is effected by orbit commands. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum OrbitMode
		{
			/// <summary> Adjust heading after the orbit to look at target position. </summary>
			LOOKAT_HEADING = 0,
			/// <summary>  Orbit and leave heading alone. </summary>
			SAME_HEADING
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Enable or disable setting of the camera location and orientation via IGW3DCameraManipulator. </summary>
		///
		/// <param name="enable"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_EnableOverride(bool enable) = 0; //disabled by default

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Gets whether setting of the camera location and orientation via IGW3DCameraManipulator is enabled or disabled. </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_EnableOverride() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the window coordinate origin. </summary>
		///
		/// <remarks>	Sets the window coordinate origin to be used in defining window coordinates.
		///				in this class.  LOWER_LEFT is the assumed origin for OpenGL, where UPPER_LEFT
		///				is the assumed origin for GDI, X11 and Direct3D through DirectX version 9.</remarks>
		///
		/// <param name="origin">	The origin. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_WindowCoordinateOrigin(WindowCoordinateOrigin origin) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the window coordinate origin. </summary>
		///
		/// <returns>	[out] The currently configured origin. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual WindowCoordinateOrigin get_WindowCoordinateOrigin() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target point to always keep as the pan focus. Typically done on a mouse down event. </summary>
		///
		/// <param name="window_x"> the target point. </param>
		/// <param name="window_y"> the target point. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void pan_Begin(int window_x, int window_y) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Triggers the camera to move to a new position so that the pan target location
		///				appears at the given window coordinate (for example, the mouse cursor). Typically done
		///             on a mouse move event. </summary>
		///
		/// <param name="window_x"> the target point. </param>
		/// <param name="window_y"> the target point. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void pan_ToWindowCoordinate(int window_x, int window_y) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Rotates the camera orientation based on the delta between two window coordinates. </summary>
		///
		///	<remarks>	This is primarily meant to facilitate rotation based on movement of a mouse cursor. </remarks>
		///
		/// <param name="x_start">	the start x window coordinate for rotation. </param>
		/// <param name="y_start">	the start y window coordinate for rotation. </param>
		/// <param name="x_end">	the end x window coordinate for rotation. </param>
		/// <param name="y_end">	the end y window coordinate for rotation. </param>
		/// <param name="dampen_x">	dampening factor for rotation along the x. </param>
		/// <param name="dampen_y">	dampening factor for rotation along the y. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void rotate_ByWindowCoordinateDelta(int x_start, int y_start, int x_end, int y_end, double dampen_x = 1.0, double dampen_y = 1.0) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Move the camera location relative to the current camera heading.</summary>
		///
		/// <param name="distance">	Distance in meters to move the camera location. </param>
		/// <param name="relative_direction"> the relative direction to move (0.0 moves forward). </param>
		/// <param name="use_pitch"> if true, move along pitch, else move only horizontally. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void move(double distance, double relative_direction, bool use_pitch) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Move the camera location toward a target location.</summary>
		///
		/// <param name="distance">	Distance in meters to move the camera location. </param>
		/// <param name="location"> the target location to move toward. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void move_Toward(double distance, const GW3DPoint* location) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the closest horizon location relative to the camera's  view vector.</summary>
		///
		/// <param name="lon">	Closest horizon longitude. </param>
		/// <param name="lat">  Closest horizon latitude. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_ClosestHorizonLocation(double& lon, double& lat) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the closest geospatial location relative to the camera's view vector
		/// 			at a specified angular distance from camera location.  If this distance is over  
		/// 			the horizon the closest horizon location is returned.  </summary>
		///
		/// <param name="lon">	Closest longitude. </param>
		/// <param name="lat">  Closest latitude. </param>
		/// <param name="angle">	The angular distance specified in degrees. Must be between [0, 90). </param>
		/// 
		/// <returns> Whether the angular distance is within the limits of the parameter. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_ClosestGeospatialLocation(double& lon, double& lat, const double& angle) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Orbit the camera location around a target location.</summary>
		///
		/// <param name="azimuth_ang">	Angle in degrees to orbit. </param>
		/// <param name="altitude_ang">	Angle in degrees to orbit. </param>
		/// <param name="target_location"> the target location to orbit around. </param>
		/// <param name="heading_adjustment"> see OrbitMode. </param>
		///
		/// <returns>	Whether a orbit was applied. A failure case would be if the camera is set to stay above ground
		///				and the altitude angle cannot be honored. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult orbit(double azimuth_angle, double altitude_ang, const GW3DPoint* target_location, OrbitMode heading_adjustment = OrbitMode::LOOKAT_HEADING) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Orbit the camera location around a target location.</summary>
		///
		/// <param name="azimuth_ang">	Angle in degrees to orbit. </param>
		/// <param name="altitude_ang">	Angle in degrees to orbit. </param>
		/// <param name="range">	Distance in meters to orbit the camera location. </param>
		/// <param name="target_location"> The target location to orbit around. </param>
		/// <param name="heading_adjustment"> see OrbitMode. </param>
		///
		/// <returns> 	Whether a orbit was applied. A failure case would be if the camera is set to stay above ground
		/// 			and the altitude angle cannot be honored. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult orbit(double azimuth_ang, double altitude_ang, double range, const GW3DPoint* target_location, OrbitMode heading_adjustment = OrbitMode::LOOKAT_HEADING) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the orbit parameters defined by a target.</summary>
		///
		/// <param name="azimuth_angle"> Orbit's azimuth angle relative from target.	
		///								 i.e. 0 would be a camera south of the target, 90 would be a camera west of the target </param>
		/// <param name="altitude_angle"> Orbit's altitude angle relative from target. 
		///								  [degrees 0 -> -90 -> -180] : (-90) is camera above the target, (-45) is a downward angle of the camera where as (-135) would be the same view but upside down
		///								</param>
		/// <param name="range"> Distance between camera and target. </param>
		/// <param name="target_location"> the target location that defines the orbit parameters.  </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_OrbitAngles(double& azimuth_angle, double& altitude_angle, double& range, const GW3DPoint* target_location) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Orbit the camera location around a target location.</summary>
		///
		/// <param name="range_diff"> A distance delta to move camera relative to a target location. </param>
		/// <param name="location"> The target location. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void orbit_ByRange(double range_diff, const GW3DPoint* target_location) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Orbit the camera location relative to a target location by elevation.</summary>
		///
		/// <param name="elevation_diff"> A elevation delta to move a camera while looking at a target location. </param>
		/// <param name="target_location"> The target location. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void orbit_ByElevation(double elevation_diff, const GW3DPoint* target_location) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Orbit the camera location around a target location.</summary>
		///
		/// <param name="azimuth_ang_diff">	Azimuth angle delta to orbit camera relative to a target location. 
		///									See get_OrbitAngles for azimuth angle conventions.</param>
		/// <param name="target_location"> the target location to orbit around. </param>
		/// <param name="heading_adjustment"> see OrbitMode. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void orbit_ByAzimuth(double azimuth_ang_diff, const GW3DPoint* target_location, OrbitMode heading_adjustment = OrbitMode::LOOKAT_HEADING) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A specific type of orbit found in Google earth.  A heading based orbit occurs in the x delta coordinates;
		///             whereas, as hinge/pitch type orbit is in the y delta coordinates.
		///             The hinge axis is defined by the target location and it's projected point onto the great circle plane 
		///             (defined by the camera and it's heading). </summary>
		///
		///	<remarks>	This is primarily meant to facilitate orbit based motion based on movement of a mouse cursor. </remarks>
		///
		/// <param name="x_start">	the start x window coordinate for heading based rotation. </param>
		/// <param name="y_start">	the start y window coordinate for hinge rotation. </param>
		/// <param name="x_end">	the end x window coordinate for heading based rotation. </param>
		/// <param name="y_end">	the end y window coordinate for hinge rotation. </param>
		/// <param name="dampen_x">	dampening factor for rotation along the x. </param>
		/// <param name="dampen_y">	dampening factor for rotation along the y. </param>
		/// <param name="target_location">  If true need to update camera rotation to look at target. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult hingeAndOrbit_ByWindowCoordinateDelta(int x_start, int y_start, int x_end, int y_end, double dampen_x, double dampen_y, const GW3DPoint* target_location) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target point to always keep as the hinge and orbit focus. Typically done on a mouse down event. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void mouse_hinge_orbit_Begin(int window_x, int window_y) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Orbit the camera location to or from a target location.</summary>
		///
		///	<remarks>	This is primarily meant to facilitate orbit based motion based on movement of a mouse cursor. </remarks>
		///
		/// <param name="x_start">	the start x window coordinate for rotation. </param>
		/// <param name="y_start">	the start y window coordinate for rotation. </param>
		/// <param name="x_end">	the end x window coordinate for rotation. </param>
		/// <param name="y_end">	the end y window coordinate for rotation. </param>
		/// <param name="target_location">  If true need to update camera rotation to look at target. </param>
		/// <param name="heading_adjustment"> see OrbitMode. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void orbit_ByWindowCoordinateDelta(int x_start, int y_start, int x_end, int y_end, const GW3DPoint* target_location, OrbitMode heading_adjustment = OrbitMode::LOOKAT_HEADING) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target point to always keep as the zoom and orbit focus. Typically done on a mouse down event. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void mouse_zoom_orbit_Begin(int window_x, int window_y) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Zoom and Orbit the camera location to or from a target location.</summary>
		///
		///	<remarks>	This is primarily meant to facilitate zoom and orbit based motion based on movement of a mouse cursor. </remarks>
		///
		/// <param name="window_x">	the current window x coordinate for determining azimuth rotate speed. </param>
		/// <param name="window_y">	the current window y coordinate for determining zoom speed. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void zoomAndOrbit_ByWindowCoordinateDelta(int window_x, int window_y) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Move camera forward and update bearing to follow the great circle. </summary>
		///
		/// <param name="distance">	Distance in meters to move the camera location. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void move_AndUpdateBearing(const double& distance) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Orient the camera to be looking at the target location .</summary>
		///
		/// <param name="target_location"> The target location to look at. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void look_At(const GW3DPoint* target_location) = 0;


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target point to always keep as the focus of a pan, zoom, and orbit. Typically done on a down event. </summary>
		///
		///	<remarks>	A target location is captured and used for all scale operations </remarks>
		/// 
		/// <param name="window_x">	the target point. </param>
		/// <param name="window_y">	the target point. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void panZoomAndOrbit_Begin(int window_x, int window_y) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Pan, Zoom, and Orbit the camera location to or from a target location.</summary>
		///
		///	<remarks>	This is primarily meant to facilitate pan, zoom, and orbit based motion based on a move. 
		///				A scale of < 1.0 will move the camera towards the target location.
		///				A scale of > 1.0 will move the camera away from the target location.
		///				The camera will tilt about the target location as the range increase above 375,000 meters. </remarks>
		///
		/// <param name="window_x">	the target point. </param>
		/// <param name="window_y">	the target point. </param>
		/// <param name="angle">	the Angle in degrees to orbit (Azimuth) </param>
		/// <param name="scale">	scale factor used to adjust the cameras distance from the target location. [0, Inf] </param>
		/// <param name="alt_zoom_scale"> scale factor used to adjust overall zoom scaling based on altitude. [1.0, Inf] 
		/// This is applied on a linear scale starting at camera altitude 375000.0m, from this point the scaling factor will start
		/// to be applied. If alt_zoom_scale is set to 2.0f then as you get closer to the ground you should see your altitude
		/// changing at twice the rate then if this value was 1.0f. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void pan_ToWindowCoordinate_ZoomAndOrbit_ByAngleScale(int window_x, int window_y, float angle, float scale, float alt_zoom_scale = 3.0f) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Creates an IGW3DAnimationPath</summary>
		///
		/// <returns>	[out] The IGW3DAnimationPath. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DAnimationPathPtr create_AnimationPath() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Attaches an IGW3DAnimationPath for automated playback </summary>
		///
		/// <param name="path"> the animation path. </param>
		/// 
		/// <returns>	[out] The IGW3DAnimationPathPlayback interface use to control playback. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DAnimationPathPlaybackWPtr attach_AnimationPath(IGW3DAnimationPathPtr path, IGW3DAnimationPathPlaybackEventCallback* pCallback = 0) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Throws the camera, if the conditions are right. </summary>
		/// 
		/// <remarks>	If the user panned recently and "threw" or "flung"
		///				the mouse and they've properly set their camera
		///				settings to allow throwing, then this will implicitly
		///				throw the camera based off of the last panning operation.
		///				Since the camera can't know that the mouse was released,
		///				it's up to a client to pump this themselves. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void throw_Camera() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Cancels any active camera throw, if the conditions are right. </summary>
		/// 
		/// <remarks>	Use this if there's a need to programmatically cancel a throw
		///				that may be currently active. If there's no active throw, there
		///				won't be any side effects.									 </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void cancel_Throw() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Applies new camera throw settings to this camera. </summary>
		/// 
		/// <remarks>	See Geoweb3d::IGW3DCameraThrowSettings for more information.
		///				This operation is always successful since the throw settings
		///				enforce valid parameter ranges. </remarks>
		///
		/// <param name="camera_throw_settings"> The new camera throw settings. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_CameraThrowSettings( const Geoweb3d::IGW3DCameraThrowSettings& camera_throw_settings ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the current throw settings. </summary>
		/// 
		/// <returns>	[out] The camera manipulator's current throw settings. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual Geoweb3d::IGW3DCameraThrowSettings* const get_CameraThrowSettings() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns a position ensuring the supplied envelope
		///				will be visible within the current camera's view. </summary>
		/// 
		/// <remarks>	We will attempt to set the camera view to be the center of the envelope. </remarks>
		/// 
		/// <remarks>	If the order of Min / Max members are reversed, due to the confusing nature
		///				of minimums and maximums depending on the quadrant, we'll try our best 
		///				to detect that and reverse their ordering to make sense of it. </remarks>
		/// 
		/// <param name="envelope"> The envelope to determine a new camera view from. </param>
		/// <param name="result"> GW3D_sOK if a valid GW3DPoint was generated. </param>
		/// 
		/// <returns>	[out] Geoweb3d::GW3DPoint with a position such that the envelope will be visible. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DPoint ensure_Visible( const GW3DEnvelope& envelope, Geoweb3d::GW3DResult& result ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns a position ensuring the supplied positional data 
		///				will be visible within the current camera's view. </summary>
		/// 
		/// <remarks>	If the order of arguments are reversed for minimums and maximums, due to the 
		///				confusing nature of minimums and maximums depending on the quadrant, we'll try
		///				our best to detect that and reverse their ordering to make sense of it. </remarks>
		/// 
		/// <param name="minimum_latitude"> The latitude closer to 0.0. </param> 
		/// <param name="minimum_longitude"> The longitude closer to 0.0. </param>
		/// <param name="maximum_latitude"> The latitude farther from 0.0. </param>
		/// <param name="maximum_longitude"> The latitude farther from 0.0. </param>
		/// <param name="result"> GW3D_sOK if a valid GW3DPoint was generated. </param>
		/// 
		/// <returns>	[out] Geoweb3d::GW3DPoint with the position such that both points will be visible. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DPoint ensure_Visible( double minimum_latitude, double minimum_longitude,
		                                            double maximum_latitude, double maximum_longitude,
		                                            Geoweb3d::GW3DResult& result ) = 0;

	};
}
