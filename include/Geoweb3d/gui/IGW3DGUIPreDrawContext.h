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
	/// <summary>	The pre draw context </summary>
	///
	/// <remarks>	The pre draw context is provided to the plugin just before any 3D view is drawn. </remarks>
	/// 
	/// <see cref="OnPostDraw"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIPreDrawContext
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

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the 3D view. </summary>
		///
		/// <remarks>	The active 3D view for this draw </remarks>
		///
		/// <returns>	The 3D view. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIView3dWPtr get_View() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the collection of line segment intersection tests. </summary>
		///
		/// <remarks>	A line segment intersection test will, when active, report all vector
		/// 			representations that were intersected by the test's line segment in the
		/// 			previous draw.  All line segment intersection tests are created and accessed
		/// 			though the collection.  </remarks>
		///
		/// <returns>	null if it fails, else the line segment intersection test query collection.</returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUILineSegmentIntersectionTestCollection* get_LineSegmentIntersectionTestCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets 2D intersection test collection. </summary>
		///
		/// <param name="reset_next_iterator">	(optional) reset the iterator. </param>
		///
		/// <returns>	null if it fails, else the 2D intersection test collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUI2DIntersectionTestCollection* get_2DIntersectionTestCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the collection of root table of contents (TOC) items </summary>
		///
		/// <remarks>	The table of contents contains references to all layers loaded by the user
		///				or by projects, and all representations created by plugins with the 'addToToc'
		///				argument set to true.  All the first level items on the TOC can be accessed via
		///				this collection.  The full hierarchy of the TOC can be accessed by querying the
		///				descendants of each item in this collection.</remarks>
		///
		/// <returns>	the root TOC item collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemCollection* get_RootTOCItemCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the scene graph context. </summary>
		///
		/// <remarks>	The scene graph context provides access to functions relating to the Geoweb3d
		/// 			Engine scene graph, including image and image palette management (such as
		/// 			textures for extruded polygons. </remarks>
		///
		/// <returns>	the scene graph context. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DSceneGraphContext* get_SceneGraphContext( ) = 0;

	};

}
}



