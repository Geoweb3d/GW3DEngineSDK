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
	/// <summary>	Collection of images with user defined attributes.
	///				NOTE If you plan to share texture IDs, you need to be using or at least be sharing
	///				the sdk's opengl context.  Otherwise texture IDs between us will not be able to be shared. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DOpenGLTextureCollection : public IGW3DCollection< IGW3DOpenGLTextureWPtr >
	{
		virtual ~IGW3DOpenGLTextureCollection() {}

#ifndef SWIGGING

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Array indexer operator. </summary>
		///
		/// <param name="index">	Zero-based index of the image. </param>
		///
		/// <returns>	The indexed value. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DOpenGLTextureWPtr operator[](unsigned long index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets at index. </summary>
		///
		/// <param name="index">	Zero-based index of the image. </param>
		///
		/// <returns>	at index. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DOpenGLTextureWPtr get_AtIndex(unsigned long index) const = 0;

#endif //SWIGGING

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="id"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DOpenGLTextureWPtr get_ByPropertyCollectionID(int id) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="id"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DOpenGLTextureWPtr get_ByOpenGLTextureID(int id) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="id"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DOpenGLTextureWPtr get_ByPropertyCollectionID(int id) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="id"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DOpenGLTextureWPtr get_ByOpenGLTextureID(int id)  const = 0;

			/// if  width == -1 and  height == -1, and if you have this texture mapped to geometry 
			/// that conforms to the aspect ratio, a GPU readback of the texture width and height
			/// will take place the first time this texture is needed to render in the 3D scene. 
			/// Its advisable if you know what the texture width and height is (or will be) to 
			/// set the values instead of the need for the GPU readback.
			///
			/// If you set owner == false instead of leaving at true, the SDK will not delete the 
			/// opengl texture on the GPU if its no longer needed.  You will get unknown results 
			/// and possibly even have stability issues if you delete the texture id while its 
			/// still needed in the 3D scene.

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Creates this object. </summary>
		///
		/// <remarks> Creates an OpenGL Texture object. </remarks>
		///
		/// <param name="opengl_texture_id"> TODO </param>
		/// <param name="width"> TODO </param>
		/// <param name="height"> TODO </param>
		/// <param name="owner"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DOpenGLTextureWPtr create(int opengl_texture_id, int width = -1, int height = -1, bool owner = true) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DOpenGLTextureWPtr create() = 0;

			/// if  width == -1 and  height == -1, and if you have this texture mapped to geometry 
			/// that conforms to the aspect ratio, a GPU readback of the texture width and height
			/// will take place the first time this texture is needed to render in the 3D scene. 
			/// Its advisable if you know what the texture width and height is (or will be) to 
			/// set the values instead of the need for the GPU readback.
			///
			/// If you set owner == false instead of leaving at true, the SDK will not delete the 
			/// opengl texture on the GPU if its no longer needed.  You will get unknown results 
			/// and possibly even have stability issues if you delete the texture id while its 
			/// still needed in the 3D scene.
			///
			/// note, if opengl_texture_id lives in another texture object within this collection, GW3D_sAlreadyInserted will be returned. 
			/// If both are owners, you may get into a bad situation.  It would be suggested to scan the collection first if you are not sure.
			/// Even with GW3D_sAlreadyInserted, we assume you know what you are doing and set the ID anyways.

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="texture"> TODO </param>
		/// <param name="opengl_texture_id"> TODO </param>
		/// <param name="width"> TODO </param>
		/// <param name="height"> TODO </param>
		/// <param name="owner"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult	put_TextureID(IGW3DOpenGLTextureWPtr& texture, int opengl_texture_id, int width = -1, int height = -1, bool owner = true) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DOpenGLTextureFinalizationTokenPtr create_FinalizeToken() = 0;
	};
}