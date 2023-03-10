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
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3DPathPoint
	{
		double longitude;
		double latitude;
		double elevation;
		double roll;
		double pitch;
		double yaw;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	An animation path. </summary>
	///
	/// <remarks>  A class for interpolating a camera position, for example when animating the camera's motion. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DAnimationPath
	{
		virtual ~IGW3DAnimationPath() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Remove all control points from animation path. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void Clear() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add control point to animation path. </summary>
		///
		/// <param name="time">	Time to add point at. </param>
		/// <param name="point">	Point to add. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void add_ControlPoint(double time, const GW3DPathPoint& point) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get interpolated control point from animation path. </summary>
		///
		/// <param name="time">	Time to fetch point for. </param>
		/// <param name="point">	Point at given time. </param>
		///
		/// <returns>	True if point was retrieved, otherwise false. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_InterpolatedControlPoint(double time, GW3DPathPoint& point) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Remove control points before given time from animation path. </summary>
		///
		/// <param name="time">	Time to remove points before. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void remove_ControlPointsBefore(double time) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Remove control points after given time from animation path. </summary>
		///
		/// <param name="time">	Time to remove points after. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void remove_ControlPointsAfter(double time) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check if animation path is empty. </summary>
		///
		/// <returns>	True if animation path is empty, otherwise false. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool IsEmpty() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check beginning time of animation path. </summary>
		///
		/// <returns>	Time of first point in path. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_FirstTime() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check ending time of animation path. </summary>
		///
		/// <returns>	Time of last point in path. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_LastTime() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	builds an animation path from file. </summary>
		///
		/// <remarks>	Any previous contents will be cleared. </remarks>
		/// 
		/// <returns>	 GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult build_FromFile(const char* file) = 0;

	};
}