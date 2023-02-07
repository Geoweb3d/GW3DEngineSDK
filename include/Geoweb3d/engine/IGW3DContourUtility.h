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

	struct IGW3DProgress;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> TODO </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL ContourCreationParameters
	{
		virtual ~ContourCreationParameters() {}

		ContourCreationParameters() :
			elevation_interval(0.f),
			x_size(800),
			y_size(800),
			point_resolution(-1.f)
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Elevation interval between contours. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		float elevation_interval;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Only used if point_resolution is not specified. (ie. if point_resolution <= 0)
		/// This will determine how close the final contour line points can be. Specifies the precision of the contour calculation. 
		/// This in conjunction with the target bounds will determine the underlying datasource query precision.
		/// If either x_size or y_size are less than or equal to zero - the composite resolution will be of the maximum
		/// constituent datasource. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int x_size;
		unsigned int y_size;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Specifies the precision of the contour calculation. 
		/// This will determine how close the final contour line points can be. 
		/// It is specified in meters. If point resolution is 1 meter you can expect
		/// that the contour line could turn every meter. Note: the resolution of the datasources
		/// have to have had resolutions greater than this value or else the point resolution 
		/// will only be as good as highest possible resolution that the underlying data will support. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		float point_resolution;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> The target EPSG:4326 geospatial bounds of the scribed area. We don't currently support dateline spanning.
		/// and then envelope min values must be less than envelope max values. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3DEnvelope env;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Driver type to export into </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		IGW3DRasterDriverWPtr export_driver;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Not hooked up yet. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		IGW3DPropertyCollection* export_driver_properties;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DContourUtility
	{
		virtual ~IGW3DContourUtility() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates Contours </summary>
		///
		/// <remarks> Creates a vectorized datasource of within the bounds defined in ContourCreationParameters
		/// and of the layers in the IGW3DLayerHelperCollection object.  </remarks>
		///
		/// <param name="out_file">Contours will be saved out to this location. </param>
		///
		/// <param name="projection">The coordinate reference systems that can be passed are EPSG Projected, Geographic or Compound CRS (i.e. EPSG:4296), 
		/// a well known text (WKT) CRS definition, PROJ.4 declarations, or the name of a .prj file containing a WKT CRS 
		/// definition. If not specified we will default to EPSG:4326</param>
		///
		/// <param name="layers">The list of layers that are chosen to take part in the contouring.  Internally, although a raster layer may be included we will use
		/// the associated raster datasource.  </param>
		///
		/// <param name="params">refer to ContourCreationParameters</param>
		/// 
		/// <returns> A IGW3DTask object. </returns>
		/// 
		/// <see cref="IGW3DTask"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DTaskPtr create_Contours(const char* out_file, const char* projection, const IGW3DLayerHelperCollectionPtr layers, const ContourCreationParameters& params, IGW3DProgress* progress_callback) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates Contours </summary>
		///
		/// <remarks>	This mode that takes a IGW3DRasterDataSource. It does not support the ContourCreationParameters resolution settings.
		/// The resolution is determined by the datasource resolution. It also does not currently use the GW3DEnvelope target bounds - it
		/// will use the default bounds of the datasource.  TODO.
		/// For now you can get around that by using the the other create_Contours API of construct your IGW3DRasterDataSource with
		/// IGW3DDataSourceCompositor</remarks>
		///
		/// <param name="out_file">Contours will be saved out to this location. </param>
		/// <param name="datasource">The datasource chosen to take part in the contouring.  </param>
		/// <param name="elevation_interval"> Elevation interval between contours.</param>
		/// <param name="export_driver">driver type to export into</param>
		/// 
		/// <returns> A IGW3DTask object. </returns>
		/// 
		/// <see cref="IGW3DTask"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DTaskPtr create_Contours(const char* out_file, const IGW3DRasterDataSourceWPtr datasource, const float elevation_interval, IGW3DRasterDriverWPtr export_driver, IGW3DProgress* progress_callback) = 0;

	};
}