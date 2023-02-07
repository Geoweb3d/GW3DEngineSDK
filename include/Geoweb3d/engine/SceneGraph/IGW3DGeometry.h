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
#include "../../core/GW3DInterFace.h"

namespace Geoweb3d
{

//Here and not in the interface as this is not implemented stuff.
struct	IGW3DGeometry;
_GW3D_SMARTPTR_TYPEDEF( IGW3DGeometry );
extern "C++"
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGeometry : public IGW3DSpatial
	{
		virtual ~IGW3DGeometry() {}

	}
}
}


