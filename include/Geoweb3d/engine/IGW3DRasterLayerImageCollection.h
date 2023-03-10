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
	/// <summary>	Collection of raster layer images. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DRasterLayerImageCollection : public IGW3DCollection< IGW3DRasterLayerWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Array indexer operator. </summary>
		///
		/// <param name="index">	Zero-based index of the image. </param>
		///
		/// <returns>	The indexed value. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DRasterLayerWPtr operator[](unsigned long index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets at index. </summary>
		///
		/// <param name="index">	Zero-based index of the image. </param>
		///
		/// <returns>	at index. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DRasterLayerWPtr get_AtIndex(unsigned long index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a IGW3DRasterLayerWPtr to the collection. </summary>
		///
		/// <returns>	GW3D_s0k on success. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult add(IGW3DRasterLayerWPtr layer) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a finalization token. </summary>
		///
		/// <remarks>	Once an raster layer image collection is populated and configured as desired, create a
		/// 			finalization token for use in the Geoweb3d SDK, such as a representation.
		/// 			Subsequent changes to the collection and its images will have no effect on the
		/// 			configuration represented by the token. </remarks>
		///
		/// <returns>	The new finalization token. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterLayerImageCollectionFinalizationTokenPtr create_FinalizeToken(int duration_minutes) = 0;
	};
}