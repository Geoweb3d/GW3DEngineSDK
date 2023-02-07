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
#include "IGW3DPositionOrientation.h"
#include "IGW3DCameraManipulator.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A camera controller. </summary>
	///
	/// <remarks>	Each camera has a camera controller that is the interface for getting and setting 
	///				the camera's position and orientation.  It also provides interfaces for getting and 
	///				setting the camera field of view angle, and for getting the ray defined by a 
	///				given window pixel's intersection with the camera's near and far plane. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DCameraController : public IGW3DPositionOrientation, public IGW3DCameraManipulator
	{
		virtual ~IGW3DCameraController() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the field of view angle. </summary>
		///
		/// <returns>	The field of view angle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_FieldViewAngle() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the field view angle. </summary>
		///
		/// <param name="angle">	The field of view angle. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_FieldViewAngle(double angle) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the ray defined by the intersection of a window pixel with the near and far
		/// 			plane. </summary>
		///
		/// <remarks>	In a virtual world, a near and far plane exists.  This returns both endpoints of
		/// 			the ray at a given screen pixel location from the current camera position. </remarks>
		///
		/// <param name="pixel_x">  	The pixel x coordinate. </param>
		/// <param name="pixel_y">  	The pixel y coordinate. </param>
		/// <param name="ray_start">	[out] The ray start. </param>
		/// <param name="ray_stop"> 	[out] The ray stop. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_Ray( /*in*/ int pixel_x, /*in*/  int pixel_y, /*out*/ GW3DPoint& ray_start,  /*out*/ GW3DPoint& ray_stop) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the normalized view direction vector. </summary>
		///
		/// <remarks>	The Cartesian coordinate system is: 
		///				Y : north pole 
		///				Z : lat, lon : 0, 0
		///				X : lat, lon : 90 E, 0 </remarks>
		///
		/// <param name="x">  	The normalized x coordinate. </param>
		/// <param name="y">  	The normalized y coordinate. </param>
		/// <param name="z">	The normalized z coordinate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_Direction(double& x, double& y, double& z) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the normalized view right vector. </summary>
		///
		/// <remarks>	The Cartesian coordinate system is: 
		///				Y : north pole 
		///				Z : lat, lon : 0, 0
		///				X : lat, lon : 90 E, 0 </remarks>
		///
		/// <param name="x">  	The normalized x coordinate. </param>
		/// <param name="y">  	The normalized y coordinate. </param>
		/// <param name="z">	The normalized z coordinate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_Right(double& x, double& y, double& z) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the normalized view up vector. </summary>
		///
		/// <remarks>	The Cartesian coordinate system is: 
		///				Y : north pole 
		///				Z : lat, lon : 0, 0
		///				X : lat, lon : 90 E, 0 </remarks>
		///
		/// <param name="x">  	The normalized x coordinate. </param>
		/// <param name="y">  	The normalized y coordinate. </param>
		/// <param name="z">	The normalized z coordinate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_Up(double& x, double& y, double& z) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the camera offset object. </summary>
		///
		/// <returns>	The camera offset. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DCameraOffset* get_Offset() = 0;

	};
}