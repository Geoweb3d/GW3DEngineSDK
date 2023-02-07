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
    /// <summary>	A media center task. </summary>
    ///
    /// <remarks>	A task for capturing an image. Tasks are asynchronous, but you may optionally wait
    /// 			for a task to complete by calling wait_ForCompletion in the Media Center API. </remarks>
    ///
    /// <see cref="IGW3DMediaCenter::wait_ForCompletion"/>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DMediaCenterTask
    {
        virtual ~IGW3DMediaCenterTask() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the status of the media center task. </summary>
        ///
        /// <remarks>	Gets the task status, i.e. if the task is complete, pending, canceled, of in an
        /// 			error state. </remarks>
        ///
        /// <returns>	The task status. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult get_Status() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the image. </summary>
        ///
        /// <remarks>	Gets the image captured by the media center task. </remarks>
        ///
        /// <returns>	The image, or an expired pointer if no image was captured. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DImageWPtr get_Image() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the image. </summary>
        ///
        /// <remarks>	Gets the image captured by the media center task. </remarks>
        ///
        /// <returns>	The image, or an expired pointer if no image was captured. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DImageWPtr get_Image() = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	The Geoweb3d Media Center API </summary>
    ///
    /// <remarks>	The media center allows thumbnail images to be created from raster and vector
    /// 			layers, as well as from 3D windows. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DMediaCenter
    {
        virtual ~IGW3DMediaCenter() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Create an image from an envelope and a raster representation. </summary>
        ///
        /// <remarks>	You can get pretty creative with this api.  For example, you could stream all the
        /// 			extruded polygons for generic buildings and pass in the envelope here to get back
        /// 			a roof texture.  Note, you pass in a collection of raster representations that
        /// 			will be used to create the images.  NOTE! the order in the collection is
        /// 			important if the rasters overlap, as the lower index will take priority, just as
        /// 			is the same behavior if adding rasters to our 3d scene.  Things to be aware of..
        /// 			if you plan to make an image palette, your width and height textures should
        /// 			always be the same to reduce extra work when creating the finalization token.
        /// 			You also need to be aware of the fact that the number of textures in an image
        /// 			collection is dependent on a GPU, so if you are creating thousands of images,
        /// 			make sure you properly spill over and make a new collection.  This will not ever
        /// 			create the perfect roof texture BTW, but it may be better than a generic if you
        /// 			are after realism, as well as when we start exporting 3D models will help the
        /// 			artist out as well. NOTE:  * API CURRENTLY NOT IMPLIMENTED *. </remarks>
        ///
        /// <param name="raster_reps">	The raster representations. </param>
        /// <param name="env">		  	The envelope/constraint to gather in WGS84. </param>
        /// <param name="max_width">  	The maximum image width. </param>
        /// <param name="max_height"> 	The maximum image height. </param>
        ///
        /// <returns>	A media center task that will create the new image. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DMediaCenterTaskPtr create_Image(IGW3DRasterRepresentationCollectionPtr raster_reps, const GW3DEnvelope* env, int max_width, int max_height) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates an image of a raster. </summary>
        ///
        /// <remarks>	Creates an image from a raster file, with the specified size.  The aspect ratio of
        /// 			the raster will be maintained, with the width and height not exceeding the maximum
        /// 			pixel size.  If filename is not null, the image will be written directly to disk
        /// 			at the given filepath. </remarks>
        ///
        /// <param name="rasterfilename">	The raster filename. </param>
        /// <param name="max_pixelsize"> 	The maximum pixel size. </param>
        /// <param name="filename">		 	(Optional) file path of the output image file. </param>
        ///
        /// <returns>	The new image. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DMediaCenterTaskPtr create_Image(const char* rasterfilename, int max_pixelsize, const char* filename = 0) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a image of a raster. </summary>
        ///
        /// <remarks>	Creates an image from a raster representation, with the specified width and height.
        /// 			If filename is not null, the image will be written directly to disk	at the given
        /// 			filepath. </remarks>
        ///
        /// <param name="raster_rep">  	The raster representation. </param>
        /// <param name="pixel_width">  The width of the output image in pixels. </param>
        /// <param name="pixel_height">	The height of the output image in pixels. </param>
        /// <param name="filename">	   	(Optional) file path of the output image file. </param>
        ///
        /// <returns>	A media center task that will create the new image. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DMediaCenterTaskPtr create_Image(const IGW3DRasterRepresentationWPtr raster_rep, int pixel_width, int pixel_height, const char* filename = 0) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a image of a vector layer. </summary>
        ///
        /// <remarks>	Creates an image from a vector layer, with the specified width and height.
        /// 			If filename is not null, the image will be written directly to disk	at the given
        /// 			filepath. </remarks>
        ///
        /// <param name="vector_layer">	The vector layer. </param>
        /// <param name="env">		  	The envelope/constraint to gather in WGS84. </param>
        /// <param name="max_pixelsize"> if -1, raster will be that of the GW3DEnvelope.  Otherwise it will be constrained to max_pixelsize  </param>
        /// <param name="filename">	   	(Optional) file path of the output image file. </param>
        ///
        /// <returns>	A media center task that will create the new image. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DMediaCenterTaskPtr create_Image(const IGW3DVectorLayerWPtr vector_layer, const GW3DEnvelope* env, int max_pixelsize = -1, const char* filename = 0) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates an image of a 3D window. </summary>
        ///
        /// <remarks>	Creates an image from a 3D window, with the specified width and height.
        /// 			If filename is not null, the image will be written directly to disk	at the given
        /// 			filepath. </remarks>
        ///
        /// <param name="window">	   	The window. </param>
        /// <param name="pixel_width"> 	Width of the pixel. </param>
        /// <param name="pixel_height">	Height of the pixel. </param>
        /// <param name="filename">	   	(Optional) file path of the output image file. </param>
        ///
        /// <returns>	A media center task that will create the new image. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DMediaCenterTaskPtr create_Image(const IGW3DWindowWPtr window, int pixel_width, int pixel_height, const char* filename = 0) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Cancel work on a media task. </summary>
        ///
        /// <param name="task">	The task. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void cancel_Work(IGW3DMediaCenterTaskPtr task) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Wait for completion on a task. </summary>
        ///
        /// <remarks>	Media tasks are handled asynchronously, and the application can poll the task for
        /// 			completion using this function. </remarks>
        ///
        /// <param name="task">   	The task. </param>
        /// <param name="timeout">	(optional) the timeout.  0 is infinite. </param>
        ///
        /// <returns>	GW3D_sOk if complete, timeout, or error. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult wait_ForCompletion(IGW3DMediaCenterTaskPtr task, int timeout = 0) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the codec driver collection. </summary>
        ///
        /// <param name="reset_next_iterator">	(optional) reset the next iterator on the collection. </param>
        ///
        /// <returns>	the codec driver collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //virtual IGW3DCodecDriverCollection *get_CodecDriverCollection(  bool reset_next_iterator = false ) = 0;
    };
}