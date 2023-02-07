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
	/// <summary>	Collection of . </summary>
	///
	/// <remarks>	This collection contains all, and is also where new terrain avoidance tests can
	/// 			be created. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DTerrainAvoidanceTestCollection : public IGW3DCollection< IGW3DTerrainAvoidanceTestWPtr >
	{
		virtual ~IGW3DTerrainAvoidanceTestCollection() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a terrain avoidance test. </summary>
		///
		/// <remarks>	A color lookup table needs to be assigned to the Terrain Avoidance Test in order
		///				to visualize the test. See IGW3DTerrainAvoidanceTest::put_GW3DFinalizationToken 
		///				for further details. </remarks>
		///
		/// <returns>	A terrain avoidance test. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DTerrainAvoidanceTestWPtr create() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables/disables all the tests. </summary>
		///
		/// <remarks>	Globally enables or disables all the terrain avoidance tests. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_EnableAll(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="vtest"></param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close(IGW3DTerrainAvoidanceTestWPtr& vtest)
		{
			close_TerrainAvoidanceTest(vtest);
		}
	};
}