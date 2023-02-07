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
#include "IGW3DAnalysis.h"
#include "IGW3DPositionOrientation.h"

/* Primary namespace */
namespace Geoweb3d
{

	//////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A Terrain Avoidance analysis. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DVisibilityDomeTest
		: public IGW3DPositionOrientation
#ifndef SWIGGING
		, public IGW3DAnalysis
#endif
	{
		virtual ~IGW3DVisibilityDomeTest() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables/disables . </summary>
		///
		/// <remarks>	While disabled,  will not be visible </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Enabled(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the enabled state. </summary>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables/disables Terrain Only. </summary>
		///
		/// <remarks>	While disabled, Visibility Dome Analytic will be run on Terrain and Scene geometry. 
		///				Otherwise it will only be run on terrain. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_TerrainOnly(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the Terrain Only state. </summary>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_IsTerrainOnly() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the inner radius. </summary>
		///
		/// <param name="radius">	The inner radius in meters. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_InnerRadius(float radius) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the inner radius of the Visibility Dome. </summary>
		///
		/// <returns>	The inner radius in meters. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_InnerRadius() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the outer radius. </summary>
		///
		/// <param name="radius">	The outer radius in meters. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void  put_OuterRadius(float radius) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the outer radius of the Visibility Dome. </summary>
		///
		/// <returns>	The outer radius in meters. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual float get_OuterRadius() const = 0;

#ifdef SWIGGING

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a camera to the analysis (like viewshed). </summary>
	///
	/// <remarks>	Adds a camera to the analysis, causing visual results to be rendered on this
	///				camera.  No results  will be visible in the 3D scene unless a camera is added.
	///				TODO: this will probably become a collection so that one can query which camera
	///				as were added. </remarks>
	///
	/// <param name="camera">	The camera to add. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void put_AddCamera(IGW3DCameraWPtr camera) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Removes a camera from the viewshed. </summary>
	///
	/// <remarks>	Removes a camera from the viewshed, causing it to no longer be rendered for that
	/// 			camera. </remarks>
	///
	/// <param name="camera">	The camera to remove. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void put_RemoveCamera(IGW3DCameraWPtr camera) = 0;
#endif

	};
}