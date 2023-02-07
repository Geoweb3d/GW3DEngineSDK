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
	struct  GW3D_DLL IGW3DLightingConfiguration
	{
		virtual  ~IGW3DLightingConfiguration() {};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enable/disable lighting override </summary>
		///
		/// <remarks>	When enabled, the lighting for the scene is being controlled via the Azimuth and Elevation
		/// parameters. It is important to note, that the ephemeris model (position of sun, moon, stars) is still 
		/// controlled by the time-of-day api. When disabled the lighting for the scene is controlled by the time-
		/// of-day api.  </remarks>
		///
		/// <param name="enable">	true to enable, false to disable </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_EnableLightingOverride(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if lighting is being overridden. </summary>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_EnableLightingOverride() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> puts the Azimuth for the light source used when overriding time-of-day lighting </summary>
		///
		/// <remarks>	0 degrees is due north. 90 degrees due east, 180 degrees is due south 270 degrees is due west.
		/// Defaulted to 120 degrees. </remarks>
		///
		/// <param name="azimuth">	The horizontal angular distance along the observer's local horizon at sea level
		/// measured clockwise from due north. [0, 360] </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_LightingAzimuth(double azimuth) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the Azimuth of the overriding light source. </summary>
		///
		/// <returns>	The currently set Azimuth. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// 
		virtual double get_LightingAzimuth() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	puts the elevation for the light source used when overriding time-of-day lighting </summary>
		///
		/// <remarks>	0 degrees is at the horizon. 90 degrees is directly above the observer. 
		/// Defaulted to 70 degrees. </remarks>
		///
		/// <param name="elevation"> The vertical angular distance from the observer's local horizon at sea level
		/// toward the Zenith (directly above the observer) [-90, 90 ] </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_LightingElevation(double elevation) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the Elevation of the overriding light source. </summary>
		///
		/// <returns>	The currently set  Azimuth </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_LightingElevation() const = 0;

	};
}