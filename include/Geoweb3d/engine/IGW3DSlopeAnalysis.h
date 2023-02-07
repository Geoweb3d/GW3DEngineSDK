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
#include "IGW3DRegionOfInterest.h"
#include "IGW3DAnalysis.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A slope analysis. </summary>
	///
	/// <remarks>	The Geoweb3d slope analysis provides a visual display of surface slopes. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  IGW3DSlopeAnalysis 
		: public IGW3DRegionOfInterest
#ifndef SWIGGING
		, public IGW3DAnalysis
#endif
	{
		virtual ~IGW3DSlopeAnalysis() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the inner radius for the slope analysis. </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_InnerRadius() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the inner radius for the slope analysis.	 </summary>
		///
		/// <param name="inRad"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_InnerRadius(const double& inRad) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the outer radius for the slope analysis.	 </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_OuterRadius() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the outer radius for the slope analysis.	 </summary>
		///
		/// <param name="outRad"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_OuterRadius(const double& outRad) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assign a color lookup table finalization token. </summary>
		///
		/// <remarks>	This function assigns a color lookup table finalization token as the color lookup 
		///				table that is utilized by this IGW3DSlopeAnalysis. A color lookup finalization
		///				token is a finalized configuration of an IGW3DColorLookupTablePtr, which is created 
		///				using the IGW3DSceneGraphContext interface. Needs to be assigned in order to visualize
		///				the test. </remarks>
		///
		/// <param name="color_lut">	The color lookup table. </param>
		/// 
		/// <see cref="IGW3DSceneGraphContext, IGW3DColorLookupTablePtr"/>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_GW3DFinalizationToken(IGW3DColorLookupTableFinalizationTokenPtr color_lut) = 0;

#ifdef SWIGGING
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a camera to the analysis (like viewshed). </summary>
	///
	/// <remarks>	Adds a camera to the analysis, causing visual results to be rendered on this
	///				camera.  No results  will be visible in the 3D scene unless a camera is added.
	///
	///				TODO: this will probably become a collection so that one can query which camera
	///				was added. </remarks>
	///
	/// <param name="p">	The camera to add. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void put_AddCamera(IGW3DCameraWPtr camera) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Removes a camera from the viewshed. </summary>
	///
	/// <remarks>	Removes a camera from the viewshed, causing it to no longer be rendered for that
	/// 			camera. </remarks>
	///
	/// <param name="p">	The camera to remove. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void put_RemoveCamera(IGW3DCameraWPtr camera) = 0;
#endif

	};
}