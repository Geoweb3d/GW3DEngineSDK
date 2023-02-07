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
#include "../core/IGW3DDriver.h"

namespace Geoweb3d
{

    //struct  GW3D_DLL IGW3DSceneGraphFactory
    //{
    //    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //    /// <summary>	Creates an image collection. </summary>
    //    ///
    //    /// <remarks>	Creates an image collection for use in other parts of the Geoweb3d SDK, such as
    //    /// 			assigning images to a representation. Same as the IGW3DSceneGraphContext. </remarks>
    //    ///
    //    /// <returns>	The new image collection. </returns>
    //    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //    virtual IGW3DImageCollectionPtr create_ImageCollection() = 0;
    //
    //	////////////////////////////////////////////////////////////////////////////////////////////////////
    //	/// <summary>	Creates a OpenGL collection. </summary>
    //	///
    //	/// <remarks>	Creates an opengl collection for use where the texture id will be supplied
    //	//				external to the Geoweb3d sdk. </remarks>
    //	///
    //	/// <returns>	The new collection. </returns>
    //	////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //	// NOTE If you plan to share texture IDs, you need to be using or at least be sharing
    //	// the sdk's opengl context.  Otherwise texture IDs between us will not be able to
    //	// be shared
    //
    //	virtual IGW3DOpenGLTextureCollectionPtr create_OpenGLTextureCollection() = 0;
    //
    //};

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Vector layer representation driver. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorRepresentationDriver : /*public IGW3DSceneGraphFactory, */ public IGW3DDriver
    {
        virtual ~IGW3DVectorRepresentationDriver() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the name. </summary>
        ///
        /// <remarks>	Gets the name of the representation driver. </remarks>
        ///
        /// <returns>	the name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_Name() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Check if a given vector layer can be represented. </summary>
        ///
        /// <remarks>	Check a vector layer to determine if this driver is capable of creating a
        /// 			representation of it. It does not check things like projection etc, as that call
        /// 			is on a layer itself. </remarks>
        ///
        /// <param name="layer">				The layer. </param>
        /// <param name="field_filter_mode">	(Optional) true to use the field filter mode. </param>
        ///
        /// <returns>	True if the layer can be represented, false if it cannot. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult get_CapabilityToRepresent(IGW3DVectorLayerWPtr layer, bool field_filter_mode = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the default properties of the representation driver. </summary>
        ///
        /// <returns>	null if it fails, else the properties. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DPropertyCollection* get_PropertyCollection() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the default symbol of the representation driver. </summary>
        ///
        /// <returns>	null if it fails, else the properties. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual const IGW3DSymbolProperties* get_SymbolProperties() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the representation collection. </summary>
        ///
        /// <remarks>	Gets the collection of all vector representations created using this driver.
        /// 			representations are also created and closed through the collection. </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	the representation layer collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationCollection* get_RepresentationLayerCollection(bool reset_next_iterator = false) = 0;
    };
}