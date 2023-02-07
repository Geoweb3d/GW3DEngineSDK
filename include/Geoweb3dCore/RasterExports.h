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
#include "Geoweb3dTypes.h"
#include "Geoweb3dExports.h"
#include "ImageryExports.h"
#include "DemExports.h"
#include "RasterTypes.h"

/* Primary namespace */
namespace Geoweb3d
{	
	class GW3DGeometry;

	///* Raster namespace */
	namespace Raster
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets one click database directory. </summary>
		/// 
		/// <param name="directory">	Pathname of the directory. </param>
		///
		/// <returns>	A GW3DResult. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult  SetOneClickDatabaseDirectory(/*in*/ const char* directory );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads imagery from cache. </summary>
		///
		/// <param name="filename">  	Filename of the file. </param>
		/// <param name="cache">	 	The cache. </param>
		/// <param name="load_props">	The load properties. </param>
		/// <param name="handle">	 	[in,out] The handle. </param>
		///
		/// <returns>	The imagery from cache. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult  LoadImageryFromCache(/*in*/ const char* filename, RasterCache const& cache, Imagery::ImageryLoadProperties const& load_props,Raster::RasterSourceHandle & handle );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads dem from cache. </summary>
		///
		/// <param name="filename">  	Filename of the file. </param>
		/// <param name="cache">	 	The cache. </param>
		/// <param name="load_props">	The load properties. </param>
		/// <param name="handle">	 	[in,out] The handle. </param>
		///
		/// <returns>	The dem from cache. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult  LoadDemFromCache(/*in*/ const char* filename, RasterCache const& cache, Dem::DemLoadProperties const& load_props,Raster::RasterSourceHandle & handle );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A raster query. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class GW3D_DLL RasterQuery
		{
		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Default constructor. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			RasterQuery() { }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Destructor. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			virtual ~RasterQuery() { }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Dem query result. </summary>
			///
			/// <param name="handle">	 		The raster handle. </param>
			/// <param name="used_for_scene">	true if this is the elevation raster you will see when rendered. </param>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			virtual void Dem(RasterSourceHandle handle, bool used_for_scene) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Imagery query result. </summary>
			///
			/// <param name="handle">  	The raster handle. </param>
			/// <param name="priority">	The image priority (the highest priority is what gets rendered). </param>
			/// <param name="isactive">	true if active. </param>
			/// 
			/// <see cref="Imagery::LoadImagery"/>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			virtual void Imagery(RasterSourceHandle handle, int priority, bool isactive) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Dem query result. </summary>
			///
			/// <remarks>	This callback version does not currently indicate whether the raster is used in rendering.
			/// It is used with the StreamRasterQuery GW3DEnvelope to simply gather what dem raster are within these
			/// bounds. </remarks>
			/// 
			/// <param name="handle">	 		The raster handle. </param>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			virtual void Dem(RasterSourceHandle handle) = 0;

		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Stream raster query. </summary>
		///
		/// <remarks>	Currently only supports GW3DPoint (x,y) geometry. </remarks>
		///
		/// <param name="geom">  	The geometry. </param>
		/// <param name="stream">	[in] The stream. </param>
		///
		/// <returns>	GW3D_sOk if success </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult  StreamRasterQuery( const GW3DGeometry *geom, RasterQuery& stream );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Stream raster query with an envelope. </summary>
		///
		/// <remarks>	Currently only hooked up for DEM Rasters. </remarks>
		///
		/// <param name="env">  	The envelope. </param>
		/// <param name="stream">	[in] The stream. </param>
		///
		/// <returns>	GW3D_sOk if success </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		GW3D_DLL GW3DResult  StreamRasterQuery( const GW3DEnvelope& env, RasterQuery& stream );

//SKIP-CODE-BEGIN

		// start of the new c like interface after the release...
		/////////**
		//////// *	@brief Fetch the number of registered drivers. 
		//////// *	@param[retval] number of drivers registered
		//////// */
		////////GW3D_DLL int	GetRasterDriverCount();

		/////////**
		//////// *	@brief Fetch the indicated driver. 
		//////// *	@param[in] the name of the driver. 
		//////// *	@param[retval] the driver, or NULL if no driver with that name is found 
		//////// */
		////////GW3D_DLL const RasterDriverHandle		GetRasterDriverByName ( const char *pszName);

		/////////**
		//////// *	@brief Fetch the indicated driver. 
		//////// *	@param[in] the driver index, from 0 to GetRasterDriverCount()-1.
		//////// *	@param[reval] the driver, or NULL if iDriver is out of range
		//////// */
		////////GW3D_DLL const RasterDriverHandle		GetRasterDriverByIndex ( unsigned int index );

		/////////**
		//////// *	@brief Fetch name of driver (file format) of the raster driver. 
		//////// *  This name should be relatively short (10-40 characters), and should 
		//////// *  reflect the underlying file format. 
		//////// *	@param[in] the driver handle
		//////// *	@param[retval] name of this driver.
		//////// */
		////////GW3D_DLL const char*	GetName	( const RasterDriverHandle driver );

		/////////**
		////////*	@brief Fetch the driver this raster source was opened/created with
		////////*	@param[in] the raster
		////////*	@param[retval] the driver, or NULL if failure
		////////*/
		////////GW3D_DLL const RasterDriverHandle		GetRasterDriverByRasterSource ( const RasterSourceHandle rs );

		/////////**
		//////// *	@brief Attempt to open file/connection
		//////// *	@param[in] the name of the file, or data source to try and open. 
		//////// *	@param[in] with this driver.
		//////// *	@param[in] with this driver or "auto found" driver, use these properties(for usernames passwords and etc)
		//////// *	@param[out] NULL on error or if the pass name is not supported by this driver, 
		//////// *				otherwise a RasterSourceHandle. This RasterSourceHandle should be closed 
		//////// *				by deleting the object when it is no longer needed. 
		//////// */
		////////		
		////////GW3D_DLL GW3DResult OpenRasterSource( const char* url, const RasterDriverHandle driver = 0, const PropertyGroup* rasterdriverproperties = 0, /*out*/ RasterSourceHandle &handle );

		/////////**
		////////*  @brief Returns the number of layers/color channels within the raster
		////////*  @param[in] datasource handle
		////////*  @param[reval] number of layers
		////////*/
		////////GW3D_DLL int  GetLayerCount ( RasterSourceHandle ds );

		/////////**
		//////// * @fn GW3D_DLL GW3DResult SetUserDataPtr(RasterSourceHandle, void*p )
		//////// * @brief ability to set a pointer to your own datastructure
		//////// * @param[in] handle The unique identifier
		//////// */
		////////GW3D_DLL GW3DResult SetUserDataPtr( RasterSourceHandle handle, void *p );

		/////////**
		//////// * @fn GW3D_DLL const char* SetUserDataPtr(RasterSourceHandle )
		//////// * @brief gets the pointer set by SetUserDataPtr
		//////// * @param[in] handle The unique identifier
		//////// * returns null of not set.
		//////// */
		////////GW3D_DLL void* GetUserDataPtr( const RasterSourceHandle  handle );

		/////////**
		//////// *	@brief Query the information about a raster, including extents, number of layers, type, and field definitions
		//////// *	@param[in] handle The raster source identifier
		//////// *	@param[out] params raster information if query is successful
		//////// *	@return GW3D_sOk if successful 
		//////// */
		//////////GW3D_DLL GW3DResult QueryLayerInformation(/*in*/ const LayerHandle layerhandle, /*out*/QueryLayerParams& params );

		/////////**
		////////*  @brief Returns the name of the data source. 
		////////*  @param[in] datasource handle
		////////*  @param[reval] character array/string of the datasource name
		////////*/
		////////GW3D_DLL const char *  GetName ( const RasterSourceHandle ds );


		////////GW3D_DLL IGW3DSpatialReferencePtr GetSpatialReference( const  RasterSourceHandle  handle );

		////////GW3D_DLL void SetSpatialReference( const RasterSourceHandle  handle, const IGW3DSpatialReferencePtr  ref );

		/////////**
		//////// * This function returns the default raster representation properties
		//////// * If an individual feature within a vector layer has never been configured via SetProperties, 
		//////// *	This memory is owned by the SDK and should not be deallocated.
		//////// */
		////////GW3D_DLL const PropertyGroup* GetRepresentationRasterDefaultProperties( /*in*/ RasterSourceHandle handle );

		/////////**
		////////*  @fn RepresentAs
		////////*	This is the primary function for vector visualization.  At heart, it's the method to describe how the data should be seen-- whether it's light points, lines, polygons, etc.
		////////*
		////////*	Handle is the handle returned from OpenFile, where the layer names can be queried for the file through GetLayerNames.  
		////////*	The list of representations can be found through QueryKnownRepresentations.
		////////*
		////////*/
		////////GW3D_DLL GW3DResult 	RepresentAs(/*in*/ RasterSourceHandle rh,	/*in*/ RasterDriverHandle driverhandle,	/*in*/ const RepresentationLayerCreationParameter &params,	/*out*/ RepresentationRasterHandle &rethandle );
		////////GW3D_DLL const char *GetName( RepresentationRasterHandle handle );

		//end of start of new clike interface

		//@}
			//enum UnitType
		//{
		//	Linear = 0,
		//	Angular
		//};

		///**
		// *	@struct ImageryQuery
		// *	@brief Query the extents and resolution of an imagery
		// */
		//
		//struct RasterQuery
		//{
		//	RasterQuery() : 
		//		min_lon( 0.0 ), min_lat( 0.0 ),
		//		max_lon( 0.0 ), max_lat( 0.0 ),
		//		x_resolution( 0.0 ), y_resolution( 0.0 ),
		//		width( 0 ), height( 0 ),
		//		number_of_bands(0)	{ }
		//	/* Minimum longitude */
		//	double min_lon;

		//	/* Minimum latitude */
		//	double min_lat;

		//	/* Maximum longitude */
		//	double max_lon;

		//	/* Maximum latitude */
		//	double max_lat;

		//	/* X Resolution of the image in dataset coordinates */
		//	double x_resolution;

		//	/* Y Resolution of the image in dataset coordinates */
		//	double y_resolution;

		//	/* Resolution unit type (linear or angular) */
		//	UnitType resolution_unit_type;

		//	/* Conversion multiplier for the dataset units into meters (if lineal) or degrees (if angular) */
		//	double resolution_unit;

		//	/* Width, in pixels */
		//	int width;

		//	/* Height, in pixels */
		//	int height;

		//	/* Number of bands/channels in the image*/
		//	int number_of_bands;
		//};


		/**
		*	@struct ImageryLoadProperties
		*	@brief Parameters for configuring the Imagery file at load time
		*/

		/**
		* The imagery load properties control the following properties:
		*
		* @param[in] projection_file Projection reference, currently unused
		* @param[in] priority If two images occupy the same space, the one with the greatest priority will be the one used to texture the terrain, if priorities are equal, then the one with the greatest resolution
		* @param[in] stretch Reinterpret the pixel values for each band the minimum value (for all bands) = 0 and the maximum value (for all bands) = 255, and all other values are scaled accordingly
		* @param[in] stretch_min If the minimum pixel value is less than the given value, use this value for the stretch instead.
		* @param[in] stretch_max If the maximum pixel value is greater than the given value, use this value for the stretch instead.
		*/
		/*
		struct ImageryLoadProperties
		{
			ImageryLoadProperties() 
				: priority( 0 )
				, stretch (false)
				, stretch_min (-10000)
				, stretch_max (10000)
				, red_band(0)
				, green_band(1)
				, blue_band(2)
				, alpha_band(-1){}
			
			//used to map different bands in the raster to a color channel
			//note, number of bands can be queried.
			int red_band;
			int green_band;
			int blue_band;
			int alpha_band; 
			
			bool stretch;
			int stretch_min;
			int stretch_max;
			
			int priority;
		};
		*/

		/**
		*	@struct DemLoadProperties
		*	@brief Parameters for configuring the DEM file at load time
		*/

		/**
		* The dem load properties control the following properties:
		*
		* @li @b vertical_exaggertion Vertical exaggeration of the points, conversion like feet to meters can be done here with a vertical_exaggeration of 0.3048 
		* @li @b page_level This is currently fairly abstract, but it reflects the level of the terrain where the dem file starts paging in, lower numbers mean that the file comes in quicker.
		* @li @b invalid_elevation_value Manually set the invalid elevation value for a file
		* @li @b override_dataset If invalid_elevation_value is set and the dataset has an embedded invalid elevation value, this can be used to override the dataset and use the LoadProperties
		* @li @b op Comparison function for if an elevation point is invalid, OP_EQ means if( point == invalid_elevation_value ) discard;, OP_LEQ mean if( point <= invalid_elevation_valid ) discard;
		* @li @b priority Currently does nothing
		*/
		//struct DemLoadProperties
		//{
		//	/* Comparison function for invalid elevation values */
		//	enum OpType
		//	{
		//		DEM_EQ = 0, /* if( point == invalid_elevation_value ) discard; */
		//		DEM_LEQ		/* if( point <= invalid_elevation_value ) discard; */
		//	};


		//	DemLoadProperties() : 
		//		override_dataset( false ), 
		//		invalid_elevation_value( -10000 ), 
		//		op( DEM_LEQ ), 
		//		vertical_exaggeration( 1.0 ),
		//		page_level( 11 ),
		//		elevation_band(0)
		//	{ }

		//	/* Whether or not to override any invalid elevation values in the dataset */
		//	bool override_dataset;

		//	/* Invalid elevation value, if this is not set or override_dataset = false, an attempt will be made to get it from the dataset */
		//	float invalid_elevation_value;
		//	
		//	/* Comparison function for invalid values */
		//	OpType op;

		//	/* Vertical exaggeration of the dataset */
		//	double vertical_exaggeration;
		//	
		//	/* Does not do anything at the moment */
		//	int priority;

		//	/* 
		//		Level of the terrain to start bringing in the file, ranges from 1 - 20 or so,
		//		with lower numbers bringing in the dataset quicker.  
		//		
		//		While this can be advantageous for some resolutions of data, for fine resolutions it is usually unnecessary to bring the data in
		//		before level 11+.  Bringing in high resolution data preemptively will just spike memory overhead.

		//		This is a parameter for the user to play around with and get a feel for it, the default has been selected as a general purpose
		//		value.
		//	*/
		//	unsigned page_level;
		//	
		//	//used to have elevation be pulled from a certain band within a raster.
		//	int elevation_band;
		//};


	//	GW3D_DLL GW3DResult OpenRaster(   /*in*/ const char* filename, /*out*/ unsigned* file_handle );
	//	GW3D_DLL GW3DResult RemoveRaster( /*in*/ unsigned file_handle );
	//	GW3D_DLL GW3DResult QueryRaster(  /*in*/ const unsigned file_handle, /*out*/RasterQuery& raster_info );
	//	GW3D_DLL GW3DResult RepresentRasterAs(/*(dem or imagery or mask? or blend?)*/ /*out*/ unsigned* representation_handle );
	//	GW3D_DLL GW3DResult ApplyDemPropertiesToRaster( const unsigned representation_handle, const DemLoadProperties &props );
	//	GW3D_DLL GW3DResult ApplyImageryPropertiesToRaster(const unsigned representation_handle, const ImageryLoadProperties &props );
	//	GW3D_DLL GW3DResult ExportVectorToRaster( /*in*/ const char* raster_export_name, /*in*/const unsigned handle, /*in*/const char* layer_name, /*in*/ const VectorExport &props );
		
//SKIP-CODE-END

	}

}