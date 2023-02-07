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
#include "../core/IGW3DCollection.h"

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of camera frustum intersection stream results. </summary>
	///
	/// <remarks>	This collection is only populated in IGW3DFrustumAnalysisStream::OnStream when 
	///				the representation being reported has 
	///				IGW3DFeatureID2DPixelLocationConfiguration::put_Enabled	set to true.
	///				
	///				ONLY supported in representations: model, custom, all shapes, range ring. </remarks>
	///				
	/// <see cref="IGW3DFrustumAnalysisStream"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DFeatureFeedbackResultCollection : public IGW3DCollection< IGW3DFeatureFeedbackResult* >
	{
		virtual ~IGW3DFeatureFeedbackResultCollection() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query whether IGW3DFeatureID2DPixelLocationConfiguration::put_Enabled is set 
		///				to true on the representation being reported. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_WasEnabled() const = 0;

	};
}