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

struct	IGW3DVec2fBufferObject;
_GW3D_SMARTPTR_TYPEDEF( IGW3DVec2fBufferObject );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Vec2fRaw
{
    float x;
    float y;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DVec2fBufferObjectCollection : public IGW3DCollection< Vec2fRaw* >, public IGW3DIBaseObject
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DVec2fBufferObject
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> TODO </summary>
    ///
    /// <returns> TODO </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual IGW3DVec2fBufferObjectCollection * get_Collection(  ) = 0;
};
}