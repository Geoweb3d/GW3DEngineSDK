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

//SKIP-CODE-BEGIN

struct	IGW3DRenderState;
_GW3D_SMARTPTR_TYPEDEF( IGW3DRenderState );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DRenderState
{

    //maybe just these states for now...use our property object? tbd
    enum StateType
    {
        DEPTH,
        FRONT_FACE,
        CULL_FACE,
        POLYGON_MODE,
        POLYGON_OFFSET,
        COLOR,
        NORMAL,
        MAX_STATE_TYPE
    } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DRenderState
{

}

//SKIP-CODE-END
}

