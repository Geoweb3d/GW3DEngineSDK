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

/* Primary namespace */
namespace Geoweb3d
{

    //// todo, but in a header, using this idea we can combine 2d,3d and multirender passes one day instead of individual callbacks

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // <summary> TODO </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DRenderPassDetails
    {
        virtual ~IGW3DRenderPassDetails() {}

    };
};