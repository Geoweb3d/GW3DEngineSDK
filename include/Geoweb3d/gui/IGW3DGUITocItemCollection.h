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
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of TOC items. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUITocItemCollection : public IGW3DCollection< IGW3DGUITocItemWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Moves the given TOC item to this collection, optionally at a given index. </summary>
		///
		/// <remarks>	If no index is provided, the item will be moved to the bottom-most position
		/// 			amongst the item's children.  If the move is not allowed, an error will be returned.
		/// 			For example, representations may not be moved to a new parent, layers may only
		/// 			be moved in and out of groups, and tours may be moved under a variety of parent
		/// 			items. </remarks>
		///
		/// <param name="item"> 	The item to be moved. </param>
		/// <param name="index">	(optional) zero-based index of the new position within the collection.
		/// 						 </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult move ( IGW3DGUITocItemWPtr item, int index = -1 ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a group in the table of contents. </summary>
		///
		/// <remarks>	Creates a group as a child of the IGW3DGUITocItemWPtr for this collection.
		///				The item will be created at the bottom-most position amongst the item's children.
		///
		///				If the group cannot be created, an expired pointer will be returned.  Groups may only 
		///				be created under the root TOC item collection, or under another group.
		/// 			 </remarks>
		///
		/// <param name="name"> 	The name of the new group. </param>
		///	<param name="checked">	(optional) The initial checked state of the new group.
		/// 						 </param>
		///
		/// <returns>	The new group, or an expired pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemWPtr create_Group ( const char* name, bool checked = true ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a group in the table of contents. </summary>
		///
		/// <remarks>	Creates a group as a child of the IGW3DGUITocItemWPtr for this collection.
		///				If an expired 'preceding' is provided, the item will be created at the top-most
		/// 			position among the item's children.  Otherwise, the item will be placed directly below 
		///				the 'preceding' IGW3DGUITocItemWPtr.
		///
		///				If the group cannot be created, an expired pointer will be returned.  Groups may only 
		///				be created under the root TOC item collection, or under another group.
		/// 			 </remarks>
		///
		/// <param name="name"> 	The name of the new group. </param>
		/// <param name="preceding"> The item to precede the new item, or an expired pointer to make first.
		///	<param name="checked">	(optional) The initial checked state of the new group.
		/// 						 </param>
		///
		/// <returns>	The new group, or an expired pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemWPtr create_Group ( const char* name, IGW3DGUITocItemWPtr preceding, bool checked = true ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add a layer to the table of contents. </summary>
		///
		/// <remarks>	Adds the layer as a child of the IGW3DGUITocItemWPtr for this collection.
		///				The item will be created at the bottom-most position amongst the item's children.  
		///
		///				Existing and future representations of the layer will also be added to the table 
		///				contents, and their visibility will henceforth be controlled by the table of contents.
		///				Basic representations (IGW3DGUIVectorRepresentationBasic) are never added to the 
		///				table of contents, but it is not recommended that layers in the table of contents
		///				be given basic representations, since their visibility etc. may be contradictory.
		///
		///				If the layer cannot be added, or if the layer already has been added, an expired 
		///				pointer will be returned.  Layers may only be added to the root TOC item collection, 
		///				or under a group.
		///
		///				Note that once a layer has been added to the table of contents it may be removed by
		///				the application user via the table of contents, or by another plugin via the 
		///				TOC Item interface.  It will also be saved to project files independent of the plugin, 
		///				closed when a new project is started or a project is opened.
		/// 			 </remarks>
		///
		/// <param name="layer"> 	The layer to add. </param>
		///	<param name="checked">	(optional) The initial checked state of the layer.
		/// 						 </param>
		///
		/// <returns>	The new table of contents item for the layer, or an expired pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemWPtr add_Layer ( IGW3DGUIVectorLayerWPtr layer, bool checked = true ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add a layer to the table of contents. </summary>
		///
		/// <remarks>	Adds the layer as a child of the IGW3DGUITocItemWPtr for this collection.
		///				If an expired 'preceding' is provided, the item will be created at the top-most 
		/// 			position amongst the item's children.  Otherwise, the item will be placed directly 
		///				below the 'preceding' IGW3DGUITocItemWPtr.
		///
		///				Existing and future representations of the layer will also be added to the table 
		///				contents, and their visibility will henceforth be controlled by the table of contents.
		///				Basic representations (IGW3DGUIVectorRepresentationBasic) are never added to the 
		///				table of contents, but it is not recommended that layers in the table of contents
		///				be given basic representations, since their visibility etc. may be contradictory.
		///
		///				If the layer cannot be added, or if the layer already has been added, an expired 
		///				pointer will be returned.  Layers may only be added to the root TOC item collection, 
		///				or under a group.
		///
		///				Note that once a layer has been added to the table of contents it may be removed by
		///				the application user via the table of contents, or by another plugin via the 
		///				TOC Item interface.  It will also be saved to project files independent of the plugin, 
		///				closed when a new project is started or a project is opened.
		/// 			 </remarks>
		///
		/// <param name="layer"> 	The layer to add. </param>
		/// <param name="preceding">The item to precede the new item.
		///	<param name="checked">	(optional) The initial checked state of the layer.
		/// 						 </param>
		///
		/// <returns>	The new table of contents item for the layer, or an expired pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemWPtr add_Layer ( IGW3DGUIVectorLayerWPtr layer, IGW3DGUITocItemWPtr preceding, bool checked = true ) = 0;

	};


}
}



