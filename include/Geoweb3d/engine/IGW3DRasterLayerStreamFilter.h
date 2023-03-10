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
	/// <summary>	A raster layer stream filter. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DRasterLayerStreamFilter : public IGW3DCollection< unsigned long >
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the count of dimension values to be processed. </summary>
		///
		/// <remarks>	This is required to be implemented.  Note: if count returns 0, then no other of the
		/// 			APIs used to index into the collection will get called.  What this means is if you
		/// 			wish to stream an entire layers dimension values, its safe to return 0 for everything. </remarks>
		///
		/// <returns>	The count. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned long count() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the next dimension id in the collection and increments the iterator.
		/// 			</summary>
		///
		/// <param name="ppVal">	[out] If non-null, the value. </param>
		///
		/// <returns>	true if it succeeds, false if no more dimension values exist in the collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool next(unsigned long* ppVal) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Resets the iterator. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void reset() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Array indexer operator. </summary>
		///
		/// <remarks>	Get the dimension id at the given index. </remarks>
		///
		/// <param name="index">	Zero-based index of the dimension id. </param>
		///
		/// <returns>	The dimension id. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned long operator[](unsigned long index) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets dimension id at index. </summary>
		///
		/// <remarks>	Get the dimension id at the given index. </remarks>
		///
		/// <param name="index">	Zero-based index of the dimension id. </param>
		///
		/// <returns>	The dimension id. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned long get_AtIndex(unsigned long index) = 0;

	};
}