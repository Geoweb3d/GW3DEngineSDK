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
	/// <summary>	This context provides access to APIs that apply to the application in general. 
	///				</summary>
	///
	/// <remarks>	This context is delivered to the plugin via the IGW3DGUIApplicationUpdateTimer at
	///				the interval requested in IGW3DGUIFinalConstructContext::put_ApplicationUpdateTimer. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIApplicationUpdateContext
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the collection of all 3D views. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIView3dCollection* get_View3dCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the collection of all Web views. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIViewWebCollection* get_ViewWebCollection( bool reset_next_iterator = false ) = 0;

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
		/// <summary>	Gets the collection of root table of contents (TOC) items. </summary>
		///
		/// <remarks>	The table of contents contains references to all layers loaded by the user
		///				or by projects, and all representations created by plugins with the 'addToToc'
		///				argument set to true.  All the first level items on the TOC can be accessed via
		///				this collection.  The full hierarchy of the TOC can be accessed by querying the
		///				descendants of each item in this collection. </remarks>
		///
		/// <returns>	the root TOC item collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemCollection* get_RootTOCItemCollection( bool reset_next_iterator = false ) = 0;

	};


}
}



