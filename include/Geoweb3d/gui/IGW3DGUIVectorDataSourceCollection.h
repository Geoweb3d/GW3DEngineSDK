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

namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>The vector datasource collection. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorDataSourceCollection : public IGW3DCollection< IGW3DGUIVectorDataSourceWPtr >
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	This method attempts to create a new datasource based on the parent driver. </summary>
		///
		/// <param name="pszName">				Name for the new datasource. </param>
		/// <param name="driver_properties">	(optional) additional driver properties. </param>
		///
		/// <returns>	The new data source. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDataSourceWPtr create( const char * pszName, const IGW3DPropertyCollectionPtr driver_properties = IGW3DPropertyCollectionPtr()  ) = 0;
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a datasource by name. </summary>
		///
		/// <param name="name">	The name of the requested datasource. </param>
		///
		/// <returns>	The datasource (if failed, pointer will be expired). </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDataSourceWPtr get_ByName( const char* name ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Attempt to open file/connection. </summary>
		///
		/// <remarks>	Given a datasource definition, such as file path, url, or a GDAL connection string,
		/// 			attempts to open a vector datasource. </remarks>
		///
		/// <param name="pszName">				The name of the file or data source (i.e. path or url) to attempt to open. </param>
		/// <param name="driver_properties">	(optional) additional driver properties. </param>
		///
		/// <returns>	The datasource (if failed, pointer will be expired). </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDataSourceWPtr open( const char *  pszName, const IGW3DPropertyCollectionPtr driver_properties = IGW3DPropertyCollectionPtr()  ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the driver. </summary>
		///
		/// <remarks>	Get the driver to which this collection belongs. </remarks>
		///
		/// <returns>	the vector driver. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDriverWPtr get_Driver() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Close a datasource. </summary>
		///
		/// <remarks>	This will close the datasource and remove it from the collection.
		///				If this datasource had resources tied to it (such as a representation), those
		///				resources will to be destroyed.  One example would be is if you had a weak pointer
		///				to a representation on this datasource, its ".expired()" should be true.
		///				Note, this will reset the iterator.
		/// 			
		/// 			To close a datasource, you can do this:
		///				IGW3DGUIVectorDataSourceCollection::close( ds );
		///				Note, IGW3DGUIVectorDataSourceWPtr will become expired. </remarks>
		///
		/// <param name="ds">	[in] The datasource. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close( IGW3DGUIVectorDataSourceWPtr &ds  )
		{
			//ds.lock()->get_Driver().lock()->get_VectorDataSourceCollection()->close_DataSource(ds);
		}

	};


}
}

