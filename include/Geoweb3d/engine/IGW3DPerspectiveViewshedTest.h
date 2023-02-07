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
#include "IGW3DViewshedTest.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A Perspective viewshed analysis. </summary>
	///
	/// <remarks>	The Geoweb3d viewshed provides a visual display of surfaces that are visible and/or
	/// 			non-visible from a defined position, orientation and field of view. The display of
	/// 			viewshed is divided into two regions: the visible and the hidden region.  The
	/// 			visible region represents all terrain and geometry surfaces that are visible from
	/// 			the viewshed origin, whereas the hidden region represents all terrain and
	/// 			geometry surfaces that are not visible from the viewshed origin. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DPerspectiveViewshedTest : public IGW3DViewshedTest
	{
		virtual ~IGW3DPerspectiveViewshedTest() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the viewshed frustum. </summary>
		///
		/// <returns>	the frustum. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DViewshedFrustum* get_Frustum() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the viewshed frustum. </summary>
		///
		/// <returns>	the frustum. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DViewshedFrustum* get_Frustum() const = 0;
	};
}