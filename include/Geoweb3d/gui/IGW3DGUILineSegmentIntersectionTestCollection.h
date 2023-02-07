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
#include "core/GW3DGUIInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{

namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A subset of the intersection test containing just those test with results in the 
	///				previously drawn frame </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUILineSegmentIntersectionTestResultsCollection: public IGW3DCollection< IGW3DGUILineSegmentIntersectionTestWPtr >
	{
	
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of  line segment intersection tests. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUILineSegmentIntersectionTestCollection: public IGW3DCollection< IGW3DGUILineSegmentIntersectionTestWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Create a new 3D intersection test. </summary>
		///
		/// <returns>	A new intersection test. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUILineSegmentIntersectionTestWPtr create() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	a global way to enable/disable all the queries. </summary>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_EnableAll( bool enable ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets intersection results for the previously drawn frame. </summary>
		///
		/// <returns>	null if it fails, else the intersection results. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUILineSegmentIntersectionTestResultsCollection * get_IntersectionResultsCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Closes the given test. </summary>
		///
		/// <param name="ltest">	The test. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close( IGW3DGUILineSegmentIntersectionTestWPtr ltest  )
		{
			//todo
		}

		
	};

}
}