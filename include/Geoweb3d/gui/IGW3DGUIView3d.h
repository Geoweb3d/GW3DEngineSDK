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
#include "core/GW3DGUIInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A 3D View. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIView3d
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the location. </summary>
		///
		/// <remarks>	Gets the location of the Eye. </remarks>
		///
		/// <returns>	null if it fails, else the location. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const GW3DPoint *get_Location( ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a location. </summary>
		///
		/// <remarks>	Sets a new location for the Eye. </remarks>
		///
		/// <param name="location">	The location. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Location( const GW3DPoint &location ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a rotation. </summary>
		///
		/// <remarks>	Gets the current rotation of the Eye. </remarks>
		///
		/// <param name="heading">	[out] The heading / yaw. </param>
		/// <param name="pitch">  	[out] The pitch. </param>
		/// <param name="roll">   	[out] The roll. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_Rotation( float &heading,  float &pitch, float &roll ) const = 0;

	    ////////////////////////////////////////////////////////////////////////////////////////////////////
	    /// <summary>	Sets a rotation. </summary>
	    ///
	    /// <remarks>	Sets a new rotation for the Eye. </remarks>
	    ///
	    /// <param name="heading">	The heading / yaw. </param>
	    /// <param name="pitch">  	The pitch. </param>
	    /// <param name="roll">   	The roll. </param>
	    /// 
		/// <returns>	GW3D_sOk if succeeded. </returns>
	    ////////////////////////////////////////////////////////////////////////////////////////////////////

	    virtual GW3DResult put_Rotation( float heading,  float pitch, float roll ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets terrain mouse position. </summary>
		///
		/// <remarks>	Gets the geographic position of the mouse on the terrain as of the last draw. </remarks>
		///
		/// <param name="pt">	[out] The point. </param>
		///
		/// <returns>	true if mouse was over terrain, false if it was not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_TerrainMousePosition( GW3DPoint& pt ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets geometry mouse position. </summary>
		///
		/// <remarks>	Gets the geographic position of the mouse on geometry as of the last draw.
		/// 			This may be any geometry, including representations that are not active
		/// 			or even supported in a line segment analysis. </remarks>
		///
		/// <param name="pt">	[out] The point. </param>
		///
		/// <returns>	true if mouse was over geometry, false if it was not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_GeometryMousePosition( GW3DPoint& pt ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets mouse window coordinate. </summary>
		///
		/// <remarks>	Gets the window coordinate of the mouse.  In the pre draw, this is the location 
		/// 			of the mouse in the last draw.  In the pre draw, this is the new location of the
		/// 			mouse going into the next draw. </remarks>
		///
		/// <param name="x">	[out] The x pixel coordinate. </param>
		/// <param name="y">	[out] The y pixel coordinate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_MouseWindowCoordinate (int& x, int& y) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets two points defining the ray defined by the mouse. </summary>
		///
		/// <remarks>	Gets the geographic locations where the mouse position intersects the near and 
		/// 			far planes.  This defines the ray representing the mouse pointer in 3D space, which
		/// 			may be used for defining a line segment analysis for mouse selection. </remarks>
		///
		/// <param name="pixel_x">  	The pixel x coordinate. </param>
		/// <param name="pixel_y">  	The pixel y coordinate. </param>
		/// <param name="ray_start">	[out] The ray start (i.e. the near plane intersection). </param>
		/// <param name="ray_stop"> 	[in,out] The ray stop (i.e. the far plane intersection. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_Ray( /*in*/ int pixel_x, /*in*/  int pixel_y, /*out*/ GW3DPoint &ray_start,  /*out*/ GW3DPoint &ray_stop ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the status overlay. </summary>
		///
		/// <remarks>	Use the status overlay to display temporary messages to the user. </remarks>
		///
		/// <returns>	the status overlay. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIStatusOverlay *get_StatusOverlay( ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	This will return the grid configuration</summary>
		///
		/// <returns>	will never return null</returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DLatitudeLongitudeGrid *get_LatitudeLongitudeGrid( ) = 0;
	};
}
}



