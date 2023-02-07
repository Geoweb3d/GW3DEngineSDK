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

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection stereo drivers. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DStereoDriverCollection : public IGW3DCollection< IGW3DStereoDriverWPtr >
    {
        virtual ~IGW3DStereoDriverCollection(){}

#ifndef SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the stereo driver. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DStereoDriverWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the stereo driver. </param>
        ///
        /// <returns>	null if it fails, else at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DStereoDriverWPtr get_AtIndex(unsigned long index) const = 0;
#endif //SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Search for available drivers.</summary>
        ///
        /// <remarks>	If for some reason you should have active stereo capabilities but they are not
        /// 			detected with the current OS configuration, force_Search will going through all
        /// 			the OS desktop display modes looking for a supported display mode.  Display may
        /// 			flicker!  </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_Search() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a Stereo Driver which allows an sdk user to integrate a stereo device .</summary>
        ///
        /// <remarks>	The user Inherits from IGW3DStereoDevice </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStereoDriverWPtr Create(IGW3DStereoDevicePtr device) = 0;

    };
}