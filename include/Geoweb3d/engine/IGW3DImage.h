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
#include "../core/IGW3DClientUserData.h"
#include <Geoweb3dCore/GW3DResults.h>

namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	An image. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DImage : public IGW3DClientUserData
    {
        virtual ~IGW3DImage() {}

        //////////////////////////////////////////////////////////////////////////////////////////////////// 
        /// <summary> TODO </summary>
        //////////////////////////////////////////////////////////////////////////////////////////////////// 

        enum ANCHOR_POSITION
        {
            /// <summary>	The 9 anchor positions the billboard can possess. </summary>
            TOP_LEFT = 0,
            TOP_CENTER = 1,
            TOP_RIGHT = 2,
            MIDDLE_LEFT = 3,
            MIDDLE_CENTER = 4,
            MIDDLE_RIGHT = 5,
            BOTTOM_LEFT = 6,
            BOTTOM_CENTER = 7,
            BOTTOM_RIGHT = 8,
            /// <summary>	APM_MAX - Can be used for looping over the modes. </summary>
            APM_MAX,
            /// <summary>	APM_MAX_DATA_SIZER - Do Not Use </summary>
            APM_MAX_DATA_SIZER = 0xFFFF
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the image name. </summary>
        ///
        /// <returns>	the image name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_ImageName() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query whether or not the image has an alpha channel. </summary>
        ///
        /// <remarks>	If false, the image is a 3 channel image, otherwise it is a 4 channel image.  If
        /// 			the image was added to a collection an IGW3DImageCollection and finalized, it may
        /// 			get changed from a 3 channel to a 4 channel image for GPU optimizations. </remarks>
        ///
        /// <returns>	true if it has an alpha channel, false if it does not. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_HasAlpha() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Save the image to disk. </summary>
        ///
        /// <remarks>	Saves the image to disk with the given filename.  The file extension is used to
        /// 			determine which driver to use.  This will NOT change the current filename. </remarks>
        ///
        /// <param name="file">	The output filename. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_ToDisk(const char* file) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Scale the image. </summary>
        ///
        /// <remarks>	Scales the image to the given width and height.  If the image is part of a
        /// 			collection, the collection manages the scale and the function will return an error. </remarks>
        ///
        /// <param name="width"> 	The width. </param>
        /// <param name="height">	The height. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_ScaleImage(unsigned  width, unsigned height) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the original width. </summary>
        ///
        /// <remarks>	Gets the original width of the image, prior to any scaling. </remarks>
        ///
        /// <returns>	The original width. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_OriginalWidth()  const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets original height. </summary>
        ///
        /// <remarks>	Gets the original height of the image, prior to any scaling. </remarks>
        ///
        /// <returns>	The original height. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_OriginalHeight() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the finalized scaled width. </summary>
        ///
        /// <remarks>	Gets the finalized scaled width of the image.  For an image in an image collection,
        /// 			pending how the collection is configured, these values will be the actual value
        ///				used within the sdk. </remarks>
        ///
        /// <returns>	The finalized scaled width. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_FinalizedScaledWidth()  const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the  scaled height. </summary>
        ///
        /// <remarks>	Gets the finalized scaled height of the image.  For an image in an image collection,
        /// 			pending how the collection is configured, these values will be the actual value
        ///				used within the sdk. </remarks>
        ///
        /// <returns>	The finalized scaled height. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_FinalizedScaledHeight() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Flip the image horizontally, around s dimension. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_FlipHorizontal() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Flip the image vertically, around t dimension. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_FlipVertical() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        ///	<summary>	Gets the raw pixels </summary>
        ///
        ///	<remarks>	[get_Buffer] is *CURRENTLY ONLY SUPPORTED* if this object came from IRasterLayer API :
        ///				StreamEnvelope/StreamEnvelopeAync OR if this object came *from* [create_Image] 
        ///				within IGW3DMediaCenter. This API will change in the future. IGW3DMediaCenter only 
        ///				creates RGB channels right now, so you have raw RGB pixels in the buffer.  
        ///	
        ///				Note: Stride is the width. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DBuffer* get_Buffer() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        ///	<summary>	Gets the raw pixels </summary>
        ///
        ///	<remarks>	[get_Buffer] is *CURRENTLY ONLY SUPPORTED* if this object came from IRasterLayer API :
        ///				StreamEnvelope/StreamEnvelopeAync OR if this object came *from* [create_Image] 
        ///				within IGW3DMediaCenter. This API will change in the future. IGW3DMediaCenter only 
        ///				creates RGB channels right now, so you have raw RGB pixels in the buffer.  
        ///	
        ///				Note: Stride is the width. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DBuffer* get_Buffer() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the property collection identifier. </summary>
        ///
        /// <remarks>	After a image property collection is finalized, the finalization token can be
        /// 			assigned to one or more representations.  The property collection ID is the
        /// 			reference to the image from within the representation properties (for example
        /// 			the side texture for an extruded polygon representation. </remarks>
        ///
        /// <returns>	The property collection identifier. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_PropertyCollectionID() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Anchor Position of the Image. </summary>
        ///
        /// <returns>	The Anchor Position. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual ANCHOR_POSITION get_AnchorPosition() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the Anchor Position of the Image. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_AnchorPosition(ANCHOR_POSITION position) = 0;

    };
}