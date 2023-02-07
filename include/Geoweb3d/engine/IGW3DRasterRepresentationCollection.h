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
#include "GW3DExports.h"
#include "RasterRepresentationLayerCreationParameter.h"

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Raster representation collection viewer. </summary>
	///
	/// <remarks>	A collection of raster representations that does not allow representations to
	/// 			be created or closed. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DRasterRepresentationCollectionViewer : public IGW3DCollection< IGW3DRasterRepresentationWPtr >
	{
		virtual ~IGW3DRasterRepresentationCollectionViewer() {}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of raster representations. </summary>
	///
	/// <remarks>	The collection of all raster representations for a raster representation driver.
	/// 			This is also where new representations get created. </remarks>
	///
	/// <see cref="IGW3DRasterRepresentationDriver"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DRasterRepresentationCollection : public IGW3DCollection< IGW3DRasterRepresentationWPtr >
	{
		virtual ~IGW3DRasterRepresentationCollection() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Array indexer operator. </summary>
		///
		/// <param name="index">	Zero-based index of the raster representation. </param>
		///
		/// <returns>	The indexed value. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DRasterRepresentationWPtr operator[](unsigned long index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets at index. </summary>
		///
		/// <param name="index">	Zero-based index of the raster representation. </param>
		///
		/// <returns>	at index. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DRasterRepresentationWPtr get_AtIndex(unsigned long index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new raster representation for the given raster layer. </summary>
		///
		/// <param name="layer">	The raster layer for which the representation is to be created. </param>
		/// <param name="params"> TODO </param>
		///
		/// <returns>	A new raster representation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterRepresentationWPtr create(IGW3DRasterLayerWPtr layer, const Raster::RasterRepresentationLayerCreationParameter& params) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Closes the given raster representation. </summary>
		///
		/// <remarks>	Closes the given raster representation and removes it from the collection.
		///
		/// 			To close a representation: IGW3DRasterRepresentationCollection::close( window );
		///
		///				IGW3DRasterRepresentationWPtr will become expired. </remarks>
		///
		/// <param name="rep"> the representation </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close(IGW3DRasterRepresentationWPtr& rep)
		{
			close_RasterRepresentation(rep);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new raster representation from a collection of raster layers. </summary>
		///
		/// <remarks>	This is only currently implemented for the IGW3DRasterRepresentation Driver named "Imagery-Overlay"
		///				It is envisioned that this mode could in the future be supported by the other drivers as well. 
		///				This API is more of a helper to group layers in a time related way, however, working work individual layer representation
		///				can achieve the same effect. </remarks>
		///
		/// <param name="collection">	The finalized collection of layers </param>
		/// <param name="params"> TODO </param>
		///
		/// <returns>	A new raster representation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterRepresentationWPtr create(IGW3DRasterLayerImageCollectionFinalizationTokenPtr collection, const Raster::RasterRepresentationLayerCreationParameter& params) = 0;
	};
}