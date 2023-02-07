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

	////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Frame analysis contents configuration. </summary>
	///
	/// <remarks> Creates a report for each camera as the 3d window is
	///           drawn, which should be read back before the next window draw. </remarks>
	///
	/// <see cref="IGW3DVectorRepresentation::get_FrameAnalysisContentsConfiguration"/>
	/// <see cref="IGW3DFrustumAnalysisStream"/>                                                                           
	/// /////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DFrameAnalysisContentsConfiguration
	{
		virtual ~IGW3DFrameAnalysisContentsConfiguration(){}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		///	<summary>	Configuration of the frustum analysis </summary>
		///
		///	<remarks>	This analysis works off the area of interest around a camera, i.e. data that is 
		///				paged in. </remarks>
		///
		///	<see cref="IGW3DFrustumAnalysisStream"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DFrustumAnalysis* get_FrustumAnalysis() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		///	<summary>	Configuration of the frustum analysis </summary>
		///
		///	<remarks>	This analysis works off the area of interest around a camera, i.e. data that is 
		///				paged in. </remarks>
		///
		///	<see cref="IGW3DFrustumAnalysisStream"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DFrustumAnalysis* get_FrustumAnalysis() const = 0;
	};
}