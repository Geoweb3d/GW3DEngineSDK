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
	/// <summary>	Collection of intersection reports from an intersection test. </summary>
	///
	/// <see cref="IGW3DLineSegmentIntersectionTest"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DLineSegmentIntersectionReportCollection : public IGW3DCollection< IGW3DLineSegmentIntersectionReport* >
	{
		virtual ~IGW3DLineSegmentIntersectionReportCollection() {}
	};
}