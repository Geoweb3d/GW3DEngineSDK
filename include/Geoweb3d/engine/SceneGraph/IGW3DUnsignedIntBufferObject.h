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

namespace Geoweb3d
{

struct	IGW3DUnsignedIntBufferObject;
_GW3D_SMARTPTR_TYPEDEF( IGW3DUnsignedIntBufferObject );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DUnsignedIntCollection : public IGW3DCollection< unsigned int* >, public IGW3DIBaseObject
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DUnsignedIntBufferObject
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> TODO </summary>
    ///
    /// <returns> TODO </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual IGW3DUnsignedIntCollection * get_Collection(  ) = 0;
};
}


