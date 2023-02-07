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
#include "GW3DExports.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of vector data sources. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DVectorDataSourceCollection : public IGW3DCollection< IGW3DVectorDataSourceWPtr >
    {
        virtual ~IGW3DVectorDataSourceCollection(){}

#ifndef SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the datasource. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorDataSourceWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets datasource at the given index. </summary>
        ///
        /// <param name="index">	Zero-based index of the datasource. </param>
        ///
        /// <returns>	the datasource. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorDataSourceWPtr get_AtIndex(unsigned long index) const = 0;

#endif //SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a vector datasource. </summary>
        ///
        /// <param name="pszName">   	The name. </param>
        /// <param name="properties">	the properties. </param>
        ///
        /// <returns>	The new vector datasource. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDataSourceWPtr create(const char* pszName, const IGW3DPropertyCollectionPtr& properties = IGW3DPropertyCollectionPtr()) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a vector datasource. </summary>
        ///
        /// <param name="pszName">   	The name. </param>
        /// <param name="properties">	the properties. </param>
        ///
        /// <returns>	The new vector datasource. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDataSourceWPtr create(const char* pszName, const IGW3DPropertyCollection* properties) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Deletes the datasource from disk (if capable) </summary>
        ///
        /// <param name="pszName">	The name to destroy. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult destroy(const char* pszName) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Opens a vector datasource. </summary>
        ///
        /// <remarks>	pszName can be any string supported by GDAL, but is typically a filepath or url. </remarks>
        ///
        /// <param name="pszName">   	The name (i.e. filepath or url). </param>
        /// <param name="result">		Returns whether the returned datasource was already opened.  </param>
        /// <param name="properties">	(optional) the properties. </param>
        ///
        /// <returns>	The vector datasource. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDataSourceWPtr open(const char* pszName, GW3DResult& result, const IGW3DPropertyCollectionPtr& properties = IGW3DPropertyCollectionPtr()) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets by name. </summary>
        ///
        /// <remarks>	Gets a vector datasource that has already been created or opened with this driver. </remarks>
        ///
        /// <param name="name">	The name. </param>
        ///
        /// <returns>	The vector datasource. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDataSourceWPtr get_ByName(const char* name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the driver. </summary>
        ///
        /// <remarks>	gets the driver that was used to open or create this datasource. </remarks>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDriverWPtr get_Driver() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes the given ds. </summary>
        ///
        /// <remarks>	Closes the given datasource and removes it from the collection.
        ///
        /// 			To close a datasource: IGW3DVectorDataSourceCollection::close( window );
        ///
        ///				IGW3DVectorDataSourceWPtr will become expired. </remarks>
        ///
        /// <param name="ds">	[in] The datasource. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static void close(IGW3DVectorDataSourceWPtr& ds)
        {
            close_VectorDataSource(ds);
        }
    };
}