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
#include <Geoweb3dCore/Geoweb3dEngine.h>
#include <Geoweb3dCore/Geoweb3dTypes.h>

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DSDKRasterContext
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="vectordrivername"> TODO </param>
		/// <param name="data_source_name"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult close_GW3DDataSource(const char* vectordrivername, const char* data_source_name) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="raster_driver_name"> TODO </param>
		/// <param name="data_source_name"> TODO </param>
		/// <param name="create"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterLayerCollection* get_IGW3DRasterLayerCollection(const char* raster_driver_name, const char* data_source_name, bool create = true) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterRepresentationDriverCollection* get_IGW3DRasterRepresentationDriverCollection() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="vector_driver_name"> TODO </param>
		/// <param name="data_source_name"> TODO </param>
		/// <param name="create"> TODO </param>
		/// <param name="cb"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DRasterDataSourceWPtr load_ElevationDataSource(const char* vector_driver_name, const char* data_source_name, bool create = false, IGW3DRasterPipelineCallbacks* cb = 0) = 0;

		// forces the dataset to go active|inactive to force a reload.

		virtual void HackCoverageUpdate() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="longitude"> TODO </param>
		/// <param name="latitude"> TODO </param>
		/// <param name="elevation"> TODO </param>
		/// <param name="handle"> TODO </param>
		/// <param name="interpolate"> TODO </param>
		/// <param name="search_all_sources"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GetElevationAt(double longitude, double latitude, /*out*/ float& elevation, /*out*/ Raster::RasterSourceHandle& handle, bool interpolate = true, bool search_all_sources = false) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="longitude"> TODO </param>
		/// <param name="latitude"> TODO </param>
		/// <param name="elevation"> TODO </param>
		/// <param name="interpolate"> TODO </param>
		/// <param name="search_all_sources"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GetElevationAt(double longitude, double latitude, /*out*/ float& elevation, bool interpolate = true, bool search_all_sources = false) = 0;
	};
}
