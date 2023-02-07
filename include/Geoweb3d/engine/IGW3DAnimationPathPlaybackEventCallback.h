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
#include "IGW3DAnimationPathPlayback.h"
#include "IGW3DAnimationPath.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Path animation playback callback. </summary>
	///
	/// <remarks>	The path animation playback callback is implemented by the SDK client and passed into the SDK at
	/// 			animation path playback creation. </remarks>
	///
	/// <see cref="IGW3DAnimationController::create_AnimationPathPlayback"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DAnimationPathPlaybackEventCallback
	{
		virtual ~IGW3DAnimationPathPlaybackEventCallback(){}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever animation playback has started.</summary>
		///
		/// <param name="owner">The invoking animation playback object</param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPlaybackStarted(const IGW3DAnimationPathPlayback* owner) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked when animation playback has stopped. Either manually or whenever the animation
		///            sequence has ended( time elapsed ). </summary>
		///
		/// <param name="owner">The invoking animation playback object</param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPlaybackStopped(const IGW3DAnimationPathPlayback* owner) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked each time the animation sequence is advanced per timestep or per-frame. </summary>
		///
		/// <param name="owner">The invoking animation playback object</param>
		/// <param name="playback_point"> todo </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPlaybackUpdated(const IGW3DAnimationPathPlayback* owner, const GW3DPathPoint& playback_point) = 0;
	};
}