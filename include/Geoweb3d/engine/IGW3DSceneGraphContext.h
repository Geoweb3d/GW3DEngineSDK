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

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	The scene graph context. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DSceneGraphContext
    {
        virtual ~IGW3DSceneGraphContext() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets file attributes. </summary>
        ///
        /// <remarks>	If a file has file attributes, they may be read using this API. </remarks>
        ///
        /// <param name="name">	The name (i.e. filepath). </param>
        ///
        /// <returns>	The file attributes. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPropertyCollectionPtr get_FileAttributes(const char* name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates an image collection. </summary>
        ///
        /// <remarks>	Creates an image collection for use in other parts of the Geoweb3d SDK, such as
        /// 			assigning images to a representation. </remarks>
        ///
        /// <returns>	The new image collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DImageCollectionPtr create_ImageCollection() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a OpenGL collection. </summary>
        ///
        /// <remarks>	Creates an opengl collection for use where the texture id will be supplied
        //				external to the Geoweb3d sdk</remarks>
        ///
        /// <returns>	The new collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // NOTE If you plan to share texture IDs, you need to be using or at least be sharing
        // the sdk's opengl context.  Otherwise texture IDs between us will not be able to
        // be shared

        virtual IGW3DOpenGLTextureCollectionPtr create_OpenGLTextureCollection() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a color lookup table with the specified range. </summary>
        ///
        /// <remarks>	Creates a color lookup table for use in other parts of the Geoweb3d SDK, such as
        /// 			assigning color lookup tables to TerrainAvoidanceTests or ViewshedTests during slope analysis. </remarks>
        ///
        /// <param name="min_value">	The upper bound of the table. </param>
        /// <param name="max_value">	The lower bound of the table. </param>
        ///
        /// <returns>	The new color lookup table. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DColorLookupTablePtr create_ColorLookupTable(int min_value, int max_value) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a georeferenced layer image collection. </summary>
		///
		/// <remarks>	Creates a georeferenced image collection for use in other parts of the Geoweb3d SDK, such as
		/// 			the imagery overlay system. </remarks>
		///
		/// <returns>	The new image collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterLayerImageCollectionPtr create_RasterLayerImageCollection() = 0;
    };
}