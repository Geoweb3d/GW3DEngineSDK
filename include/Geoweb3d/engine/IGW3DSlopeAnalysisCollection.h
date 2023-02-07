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
#include "GW3DExports.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of slope analyses. </summary>
	///
	/// <remarks>	This collection contains all slope analyses, and is also where a new slope analysis can
	/// 			be created. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DSlopeAnalysisCollection : public IGW3DCollection< IGW3DSlopeAnalysisWPtr >
	{
		virtual ~IGW3DSlopeAnalysisCollection() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a slope analysis. </summary>
		///
		/// <remarks>	Creates a new slope analysis.  Note that the slope analysis will not be visible until it is
		/// 			positioned and one or more cameras have been added to it. </remarks>
		///
		/// <returns>	A slope analysis. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DSlopeAnalysisWPtr create() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables/disables all the slope analyses. </summary>
		///
		/// <remarks>	Globally enables or disables all the slope analysis. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_EnableAll(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Closes the given slope analysis. </summary>
		///
		/// <remarks>	Closes the given slope analysis and removes it from the collection.
		///
		/// 			To close a slope analysis: IGW3DSlopeAnalysisCollection::close( vtest );
		///
		///				IGW3DSlopeAnalysisWPtr will become expired.</remarks>
		///
		/// <param name="vtest"> the slope analysis</param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close(IGW3DSlopeAnalysisWPtr& vtest)
		{
			close_SlopeAnalysis(vtest);
		}
	};
}