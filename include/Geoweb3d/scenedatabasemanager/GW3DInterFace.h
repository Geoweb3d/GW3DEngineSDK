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
#include <Geoweb3d/Core/GW3DInterFace.h>
#include <Geoweb3dCore/GW3DResults.h>
#include "stdafx.h"

namespace Geoweb3d
{
    enum SearchStatus
    {
        IDLE = 0,
        ACTIVE,
        COMPLETED,
        CANCELLED,
        INCOMPLETE
    };

    struct IGW3DSceneDatabaseSDK;
    _GW3D_SMARTPTR_TYPEDEF( IGW3DSceneDatabaseSDK );

    struct IGW3DSceneDatabaseDataSource;
    _GW3D_SMARTPTR_TYPEDEF      ( IGW3DSceneDatabaseDataSource );
    _GW3D_WEAK_SMARTPTR_TYPEDEF ( IGW3DSceneDatabaseDataSource );

    struct IGW3DSceneDatabaseDataSourceCollection;
    _GW3D_SMARTPTR_TYPEDEF      ( IGW3DSceneDatabaseDataSourceCollection );
    _GW3D_WEAK_SMARTPTR_TYPEDEF ( IGW3DSceneDatabaseDataSourceCollection );

    struct IGW3DSceneDatabaseManager;
    _GW3D_SMARTPTR_TYPEDEF      ( IGW3DSceneDatabaseManager );
    _GW3D_WEAK_SMARTPTR_TYPEDEF ( IGW3DSceneDatabaseManager );

}
