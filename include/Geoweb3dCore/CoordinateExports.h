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
#include <Geoweb3dCore/GeometryExports.h>

/* @file CoordinateExports.h */

/* Primary namespace */
namespace Geoweb3d
{
/* Coordinate namespace */
namespace Coordinates
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Cartesian to polar conversion. </summary>
///
/// <remarks>	The Geoweb3d Engine's internal mechanism from moving to Cartesian space into Polar 
/// 			coordinates. </remarks>
///
/// <param name="x">			 	The cartesian x coordinate. </param>
/// <param name="y">			 	The cartesian y coordinate. </param>
/// <param name="z">			 	The cartesian z coordinate. </param>
/// <param name="alpha">		 	[out] The alpha (Polar coordinate equal to geodetic longitude). </param>
/// <param name="beta">			 	[out] The beta (Polar coordinate equal to 90 degrees - geodetic 
/// 								latitude). </param>
/// <param name="r">			 	[out] The radius. </param>
/// <param name="target_radians">	(optional) If true, then alpha & beta will be in radians, else 
/// 								in degrees. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL void CartesianToPolar( const double &x, const double &y, const double &z, double& alpha, double& beta, double& r, bool target_radians = true );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Cartesian to geodetic conversion. </summary>
///
/// <remarks>	The Geoweb3d Engine's internal mechanism from moving to Cartesian space into 
/// 			Geodetic coordinates. </remarks>
///
/// <param name="x">			 	The cartesian x coordinate. </param>
/// <param name="y">			 	The cartesian y coordinate. </param>
/// <param name="z">			 	The cartesian z coordinate. </param>
/// <param name="longitude">	 	[out] The geodetic longitude. </param>
/// <param name="latitude">		 	[out] The geodetic latitude. </param>
/// <param name="r">			 	[out] The radius. </param>
/// <param name="target_radians">	(optional) If true, then longitude & latitude will be in radians, else in degrees. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL void CartesianToGeodetic( const double &x, const double &y, const double &z, double& longitude, double& latitude, double& r, bool target_radians = true );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Polar to cartesian conversion. </summary>
///
/// <remarks>	The Geoweb3d Engine's internal mechanism for moving from Polar coordinates into 
/// 			Cartesian space. </remarks>
///
/// <param name="alpha">		 	The alpha. </param>
/// <param name="beta">			 	The beta. </param>
/// <param name="r">			 	The radius. </param>
/// <param name="x">			 	[out] The cartesian X. </param>
/// <param name="y">			 	[out] The cartesian Y. </param>
/// <param name="z">			 	[out] The cartesian Z. </param>
/// <param name="source_radians">	(optional) If true, then alpha & beta are expected to be in 
/// 								radians, else degrees. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL void PolarToCartesian( const double &alpha, const double &beta, const double &r, double& x, double& y, double& z, bool source_radians = true );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Geodetic to cartesian. </summary>
///
/// <param name="lon">			 	The geodetic longitude. </param>
/// <param name="lat">			 	The geodetic latitude. </param>
/// <param name="r">			 	The radius. </param>
/// <param name="x">			 	[out] The cartesian X. </param>
/// <param name="y">			 	[out] The cartesian Y. </param>
/// <param name="z">			 	[out] The cartesian Z. </param>
/// <param name="source_radians">	(optional) If true, then lon & lat are expected to be in 
/// 								radians, else degrees </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL void GeodeticToCartesian( const double &lon, const double &lat, const double &r, double& x, double& y, double& z, bool source_radians = true );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets a quaternion. </summary>
///
/// <remarks>	Convenience function to convert a geodetic position and hpr into a quaternion.
///
/// 			Given the usual parameterization of a quaternion, we keep the notation: 
/// 			w + xi + yj + zk
///	
/// 			Note that this function will automatically correct the rotation to the tangent 
/// 			plane of the sphere at the position specified. </remarks>
///
/// <param name="lon">	  	Geodetic longitude in degrees. </param>
/// <param name="lat">	  	Geodetic latitude in degrees. </param>
/// <param name="elev">   	Height above the terrain. </param>
/// <param name="heading">	Heading, in degrees. </param>
/// <param name="pitch">  	Pitch, in degrees. </param>
/// <param name="roll">   	Roll, in degrees </param>
/// <param name="qx">	  	[out] X parameter of the quat </param>
/// <param name="qy">	  	[out] Y parameter of the quat </param>
/// <param name="qz">	  	[out] Z parameter of the quat </param>
/// <param name="qw">	  	[out] W parameter of the quat </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL void GetQuaternion
			( 
				const double &lon, const double& lat, const double &elev,
				const double &heading, const double &pitch, const double &roll,
				double& qx, double& qy, double& qz, double& qw
			);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the major radius of the earth. </summary>
///
/// <remarks>	Note: currently in Geoweb3d, the earth is a sphere. </remarks>
///
/// <returns>	The radius of the earth sphere. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL double MajorRadius();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Page level to meter radius. </summary>
///
/// <remarks>	Convert the paging level used for DEM and Vector data into a useful, real world 
/// 			metric. 
///
/// 			Note that depending on the configuration of the maximum terrain LOD depth in 
/// 			Geoweb3d::IGW3DInitializationConfiguration, the value returned for a given page 
/// 			level can vary.</remarks>
///
/// <param name="page_level">	The page level. </param>
///
/// <returns>	The distance in meters at which data will begin to become visible. </returns>
///
/// <see cref="Geoweb3d::IGW3DInitializationConfiguration::put_TerrainMaxLODDepth"/>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL double PageLevelToMeterRadius( int page_level );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Get estimated samples sizes from central pixel resolution. </summary>
///
/// <remarks> A utility to roughly calculate the number of uncompressed samples that the datasource would have/need. </remarks>
///
/// <param name="resoution_meters">The center pixel resolution in meters.</param>
/// <param name="env">The target WGS84 bounds (must be in sort order Max > Min) </param>
/// <param name="horizontal_samples">The returned number of horizontal samples. </param>
/// <param name="vertical_samples">The returned number of vertical samples. </param>
///
/// <returns> A Geoweb3d status code. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult get_EstimatedSamplesSize(double resoution_meters, const Geoweb3d::GW3DEnvelope& env, double& horizontal_samples, double& vertical_samples);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Get estimated vertical samples size from central pixel resolution and horizontal size, while
/// maintaining a WGS84 aspect ration. </summary>
///
/// <remarks> A utility to roughly calculate the number of uncompressed samples that the  datasource would have/need. </remarks>
///
/// <param name="env">The target WGS84 bounds (must be in sort order Max > Min) </param>
/// <param name="horizontal_samples">The number of horizontal samples. </param>
/// <param name="vertical_samples">The returned number of vertical samples. </param>
///
/// <returns> A Geoweb3d status code. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult get_VerticalWGS84SampleSize(const Geoweb3d::GW3DEnvelope& env, const double& horizontal_samples, double& vertical_samples);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Get estimated horizontal samples size from central pixel resolution and vertical size, while
/// maintaining a WGS84 aspect ration. </summary>
///
/// <remarks> A utility to roughly calculate the number of uncompressed samples that the  datasource would have/need. </remarks>
///
/// <param name="env">The target WGS84 bounds (must be in sort order Max > Min) </param>
/// <param name="horizontal_samples">The returned number of horizontal samples. </param>
/// <param name="vertical_samples">The number of vertical samples. </param>
///
/// <returns> A Geoweb3d status code. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult get_HorizontalWGS84SampleSize(const Geoweb3d::GW3DEnvelope& env, double& horizontal_samples, const double& vertical_samples);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the estimated metric size of the central pixel data in EPSG 4326 </summary>
///
/// <param name="env">	The WGS84 bounds (must be in sort order Max > Min) </param>
/// <param name="width">	The number of width samples. </param>
/// <param name="height">	The number of height samples. </param>
/// <param name="vertical_dimension">	The central pixel metric width. </param>
/// <param name="horizontal_dimension">	The central pixel metric height. </param>
///
/// <returns>	A Geoweb3d status code. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

GW3D_DLL GW3DResult get_EstimatedCentralPixelMetricDimensions(const Geoweb3d::GW3DEnvelope& env, const double width, const double height, double& horizontal_dimension, double& vertical_dimension);

}
}