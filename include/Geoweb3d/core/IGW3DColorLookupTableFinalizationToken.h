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
#include "IGW3DToken.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Color Lookup Table finalization token. </summary>
	///
	/// <remarks>	A token containing information about a color lookup table. </remarks>
	///
	/// <see cref="Geoweb3d::IGW3DColor"/>
	/// <see cref="Geoweb3d::IGW3DColorCollection"/>
	/// <see cref="Geoweb3d::IGW3DColorLookupTable::create_FinalizeToken"/>
	/// <see cref="Geoweb3d::IGW3DViewshedTest::put_GW3DFinalizationToken"/>
	/// <see cref="Geoweb3d::IGW3DTerrainAvoidanceTest::put_GW3DFinalizationToken"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DColorLookupTableFinalizationToken : public IGW3DToken
	{

	};
}