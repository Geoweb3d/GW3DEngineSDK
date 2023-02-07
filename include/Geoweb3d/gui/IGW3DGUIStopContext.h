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
	/// <summary>	Context for shutting down the plugin on application startup. </summary>
	///
	/// <remarks>	Before Geoweb3d Desktop has destroyed all of its internal objects, it will make a
	/// 			function call into OnStop.  This function will only be called once per plugin per
	/// 			application session. </remarks>
	/// 
	/// <see cref="OnStop"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIStopContext
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the shelf collection. </summary>
		///
		/// <remarks>	Provides access to all shelves in the application, both native and those created
		/// 			by plugins. </remarks>
		///
		/// <returns>	the shelf collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIShelfCollection* get_ShelfCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the modifiable shelf collection. </summary>
		///
		/// <remarks>	Provides access to all shelves that may be modified by the plugin. Typically this
		/// 			includes only those shelves created by the current plugin, but a plugin with 
		/// 			rebranding rights can access all shelves via this collection.  The collection also
		/// 			provides access to all functions that modify shelves, and new shelves are created 
		/// 			through this collection. </remarks>
		///
		/// <returns>	null if it fails, else the shelf modifiable collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIShelfModifiableCollection* get_ShelfModifiableCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the dock window collection. </summary>
		///
		/// <remarks>	Provides access to all dock windows in the application, both native and those 
		/// 			created by plugins. </remarks>
		///
		/// <returns>	the dock window collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIDockWindowCollection* get_DockWindowCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the modifiable dock window collection. </summary>
		///
		/// <remarks>	Provides access to all dock windows that may be modified by the plugin. Typically 
		/// 			this includes only those dock windows created by the current plugin, but a plugin 
		/// 			with rebranding rights can access all dock windows via this collection.  The 
		/// 			collection also	provides access to all functions that modify dock windows, and new 
		/// 			dock windows are created through this collection. </remarks>
		///
		/// <returns>	null if it fails, else the dock window modifiable collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIDockWindowModifiableCollection* get_DockWindowModifiableCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the vector driver collection. </summary>
		///
		/// <remarks>	The collection of vector drivers, through which vector layers are created, opened, and accessed </remarks>
		///
		/// <returns>	null if it fails, else the vector driver collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorDriverCollection* get_VectorDriverCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vector representation driver collection. </summary>
		///
		/// <remarks>	The collection of vector representation drivers, through which representations 
		/// 			of vector layers are created and accessed </remarks>
		///
		/// <returns>	null if it fails, else the vector representation driver collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationDriverCollection* get_VectorRepresentationDriverCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets raster representation driver collection. </summary>
		///
		/// <remarks>	The collection of raster representation drivers, through which representations 
		/// 			of raster layers are created and accessed </remarks>
		///
		/// <returns>	null if it fails, else the raster representation driver collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIRasterRepresentationDriverCollection* get_RasterRepresentationDriverCollection( bool reset_next_iterator = false ) = 0;

	};


}
}



