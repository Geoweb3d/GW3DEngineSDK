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
	/// <summary>	Image finalization token. </summary>
	///
	/// <remarks>	A token containing information about a palette of images. </remarks>
	///
	/// <see cref="Geoweb3d::IGW3DImage"/>
	/// <see cref="Geoweb3d::IGW3DImageCollection::create_FinalizeToken"/>
	/// <see cref="Geoweb3d::IGW3DVectorRepresentation::put_GW3DFinalizationToken"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DFinalizationToken : public IGW3DToken
	{
		virtual ~IGW3DFinalizationToken() {}

	};
}