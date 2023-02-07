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

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of line segment intersection tests that have results for the last frame. </summary>
	///
	/// <remarks>	This collection contains only those intersection tests that has results in the
	/// 			last frame.  This means you could theoretically have hundreds of intersection
	/// 			tests, where if only one test had an intersection, this collection would have a
	/// 			size of 1. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DLineSegmentIntersectionTestResultsCollection : public IGW3DCollection< IGW3DLineSegmentIntersectionTestWPtr >
	{
		virtual  ~IGW3DLineSegmentIntersectionTestResultsCollection() {};
	};
}