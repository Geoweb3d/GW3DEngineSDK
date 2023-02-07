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
#include "Geoweb3dExports.h"
#include "Geoweb3dTypes.h"

/* @file DemExports.h */

/* Primary namespace */
namespace Geoweb3d
{
	class GW3DGeometry;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Dem Namespace </summary>
	///
	/// <remarks>	Interfaces in the Dem namespace are part of the legacy C-like namespace. 
	///				Note: The C++ raster API has more capabilities than the legacy C-style API, however
	///				in other capabilities it does not yet have equivalency.  See 
	/// 			IGW3DRasterDriverCollection to evaluate which capabilities are important to
	///				your use case before selecting which raster API to use. </remarks>
	///
	/// <see cref="IGW3DRasterDriverCollection"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace Dem
	{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Dem query details. </summary>
///
/// <see cref="QueryDem"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DemQuery
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	DemQuery() : 
		min_lon( 0.0 ), min_lat( 0.0 ),
		max_lon( 0.0 ), max_lat( 0.0 ),
		x_pixel_size( 0.0 ), y_pixel_size( 0.0 ),
		width( 0 ), height( 0 ),
		driver_long_name( 0 ),
		driver_short_name(0),
		short_name(0),
		full_file_name(0)
		{ }

	/// <summary>	Minimum longitude of the file in WGS84 </summary>
	double min_lon;

	/// <summary>	Minimum latitude of the file in WGS84. </summary>
	double min_lat;

	/// <summary>	Maximum longitude of the file in WGS84. </summary>
	double max_lon;

	/// <summary>	Maximum latitude of the file in WGS84 </summary>
	double max_lat;

	/// <summary>	X pixel size (resolution) in the file's native pixel format. </summary>
	double x_pixel_size;

	/// <summary>	Y pixel size (resolution) in the file's native pixel format. </summary>
	double y_pixel_size;

	/// <summary>	The width in pixels. </summary>
	int width;

	/// <summary>	The height in pixels. </summary>
	int height;

	/// <summary>	Detailed name of the driver that loaded the DEM (digital elevation model). </summary>
	const char* driver_long_name;

	/// <summary>	Short name of the driver that loaded the DEM (digital elevation model). </summary>
	const char* driver_short_name;

	/// <summary>	Short name of the file that loaded the DEM (digital elevation model). </summary>
	const char* short_name;

	/// <summary>	Full file name of the file that loaded the DEM (digital elevation model). </summary>
	const char* full_file_name;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	DEM loading properties. </summary>
///
/// <remarks>	Parameters for configuring the DEM file at load time. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DemLoadProperties
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Comparison function for invalid elevation values. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum OpType
	{
		/// <summary>	An enum constant representing the option for discarding elevation values equal 
		/// 			to the value invalid_elevation_value. </summary>
		DEM_EQ = 0,
		/// <summary>	An enum constant representing the option for discarding elevation values equal
		/// 			or less than the value invalid_elevation_value. </summary>
		DEM_LEQ = 1	
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	DemLoadProperties() : priority(0),
		override_dataset( false ), 
		invalid_elevation_value( -10000 ), 
		op( DEM_LEQ ), 
		vertical_exaggeration( 1.0 ),
		page_level( 11 ),
		projection_error_threshold( 0.f ),
		activity( true )
	{ }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	true to override any invalid elevation values in the dataset </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool override_dataset;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The invalid elevation value.</summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float invalid_elevation_value;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The comparison operator to determine invalid values. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	OpType op;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Vertical exaggeration of the dataset. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	double vertical_exaggeration;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The priority of this file over others (not yet implemented). </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int priority;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The page level. </summary>
	///
	/// <remarks>	Level of the terrain to start bringing in the file, ranges from 1 - 20 or so, with lower numbers 
	/// 			bringing in the dataset quicker.  While this can be advantageous for some resolutions of data, 
	/// 			for fine resolutions it is usually unnecessary to bring the data in before level 11+.  Bringing 
	/// 			in high resolution data preemptively will just spike memory overhead. To match a page level to 
	///				a real-world distance in meters, call PageLevelToMeterRadius. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned page_level;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The projection error threshold. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	float projection_error_threshold;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	true for layer to be active on construction. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool activity;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Loads a DEM (digital elevation mode). </summary>
///
/// <remarks>	Note: this API will be deprecated.  See IGW3DRasterDriverCollection for the future 
/// 			interface. </remarks>
///
/// <param name="url">			  	Location of the DEM. </param>
/// <param name="projection_file">	The projection file. </param>
/// <param name="load_props">	  	The load properties. </param>
/// <param name="handle">		  	[out] A unique identifier for the DEM</param>
///
/// <returns>	GW3D_sOk if succeeded. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult LoadDEM( const char* url, const char* projection_file, DemLoadProperties const& load_props,Raster::RasterSourceHandle & handle );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Removes the dem described by handle. </summary>
///
/// <remarks>	remove a DEM. </remarks>
///
/// <param name="handle">	The unique identifier for the DEM. </param>
///
/// <returns>	GW3D_sOk if succeeded. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult RemoveDEM(Raster::RasterSourceHandle & handle );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets elevation at a given latitude and longitude. </summary>
///
/// <param name="longitude">  	The longitude. </param>
/// <param name="latitude">   	The latitude. </param>
/// <param name="elevation">   	The elevation. </param>
/// <param name="handle">	The unique identifier for the DEM. </param>
/// <param name="interpolate">	(optional) Whether or not to apply the interpolation we use 
/// 							internally, versus just the pixel value.. </param>
/// <param name="search_all_sources"> Will search all active elevation sources (not just the ones currently paged in). </param>
///
/// <returns>	The result code and the elevation as a parameter ( zero if no data/invalid data 
/// 			is found.) </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult GetElevationAt(double longitude, double latitude, /*out*/ float& elevation, /*out*/ Raster::RasterSourceHandle& handle, bool interpolate = true, bool search_all_sources = false);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets elevation at a given latitude and longitude. </summary>
///
/// <param name="GW3DGeometry">  	The longitude, latitude points. </param>
/// <param name="interpolate">	(optional) Whether or not to apply the interpolation we use 
/// 							internally, versus just the pixel value.. </param>
///
/// <returns>	Fills in the elevation (Z) at each point of the geometry. This API is not flushed out
///				as in today, GeometryType of the geometry will not be modified.  This may change when 
///				when we release the API	
////////////////////////////////////////////////////////////////////////////////////////////////////

//GW3D_DLL GW3DResult FillElevations( GW3DGeometry &geometry, bool interpolate = true);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets elevation at a given latitude and longitude. </summary>
///
/// <param name="GW3DGeometry">  	The longitude, latitude points. </param>
/// <param name="interpolate">	(optional) Whether or not to apply the interpolation we use 
/// 							internally, versus just the pixel value.. </param>
///
/// <returns>	will clone your input GW3DGeometry and ensure return is 2.5, elevation(Z)
///				will be filled in.
////////////////////////////////////////////////////////////////////////////////////////////////////

//GW3D_DLL GW3DGeometryPtr GetElevations(GW3DGeometry &geometry, bool interpolate = true);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Queries a file. </summary>
///
/// <remarks>	Get information about an image. </remarks>
///
/// <param name="filename">	Filename of the file. </param>
/// <param name="query">   	[in,out] The query. </param>
///
/// <returns>	GW3D_sOk if successful, the appropriate error code otherwise. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult QueryFile( const char* filename, DemQuery& query);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Queries information about a DEM. </summary>
///
/// <param name="raster">	The raster to query. </param>
/// <param name="query">	[out] The DEM query results. </param>
///
/// <returns>	GW3D_sOk if succeeded. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult QueryDem(Raster::RasterSourceHandle raster, DemQuery& query );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets the dem activity. </summary>
///
/// <remarks>	This function should only fail if handle does not point at a valid DEM.  Even upon 
/// 			success, though, your DEM will not necessarily appear, since that also depends on 
/// 			your location in the world and whether there is another DEM with greater priority covering the DEM associated with handle. </remarks>
///
/// <param name="handle">	The unique identifier for the DEM. </param>
/// <param name="activity">  The activity state. </param>
///
/// <returns>	GW3D_sOk is succeeded. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult SetDemActivity(Raster::RasterSourceHandle handle, const bool activity );

#ifndef SWIGGING

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets a user data pointer. </summary>
///
/// <remarks>	Associates a pointer to user data with the DEM handle, so it can be retrived later 
/// 			via . </remarks>
///
/// <param name="raster">	The unique identifier for the DEM. </param>
/// <param name="p">		 	[in,out] If non-null, the void * to process. </param>
///
/// <returns> TODO </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult SetUserDataPtr(Raster::RasterSourceHandle raster, void *p );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the pointer set by SetUserDataPtr. </summary>
///
/// <remarks>	Used for referencing custom data to raster datasources. </remarks>
///
/// <param name="raster">	The raster datasource. </param>
///
/// <returns>	null if it fails, else the user data pointer. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL void* GetUserDataPtr(Raster::RasterSourceHandle raster );
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets dem's vertical exaggeration. </summary>
///
/// <param name="handle">   The handle. </param>
/// <param name="vertical_exaggeration">	vertical exaggeration </param>
///
/// <returns>	A GW3DResult. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult SetVerticalExaggeration(Raster::RasterSourceHandle handle, const double vertical_exaggeration);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Set to override the data source's no data value. </summary>
///
/// <remarks>	Make sure to set SetOverrideInvalidValue to a chosen override value.  </remarks>
///
/// <param name="handle"> The handle. </param>
/// <param name="override_dataset">	Whether to override the data source's no data with a user defined value. </param>
///
/// <returns>	A GW3DResult. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult SetOverrideInvalid(Raster::RasterSourceHandle handle, const bool override_invalid);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets the value to override the data source's no data value </summary>
///
/// <remarks>	Set SetOverrideInvalid accordingly </remarks>
///
/// <param name="handle">   The handle. </param>
/// <param name="override_invalid_value">	The value to use as the override </param>
///
/// <returns>	A GW3DResult. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult SetOverrideInvalidValue(Raster::RasterSourceHandle handle, const float override_invalid_value);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets dem's page level. </summary>
///
/// <remarks>	Will cycle all the data.  </remarks>
///
/// <param name="handle">   	The handle. </param>
/// <param name="page_level">	The page level </param>
///
/// <returns>	A GW3DResult. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult SetPageLevelDEM(Raster::RasterSourceHandle handle, const unsigned page_level);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets dem's no data value comparitor. </summary>
///
/// <remarks>	See OpType </remarks>
///
/// <param name="handle">   The handle. </param>
/// <param name="op">	The comparitor </param>
///
/// <returns>	A GW3DResult. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult SetInvalidComparitor(Raster::RasterSourceHandle handle, const DemLoadProperties::OpType op);

}
}