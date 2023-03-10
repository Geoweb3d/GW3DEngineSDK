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

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Plugin registration. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class IGW3DRegistration
    {
    public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Adds a supported class interface. </summary>
        ///
        /// <param name="guid">	Unique identifier. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void add_SupportedClassInterface(const GW3DGUID& guid) = 0;
    };

#define GW3D_API_VERISON .5

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Defines an alias representing information describing the plugin metadata. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct tagGW3DMetaData
    {
        /// <summary>	Name of the plugin. </summary>
        const char* name;
        /// <summary>	The author. </summary>
        const char* author;
        /// <summary>	The description (may be multiple lines). </summary>
        const char* description;
        /// <summary>	The website, if any. </summary>
        const char* web;
        /// <summary>	Unique identifier for the plugin. </summary>
        const char* guid;
        /// <summary>	The plugin version. </summary>
        float   plugin_version;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Base (GW3DSDKCore) API version the plugin was compiled against. You should be using the
        ///				#defines in the registration interface to set this. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        float gw3dapi_version;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Plugin Metadata. </summary>
        ///
        /// <returns>	The plugin metadata pointer. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

    } GW3DMetaData, * GW3DMetaDataPtr;
}