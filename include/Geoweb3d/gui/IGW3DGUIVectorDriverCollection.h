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
#include "../core/GW3DGUIInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The collection of all available vector drivers. </summary>
	///
	/// <remarks>	The collection of all vector drivers available for opening and/or 
	/// 			creating datasources. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorDriverCollection : public IGW3DCollection< IGW3DGUIVectorDriverWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the driver of the specified name. </summary>
		///
		/// <remarks>	If the name of the desired datasource driver is known, it may be requested
		/// 			using this function. </remarks>
		///
		/// <param name="name">	The name of the driver to be returned </param>
		/// 
		/// <example> <code>get_Driver("Geoweb3d_Datasource");</code> will return the internal driver 
		/// 		  for efficiently displaying dynamic data within Geoweb3d.
		///
		/// <returns>	The driver. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDriverWPtr get_Driver( const char *name ) = 0;
	   
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Open a datasource without choosing a specific driver. </summary>
		///
		/// <remarks>	This function will automatically choose the most appropriate driver for the given name.
		///				To determine what driver was chosen, simply call get_Driver on the returned datasource.
		///				If no appropriate driver could be found, an expired pointer will be returned. </remarks>
		///
		/// <param name="name">					Name of the datasource to be opened .
		/// 									(typically a file path or url)</param>
		/// 
		/// <param name="driver_properties">	(optional) The driver properties. </param>
		///
		/// <returns>	the datasource, or an expired weak pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDataSourceWPtr auto_Open( const char * name, const IGW3DPropertyCollectionPtr driver_properties = IGW3DPropertyCollectionPtr()  ) = 0;

	};

}
}


