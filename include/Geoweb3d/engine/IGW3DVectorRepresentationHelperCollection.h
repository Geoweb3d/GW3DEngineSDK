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
#include "../core/GW3DGuids.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of vector representations allowing adding/removal. </summary>
    ///
    /// <remarks>	A collection of vector representations with the capability to add and remove
    /// 			representations. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorRepresentationHelperCollection : public IGW3DCollection< IGW3DVectorRepresentationWPtr >
    {
        virtual ~IGW3DVectorRepresentationHelperCollection(){}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Adds a representation. </summary>
        ///
        /// <param name="rep">	The representation. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult add(IGW3DVectorRepresentationWPtr rep) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Removes a representation. </summary>
        ///
        /// <param name="rep">	The representation. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult remove(IGW3DVectorRepresentationWPtr rep) = 0;
    };
}