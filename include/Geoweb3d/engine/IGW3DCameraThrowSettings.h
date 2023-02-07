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
	/// <summary>	Camera throw settings. Sometimes also referred to as "fling". </summary>
	///
	/// <remarks>	This allows the client to adjust their throw settings for the following:
	///				    1. Enable / Disable throwing					 ( Disabled by default )
	///				    2. Total animation duration						 ( In milliseconds, default is 3000 ms )
	///				    3. Total duration per frame						 ( In milliseconds, default is 16 ms )
	///				    4. Deceleration rate per frame					 ( Out of (0.0, 1.0], default is 0.025 { 2.5% } )
	///					5. Mouse cursor pixel delta to trigger the throw ( In pixels, default is 3 )
	///					6. Pan to throw timeout to trigger the throw	 ( In milliseconds, default is 80 ms )
	///					7. Throw distance divisor						 ( Out of [1.0, 10.0], default is 6.0 )
	///				Most operations, such as zooming, panning, or orbiting will stop any active throws. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DCameraThrowSettings
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enable / disable the camera's throwing. </summary>
		///
		/// <remarks>	When enabled, the camera will continue to pan in the last direction panned. 
		///				Disabled by default. </remarks>
		///
		/// <param name="enable_throwing">	true to enable, false to disable. </param>
		/// 
		/// <returns>	GW3D_sOK if successful. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_ThrowingEnabled( bool enable_throwing ) = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	If the camera throw is enabled or disabled. </summary>
		///
		/// <returns>	If throwing is enabled or disabled. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_ThrowingEnabled() const = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the total duration of throw animations, in milliseconds. </summary>
		///
		/// <remarks>	The higher this value, the longer the throw will play for.
		///				For a shorter throw duration, a stronger deceleration may be
		///				preferable, otherwise the animation may appear to stop abruptly.
		///				Modulating this value with the camera's altitude / elevation
		///				may produce more favorable behavior as well. </remarks>
		///
		/// <param name="total_milli_seconds_throw_duration">	Total milliseconds throws will play for. </param>
		/// 
		/// <returns>	GW3D_sOK if successful, GW3D_eArgumentOutOfRange otherwise. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_TotalThrowDuration( int total_milli_seconds_throw_duration ) = 0;
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The amount of milliseconds throws will playback for. </summary>
		///
		/// <returns>	Throw duration, in milliseconds. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int get_TotalThrowDuration() const = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the per-frame delta between each throw animation, in milliseconds. </summary>
		///
		/// <remarks>	At higher values, less frames will be generated for the animation over
		///				the total duration. The less frames there are, the less deceleration 
		///				will be applied since it ramps down based on the amount of frames. At lower
		///				values, the animation will appear smoother since more frames will be generated. </remarks>
		///
		/// <param name="milli_seconds_per_throw_frame_duration">	Amount of milliseconds between throw frames. </param>
		/// 
		/// <returns>	GW3D_sOK if successful, GW3D_eArgumentOutOfRange otherwise. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_ThrowFrameDurationPerPoint( int milli_seconds_per_throw_frame_duration ) = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The amount of milliseconds between each throw frame. </summary>
		///
		/// <returns>	Throw frame deltas, in milliseconds. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int get_ThrowFrameDurationPerPoint() const = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the per-frame deceleration between each throw animation. </summary>
		///
		/// <remarks>	The closer to 0.0, the less deceleration will be applied. At 0.0 would
		///				imply no deceleration. If, for example, throw_decelerate_percent is set to 0.05,
		///				it would imply every frame generated would slow the camera's movement by
		///				5%. At 1.0, it would completely stop the camera after a frame, so that is
		///				considered out of bounds and would return GW3D_eArgumentOutOfRange. </remarks>
		///
		/// <param name="throw_decelerate_percent">	Percentage slow down between frames. </param>
		/// 
		/// <returns>	GW3D_sOK if successful, GW3D_eArgumentOutOfRange otherwise. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_DecelerationPercentPerFrame( double throw_decelerate_percent ) = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The percentage of deceleration between frames. </summary>
		///
		/// <returns>	Deceleration between frames, as a percentage. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_DecelerationPercentPerFrame() const = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the mouse or gesture delta, in pixels, needed to trigger a throw. </summary>
		///
		/// <remarks>	For client simplicity, they should be able to pump IGW3DCameraManipulator::throw_Camera()
		///				in their event processing as much as they please. However, to make state tracking easier,
		///				we manage an internal state during panning operations to track mouse / gesture deltas. 
		///				If the client invokes IGW3DCameraManipulator::throw_Camera() and the last panning
		///				operation detected was greater than pixel_delta and throwing is enabled, the camera should throw. </remarks>
		///
		/// <param name="pixel_delta">	A panning gesture's delta, in pixels (x or y), required to begin throwing. </param>
		/// 
		/// <returns>	GW3D_sOK if successful, GW3D_eArgumentOutOfRange otherwise. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_ThrowPixelDetectionThreshold( int pixel_delta ) = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The amount of mouse or gesture pixels required to pan by
		///				to determine a valid throw internally. </summary>
		/// 
		/// <returns>	The delta in x or y pixels of a mouse or gesture to begin throwing. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int get_ThrowPixelDetectionThreshold() const = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	To make the client-side usage of IGW3DCameraController::throw_Camera()
		///				as easy as possible, this will allow you to configure how long between
		///				the last panning operation and the call to IGW3DCameraController::throw_Camera()
		///				can occur. This is necessary to prevent the following scenario:
		///					1. IGW3DCameraController::pan_Begin( ... ).
		///					2. IGW3DCameraController::pan_ToWindowCoordinate( ... ).
		///					3. No updates for some delta time ( e.g. A user is holding their mouse still ).
		///				  4.1. User detects their mouse release in their event processing.
		///				  4.2. IGW3DCameraController::throw_Camera() is invoked client-side.
		///				In addition to the ThrowPixelDetectionThreshold above, this also aids in
		///				preventing erroneous throws. In summary, this is the time between panning
		///				and releasing the mouse to allow a throw. </summary>
		/// 
		/// <remarks>	The longer your processing for events between frame updates, the higher this
		///				value will likely have to be, otherwise throwing may never occur. If your
		///				framerate can vary, you may have to track it client-side and update this 
		///				on a per-frame basis to be longer than your current framerate. </remarks>
		/// 
		/// <param name="milli_seconds_timeout_duration">	The amount of milliseconds between panning
		///													operations and invocations to 
		///													IGW3DCameraController::throw_Camera() that will
		///													actually allow a throw to occur. </param>
		/// 
		/// <returns>	GW3D_sOK if successful, GW3D_eArgumentOutOfRange otherwise. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_PanToThrowTimeoutDuration( double milli_seconds_timeout_duration ) = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The amount of time, in milliseconds, allowed between panning
		///				operations and a call to IGW3DCameraController::throw_Camera()
		///				to actually allow IGW3DCameraController::throw_Camera() to throw. </summary>
		/// 
		/// <returns>	The allowable delta of milliseconds between pan 
		///				operations to invalidate throw calls. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_PanToThrowTimeoutDuration() const = 0;
	
		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The modifier for throw intensity. The higher this number,
		///				the less distance that will be thrown with the same mouse delta. </summary>
		/// 
		/// <param name="throw_distance_divisor">	The value is clamped between [1.0, 10.0] </param>
		/// 
		/// <returns>	GW3D_sOK if successful, GW3D_eArgumentOutOfRange otherwise. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_ThrowDistanceDivisor( double throw_distance_divisor ) = 0;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The modifier for throw intensity. The higher this number,
		///				the less distance that will be thrown with the same mouse delta. </summary>
		/// 
		/// <remarks>	The value is clamped between [1.0, 10.0] </remarks>
		/// 
		/// <returns>	The divisor used to manipulate delta longitude and latitude for
		///				the final position at the end of the throw animation. </returns>
		/////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_ThrowDistanceDivisor() const = 0;

	};
}
