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
#include <Geoweb3dCore/GeometryExports.h>

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL DataSourceCompositeParameters
	{
		virtual ~DataSourceCompositeParameters() {}

		DataSourceCompositeParameters() :
			x_size(800),
			y_size(800),
			point_resolution(0.f)
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Only used if point_resolution is not specified. (ie. if point_resolution <= 0)
		/// This will determine resolution of the composite datasource in conjunction with the target bounds.
		/// If either x_size or y_size are less than or equal to zero - the composite resolution will be of the maximum
		/// constituent datasource. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int x_size;
		unsigned int y_size;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> This will determine resolution of the composite datasource in conjunction with the target bounds.
		/// It is specified in meters. U will have to have had data source resolutions higher (smaller point resolution value) than this value or 
		/// else the point resolution will only be as good as highest possible resolution that the underlying data will support. 
		/// The final resolution is extrapolated assuming that this value is the central pixel's metric dimension. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		float point_resolution;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> The target EPSG:4326 geospatial bounds of the scribed area. We don't currently support dateline spanning.
		/// Envelope min values must be less than envelope max values. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3DEnvelope env;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Not hooked up yet, but would correspond to the VRT driver </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		IGW3DPropertyCollection* export_driver_properties;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	DataSource Compositor Utility </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DDataSourceCompositor
	{
		virtual ~IGW3DDataSourceCompositor() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a composite file. </summary>
		///
		/// <param name="out_file">Composites will be saved with the .vrt extension so no extension needs to be provided.  Will be replaced if extension supplied. </param>
		/// <param name="out_projection">The coordinate reference systems that can be passed are EPSG Projected, Geographic or Compound CRS (i.e. EPSG:4296), a well known 
		/// text (WKT) CRS definition, PROJ.4 declarations, or the name of a .prj file containing a WKT CRS definition. If not specified we will default to EPSG:4326</param>
		/// <param name="layers">The list of layers that are chosen to take part in the compositing.  Internally, although a raster layer may be included we will use
		/// the associated raster datasource.  </param>
		/// <param name="params">refer to DataSourceCompositeParameters</param>
		/// 
		/// <returns> A Geoweb3d status code. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult create_Composite(const char* out_file, const char* out_projection, const IGW3DLayerHelperCollectionPtr layers, const DataSourceCompositeParameters& params) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Creates a composite datasource. </summary>
		///
		/// <remarks> Call create_Copy on the returned datasource if you wish to make the composite datasource to be persistent in one location. </remarks>
		///
		/// <param name="out_file"> Composites will be saved with the .vrt extension so no extension needs to be provided.  Will be replaced if extension supplied. </param>
		/// <param name="layers"> The list of layers that are chosen to take part in the compositing.  Internally, although a raster layer may be included we will use
		/// the associated raster datasource. </param>
		/// <param name="out_projection"> The coordinate reference systems that can be passed are EPSG Projected, Geographic or Compound CRS (i.e. EPSG:4296), 
		/// a well known text (WKT) CRS definition, PROJ.4 declarations, or the name of a .prj file containing a WKT CRS definition.
		//  If not specified we will default to EPSG:4326 </param>
		/// <param name="params"> refer to DataSourceCompositeParameters </param>
		/// <param name="result"> The returned Geoweb3d status code. </param>
		/// 
		/// <returns> A IGW3DRasterDataSourceWPtr for the composite datasource. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual IGW3DRasterDataSourceWPtr create_Composite(const char* out_file, const char* out_projection, const IGW3DLayerHelperCollectionPtr layers, const DataSourceCompositeParameters& params, GW3DResult& result) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Get an estimated size in bytes for composite. </summary>
		///
		/// <remarks> A simplistic utility to roughly calculate the number of uncompressed samples that the composite would have.
		/// Note: currently compositing to a 32 bit float - so each sample is 4 bytes. </remarks>
		///
		/// <param name="resoution_meters"> The requested resolution in meters. </param>
		/// <param name="env"> The target WGS84 bounds. </param>
		/// <param name="bytes"> The returned size in bytes. </param>
		/// 
		/// <returns> A Geoweb3d status code. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult get_EstimatedSize(float resoution_meters, const GW3DEnvelope& env, uint64_t& bytes) = 0;
	};
}