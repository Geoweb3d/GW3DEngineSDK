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
#include "core/GW3DGUIInterFace.h"


/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Vector Driver. </summary>
	///
	/// <remarks>	A vector driver is used for opening, creating or closing a vector datasource.  
	/// 			The driver defines the file or database format the datasources belonging to it.
	/// 			If the appropriate driver for a datasource is unknown, open the datasource using
	/// 			the auto_Open function in the vector driver collection.	 </remarks>
	/// 
	/// <see cref="IGW3DGUIVectorDriverCollection"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorDriver : public IGW3DGUIDriver
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the driver's vector data source collection. </summary>
		///
		/// <remarks>	Gets the collection of vector datasources that have been opened or created
		/// 			using this driver. </remarks>
		///
		/// <returns>	null if it fails, else the vector data source collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDataSourceCollection * get_VectorDataSourceCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the vector driver name. </summary>
		///
		/// <remarks>	Gets the name of the vector driver.  This name should be relatively short 
		/// 			(10-40 characters), and should reflect the underlying file format. For 
		/// 			instance "ESRI Shapefile".   </remarks>
		///
		/// <returns>	null if it fails, else the name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Name() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A macro that defines if creating data sources is supported. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		#define isCreateDataSourceSupported  "CreateDataSource"

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A macro that defines if deleting data sources is supported. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		#define isDeleteDataSourceSupported  "DeleteDataSource"

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check if the driver is capable of a given behavior. </summary>
		///
		/// <param name="isSupported">	The capability to be queried </param>
		///
		/// <returns>	true if the driver is capable, false if it is not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_isCapable( const char *isSupported ) = 0;

	};

}
}



