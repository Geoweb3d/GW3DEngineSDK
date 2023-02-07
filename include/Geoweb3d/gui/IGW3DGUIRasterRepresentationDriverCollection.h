////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "core/GW3DGUIInterFace.h"

namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of raster representation drivers. </summary>
	///
	/// <remarks>	Note: The raster API for the GUI SDK is not yet complete, and as such there are no 
	///				drivers available.  The only currently working API is get_Elevation.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIRasterRepresentationDriverCollection : public IGW3DCollection< IGW3DGUIRasterRepresentationDriverWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the elevation for a given latitude and longitude. </summary>
		///
		/// <param name="latitude">   	The latitude for the query. </param>
		/// <param name="longitude">  	The longitude for the query. </param>
		/// <param name="interpolate">	(optional) true to apply Geoweb3d's interpolation, false to 
		/// 							return the raster pixel value. </param>
		///
		/// <returns>	The elevation at the given location, based on the highest priority of all raster 
		/// 			data represented as elevation </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_Elevation (double latitude, double longitude, bool interpolate = 0) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a raster representation driver by name. </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	The representation driver. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIRasterRepresentationDriverWPtr get_Driver( const char *name ) = 0;

	};
}
}