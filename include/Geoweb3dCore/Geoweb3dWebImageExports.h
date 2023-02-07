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
#include <Geoweb3d/core/GW3DInterFace.h>

/* Primary namespace */
namespace Geoweb3d
{
	/* Web Image Streamer namespace */
	namespace WebImageStreamer
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets an image object for a web image streamer handle. </summary>
		///
		/// <remarks>	If called multiple times with the same handle and collection, this function will 
		/// 			return a pointer to the same image.  This image can be used as a texture for 
		/// 			representations such as planes and projected images. </remarks>
		///
		/// <param name="handle">	 	The handle of the web image streamer image. </param>
		/// <param name="collection">	[in] The collection to which the image should be added. </param>
		/// <param name="result">	 	[out] The result. </param>
		///
		/// <returns>	The image. </returns>
		///
		/// <see cref="OpenUrl"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL const IGW3DImageWPtr get_Image( unsigned handle, IGW3DImageCollectionPtr &collection, GW3DResult &result );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Opens an URL using the web image streamer. </summary>
		///
		/// <remarks>	Open a url for streaming images off the web or the local disk (for use with vector 
		/// 			representations such as planes and projected images. </remarks>
		///
		/// <param name="unique_name"> 	Unique name (can be the url). </param>
		/// <param name="url">		   	URL of the image to be streamed. </param>
		/// <param name="refresh_rate">	The rate in milliseconds that the image should be refreshed. </param>
		/// <param name="handle">	   	[out] The handle. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		///
		/// <see cref="get_Image"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult OpenUrl (/*in*/ const char* unique_name , /*in*/ const char* url, /*in*/ unsigned refresh_rate, /*out*/ unsigned &handle);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the 'no image' image. </summary>
		///
		/// <remarks>	Sets an image to be used if the stream's target image is not found. </remarks>
		///
		/// <param name="handle">  	The handle. </param>
		/// <param name="filename">	Filename of the 'no image' file. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult SetNoImage(/*in*/unsigned handle, /*in*/ const char* filename );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Automatically write any error into the image. </summary>
		///
		/// <param name="handle"> 	The handle. </param>
		/// <param name="enabled">	true to enable, false to disable. </param>
		/// <param name="legend"> 	The legend. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult AutoSetAnyErrorIntoImage(/*in*/unsigned handle, /*in*/ bool enabled, /*in*/ const char* legend );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Automatic resize image. </summary>
		///
		/// <param name="handle"> 	The handle. </param>
		/// <param name="enabled">	true to enable, false to disable. </param>
		/// <param name="width">  	The width. </param>
		/// <param name="height"> 	The height. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult AutoResizeImage(/*in*/unsigned handle, bool enabled, /*in*/unsigned width, /*in*/unsigned height );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Modify URL. </summary>
		///
		/// <remarks>	Change the url of the web image streamer image. </remarks>
		///
		/// <param name="handle"> 	The handle. </param>
		/// <param name="new_url">	the new URL. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		///
		/// <see cref="OpenUrl"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult ModifyUrl(/*in*/ unsigned handle,  /*in*/  const char* new_url );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Modify refresh rate. </summary>
		///
		/// <param name="handle">	   	The handle. </param>
		/// <param name="refresh_rate">	The refresh rate, in milliseconds. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult ModifyRefreshRate(/*in*/ unsigned handle,  /*in*/  unsigned refresh_rate );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets current frame count. </summary>
		///
		/// <param name="handle">	  	The handle. </param>
		/// <param name="frame_count">	[out] Number of frames. </param>
		///
		/// <returns>	The current frame count. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult GetCurrentFrameCount(/*in*/ unsigned handle,  /*out*/ unsigned &frame_count );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves the image. </summary>
		///
		/// <param name="handle">  	The handle. </param>
		/// <param name="filename">	Filename of the output file. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult SaveImage(/*in*/ unsigned handle, const char* filename );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Mirror the image. </summary>
		///
		/// <param name="handle">	 	The handle. </param>
		/// <param name="horizontal">	true to mirror horizontally. </param>
		/// <param name="vertical">  	true to mirror vertically. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult MirrorImage(/*in*/ unsigned handle, bool horizontal, bool vertical );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the native size of the image. </summary>
		///
		/// <param name="handle">	The handle. </param>
		/// <param name="height">	[out] The height. </param>
		/// <param name="width"> 	[out] The width. </param>
		///
		/// <returns>	The native size. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult GetNativeSize(/*in*/ unsigned handle, unsigned& height, unsigned& width );
	}
}