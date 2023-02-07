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
    /// <summary>	Collection of images with user defined attributes. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DImageCollection : public IGW3DCollection< IGW3DImageWPtr >
    {
        virtual ~IGW3DImageCollection() {}

#ifndef SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <param name="index">	Zero-based index of the image. </param>
        ///
        /// <returns>	The indexed value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DImageWPtr operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets at index. </summary>
        ///
        /// <param name="index">	Zero-based index of the image. </param>
        ///
        /// <returns>	at index. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DImageWPtr get_AtIndex(unsigned long index) const = 0;

#endif //SWIGGING

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates an image object from the specified file. </summary>
        ///
        /// <remarks>	Creates an image object. </remarks>
        ///
        /// <param name="filename"> TODO </param>
        ///
        /// <returns>   The image, or null if there was an error loading </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DImageWPtr create(const char* filename) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates an image object with the specified width and height. </summary>
        ///
        /// <remarks>	Creates an image object. The object's buffer will be zeroed out. </remarks>
        ///
        /// <param name="name">	A name for the new image object </param>
        /// <param name="width">	width of the image object. </param>
        /// <param name="height">	height of the image object. </param>
        /// <param name="has_alpha">	True if the image has an alpha channel. False otherwise. </param>
        ///
        /// <returns>   The image, or null if there was an error creating </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DImageWPtr create(const char* name, int width, int height, bool has_alpha) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Collection Width & Height Mode. </summary>
        ///
        /// <remarks>	A mode specifying how images in the collection should be scaled. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum CollectionWidthHeightMode
        {
            /// <summary>	An enum constant representing the automatic mode, where images will be
            /// 			automatically indexed based on true dimensions. The largest texture in the 
            ///				collection determines the amount of texture storage that gets consumed. </summary>
            AUTO = 0,
            /// <summary>	An enum constant representing the forced mode, where all images will be
            /// 			scaled to the width and height specified by put_Width and put_Height. </summary>
            FORCED = 1,
            /// <summary>	An enum constant representing the clamped mode, where all images will be
            /// 			automatically index based on true dimensions. Images that exceed the width
            ///				and height specified by put_Width and put_Height will be down scaled. </summary>
            CLAMPED = 2,
            /// <summary>	Do Not Use. </summary>
            MAX_MODE = 0xFF
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the collection's width and height mode. </summary>
        ///
        /// <remarks>	Determines how images inside the collection will be scaled.  For GPU optimization
        /// 			purposes, all images consume texture memory equal to the largest image size. </remarks>
        ///
        /// <param name="collectionmode">	The width and height mode. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_CollectionWidthHeightMode(CollectionWidthHeightMode collectionmode) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the collection's width and height mode. </summary>
        ///
        /// <returns>	The width and height mode. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual CollectionWidthHeightMode get_CollectionWidthHeightMode() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets current width. </summary>
        ///
        /// <remarks>	Gets the current width of the image, with scaling applied. </remarks>
        ///
        /// <returns>	The current width. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_CurrentWidth() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets current height. </summary>
        ///
        /// <remarks>	Gets the current height of the image, with scaling applied. </remarks>
        ///
        /// <returns>	The current height. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_CurrentHeight() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the image width. </summary>
        ///
        /// <remarks>	Sets the image width, according to the behavior defined by the
        /// 			CollectionWidthHeightMode.  This setting is ignored if the CollectionWidthHeightMode
        /// 			is AUTO. </remarks>
        ///
        /// <param name="width">	The image width. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Width(int width) = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the image height. </summary>
        ///
        /// <remarks>	Sets the image height, according to the behavior defined by the
        /// 			CollectionWidthHeightMode.  This setting is ignored if the CollectionWidthHeightMode
        /// 			is AUTO.  </remarks>
        ///
        /// <param name="height">	The image height. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Height(int height) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the maximum number of images allowed in the collection. </summary>
        ///
        /// <remarks>	Gets the maximum total number of IGW3DImageWPtr that may exist in the
        /// 			collection. </remarks>
        ///
        /// <returns>	Maximum allowed number of images </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_MaximumAllowableImages() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the maximum image size allowed on this GPU. </summary>
        ///
        /// <remarks>	Gets the maximum image size allowed on the current graphics processing unit (GPU).
        /// 			Width and height of images may not exceed this value in pixels. </remarks>
        ///
        /// <returns>	max allowed image width/height </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_MaximumAllowableImageSize() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets an image from the collection by name. </summary>
        ///
        /// <remarks>	Searches the collection for an image with a matching name. </remarks>
        ///
        /// <param name="name">	The image name (usually the filepath). </param>
        ///
        /// <returns>	The image, or an expired pointer if a matching image was not found. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DImageWPtr get_ByName(const char* name) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets an image by property collection identifier. </summary>
        ///
        /// <remarks>	Returns the image with the given property collection ID. </remarks>
        ///
        /// <param name="id">	The identifier. </param>
        ///
        /// <returns>	The image, or an expired pointer if no such property collection ID exists in the
        /// 			collection. </returns>
        ///
        /// <see cref="get_ByPropertyCollectionID"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DImageWPtr get_ByPropertyCollectionID(int id) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a finalization token. </summary>
        ///
        /// <remarks>	Once an image collection is populated and configured as desired, create a
        /// 			finalization token for use in the Geoweb3d SDK, such as a representation.
        /// 			Subsequent changes to the collection and its images will have no effect on the
        /// 			configuration represented by the token. </remarks>
        ///
        /// <returns>	The new finalization token. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DFinalizationTokenPtr create_FinalizeToken() = 0;
    };
}