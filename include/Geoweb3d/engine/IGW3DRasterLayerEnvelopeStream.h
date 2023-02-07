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
#include "../../Geoweb3dCore/GeometryExports.h"
#include "../../Geoweb3dCore/RasterExports.h"
#include "../core/GW3DInterFace.h"
#include "IGW3DRasterLayerStreamFilter.h"
#include "IGW3DRasterLayerEnvelopeStreamResult.h"

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DRequest : public GW3DEnvelope
	{
		virtual ~IGW3DRequest() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum BandRequest
		{
			// BANDS_AUTO will return the IGW3DImage in the native band count of the data (refer to get_HasAlpha() on the IGW3DImage)
			BANDS_AUTO = 0,
			// Return IGW3DImage will be of 3 bands
			BANDS_3 = 1,
			// Return IGW3DImage will be of 4 bands
			BANDS_4 = 2
		};

		IGW3DRequest() : raster_width(256), raster_height(256), band_request(BandRequest::BANDS_AUTO), swizzle(false), flip_vertical(false), sampler_alg(Raster::SamplerAlg::NEAREST_NEIGHBOR)
		{

		}

		//will sample to this resolution
		unsigned raster_width;
		unsigned raster_height;
		//will swizzle the first and last channel
		bool swizzle;
		bool flip_vertical;
		BandRequest band_request;
		Raster::SamplerAlg sampler_alg;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DRasterLayerEnvelopeStream : public IGW3DRasterLayerStreamFilter
	{
		virtual ~IGW3DRasterLayerEnvelopeStream() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	note, if you did an async query, this will get called on a different
		///				thread, so make sure you are thread safe.
		///				one day we can store these up on a Q and a user can gather all the 
		///				requests on whatever thread they wish instead of what we have today. </summary>
		///
		/// <param name="result"></param>
		///
		/// <returns></returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnStream(IGW3DRasterLayerEnvelopeStreamResult* result) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DRasterLayerFilterRegion : public GW3DEnvelope
	{
		virtual ~IGW3DRasterLayerFilterRegion() {}

		double min_x_pix_size; 
		double min_y_pix_size;
		double max_x_pix_size;
		double max_y_pix_size;
	};
}