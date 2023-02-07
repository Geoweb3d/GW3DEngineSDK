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
#include <Geoweb3dCore/GW3DResults.h>

namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of vector drivers. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DVectorDriverCollection : public IGW3DCollection< IGW3DVectorDriverWPtr >
    {
        virtual ~IGW3DVectorDriverCollection(){}
#ifndef SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the vector driver. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorDriverWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the vector driver. </param>
        ///
        /// <returns>	at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorDriverWPtr get_AtIndex(unsigned long index) const = 0;
#endif //SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets a driver by name. </summary>
        ///
        /// <param name="name">	The name. </param>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDriverWPtr get_Driver(const char* name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Auto-open a vector datasource. </summary>
        ///
        /// <remarks>	Automatically determines the correct driver for the data and opens it with that
        /// 			driver.
        ///
        /// 			If you do not want to open a datasource through IGW3DVectorDriver, if you don't
        /// 			know what driver to use to open a datasource, or if you are just not sure if the
        /// 			SDK can open something, you can use this API where the sdk will do its best to
        ///				automatically find an IGW3DVectorDriver and open the datasource for you, returning a
        ///				IGW3DVectorDataSource. If you want to see what driver ended up getting used, that
        ///				can be queried from the IGW3DVectorDataSource.
        ///
        ///				pszName can be any string supported by GDAL, but is typically a filepath or url. </remarks>
        ///
        /// <param name="pszName">				The name (i.e. the filepath or url). </param>
        /// <param name="result">				Returns whether the returned datasource was already opened.  </param>
        /// <param name="driver_properties">	(optional) the driver properties. </param>
        ///
        /// <returns>	The vector datasource. </returns>
        ///
        /// <see cref="IGW3DVectorDataSourceCollection::open"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDataSourceWPtr auto_Open(const char* pszName, GW3DResult& result, const IGW3DPropertyCollectionPtr& driver_properties = IGW3DPropertyCollectionPtr()) = 0;

        //currently only CDB is supported for auto_CreateSummary
        //virtual IGW3DVectorDataSourceSummaryPtr auto_CreateSummary( const char *  pszName, const IGW3DPropertyCollectionPtr &driver_properties = IGW3DPropertyCollectionPtr() ) = 0;
    };
}