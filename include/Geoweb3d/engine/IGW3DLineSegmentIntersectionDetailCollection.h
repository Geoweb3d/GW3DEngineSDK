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
	/// <summary>	Collection of intersection details. </summary>
	///
	/// <remarks>	A sortable collection of intersection details reported by a line segment
	/// 			intersection test. </remarks>
	///
	///  <see cref="IGW3DLineSegmentIntersectionTest"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DLineSegmentIntersectionDetailCollection : public IGW3DCollection< IGW3DLineSegmentIntersectionDetail* >
	{
		virtual  ~IGW3DLineSegmentIntersectionDetailCollection() {};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent sorting direction. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum SortType
		{
			/// <summary> Ascending distance (near to far) </summary>
			Ascending = 0,
			/// <summary> Descending distance (far to near) </summary>
			Descending = 1,
			/// <summary> TODO </summary>
			SortMax = 0xFFFF
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the sort operation. </summary>
		///
		/// <remarks>	Sorts the results based distance. </remarks>
		///
		/// <param name="sort_type">	The sort type. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void do_Sort(SortType sort_type) = 0;
	};
}