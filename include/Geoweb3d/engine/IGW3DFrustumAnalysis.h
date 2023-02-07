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
	/// <summary>	Frustum analysis settings </summary>
	///
	/// <see cref="IGW3DFrameAnalysisContentsConfiguration::get_FrustumAnalysis"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DFrustumAnalysis
	{
		virtual ~IGW3DFrustumAnalysis() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the configuration of analysis results for individual features </summary>
		///
		/// <remarks>	When enabled, representation will provide feedback on individual features / objects
		///				in the analysis.  Basic information will always be provided, but information on 
		///				each feature allowed by the IGW3DContentsFilter will only be provided as long as 
		///				the IGW3DFeatureFeedbackConfiguration is enabled.
		///
		///				Additional aspects of how information is reported may also be configured.  For 
		///				example, reporting of 2D screenspace coordinates can be enabled, or the area within 
		///				which data is reported can be extended beyond the camera field of view. </remarks>
		///	
		///	<see cref="IGW3DVectorRepresentationDriverCollection::Stream"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DFeatureFeedbackConfiguration* get_FeatureFeedbackConfiguration() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the configuration of analysis results for individual features </summary>
		///
		/// <remarks>	When enabled, representation will provide feedback on individual features / objects
		///				in the analysis.  Basic information will always be provided, but information on 
		///				each feature allowed by the IGW3DContentsFilter will only be provided as long as 
		///				the IGW3DFeatureFeedbackConfiguration is enabled.
		///
		///				Additional aspects of how information is reported may also be configured.  For 
		///				example, reporting of 2D screenspace coordinates can be enabled, or the area within 
		///				which data is reported can be extended beyond the camera field of view. </remarks>
		///	
		///	<see cref="IGW3DVectorRepresentationDriverCollection::Stream"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DFeatureFeedbackConfiguration* get_FeatureFeedbackConfiguration() const = 0;
	};
}