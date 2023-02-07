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
	/// <summary>	The	configuration of 2D pixel location reporting	</summary>
	///
	/// <remarks>	2D pixel locations of individual features may be reported.  This is useful for 
	///				rendering of icons and other 2D drawing via the IGW3DWindowCallback. 
	///
	///				ONLY supported in representations: model, custom, all shapes, range ring. </remarks>
	///
	/// <see cref="IGW3DFeatureFeedbackConfiguration::get_FeatureID2DPixelLocationConfiguration/>
	///	<see cref="IGW3DFeatureFeedbackResult::get_FeatureID2DPixelLocationResult"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DFeatureID2DPixelLocationConfiguration
	{
		virtual ~IGW3DFeatureID2DPixelLocationConfiguration() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query whether 2D pixel locations will be reported. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enable/disable the reporting of 2D pixel locations. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Enabled(bool enable) = 0;
	};
}