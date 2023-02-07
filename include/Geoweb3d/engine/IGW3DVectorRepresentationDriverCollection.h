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

namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of vector representation drivers. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorRepresentationDriverCollection : public IGW3DCollection< IGW3DVectorRepresentationDriverWPtr >
    {
        virtual ~IGW3DVectorRepresentationDriverCollection(){}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the vector representation driver. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorRepresentationDriverWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the vector representation driver. </param>
        ///
        /// <returns>	at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorRepresentationDriverWPtr get_AtIndex(unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets a driver by name. </summary>
        ///
        /// <param name="name">	The name. </param>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationDriverWPtr get_Driver(const char* name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Streams the given camera frustum intersection stream. </summary>
        ///
        /// <remarks>	Results are filled during a draw. Note that results for all cameras and windows are
        /// 			cleared on the next draw (every frame).  Thus, its important that you stream out the
        /// 			results after you call draw on any window.
        ///
        /// 			Currently only cameras on the window drawn will get filled.  This means if you have
        /// 			5 windows and each window has 1 camera, only the camera on the window drawn last
        /// 			will have results.  </remarks>
        ///
        /// <param name="streamobj">	[in] The frustum analysis stream callback object. </param>
        ///
        /// <see cref="IGW3DVectorRepresentation::get_CameraFrustumContentsConfiguration"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void Stream(IGW3DFrustumAnalysisStream* streamobj) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sometimes datasources describe their scenes, and one does not want to use
        //				this as a "map" per say, so they can simply load the data and it will
        //				auto represent based off the specification of the datasource. </summary>
        ///
        /// <remarks>	Results are the representations added.  This is a container you own, but
        //				its out of convenience, as the representations are added to the regular 
        //				representation collection just as if done manually. </remarks>
        ///
        /// <param name="datasource"> TODO </param>
        /// <param name="page_policy"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationHelperCollectionPtr auto_Open(IGW3DVectorDataSourceWPtr datasource, const Vector::IGW3DLODPagingPolicy* page_policy = nullptr) = 0;
    };
}