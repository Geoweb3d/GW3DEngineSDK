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
#include <Geoweb3dCore/Geoweb3dTypes.h>

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of layer previews for layers available within the datasource. </summary>
	///
	/// <remarks>	This collection contains a 'preview' of all available layers, whether or not
	/// 			the layer has been opened.  Layers can be created, opened, and closed through 
	/// 			this collection. 
	/// 			
	/// 			Note that the collection returns shared pointers to layer previews, meaning the client
	/// 			is responsible for the lifetime of the object.  These are not meant to be stored long
	/// 			term, and will no longer be	valid for use if the datasource is closed.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorLayerPreviewCollection : public IGW3DCollection< IGW3DGUIVectorLayerPreviewPtr >
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of layers that have been opened or created within the datasource. 
	///				</summary>
	///
	/// <remarks>	This collection contains a subset of all layers in the datasource, specifically
	/// 			those that have been created or opened by this plugin.  These layers are owned by 
	/// 			the plugin, though if they are added to the table of contents then they may be 
	///				removed by the application user or other plugins.
	/// 			
	/// 			This collection also contains the interfaces for opening and creating layers. 
	///				Layers are not added to the table of contents by default.  To add a layer to the 
	///				table of contents, use IGW3DGUITocItemCollection::add_Layer</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorLayerCollection : public IGW3DCollection< IGW3DGUIVectorLayerWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a layer in the datasource. </summary>
		///
		/// <remarks>	Creates a new layer in the datasource, to which new data can be added and which
		/// 			can be represented in the 3D scene.  Layers created through this function are
		/// 			automatically put into a modifiable mode optimized for modifying data in real time.
		/// 			
		/// 			Note that while the geometry of models, shapes, lines, and icons may be dynamically 
		/// 			modified after being represented, other representations such as light points and 
		/// 			polygons do not yet support this.</remarks>
		///
		/// <param name="name">		 	The name for the new layer (must be unique per datasource). </param>
		/// 
		/// <param name="type">		 	The geometry type of the new layer. </param>
		/// 
		/// <param name="field_parameters">	The field definition for the layer. </param>
		/// 
		/// <param name="properties">	(optional) additional properties. </param>
		///
		/// <returns>	A new layer, or an expired weak pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorLayerWPtr create (const char* name, const GeometryType type, const Geoweb3d::IGW3DDefinitionCollectionPtr &field_parameters, const IGW3DPropertyCollectionPtr& properties = IGW3DPropertyCollectionPtr()) = 0;			
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Open a layer in the datasource </summary>
		///
		/// <remarks>	Opens a layer in the datasource, which can be represented in the 3D scene. </remarks>
		///
		/// <param name="layer">   	The layer preview of the layer to be opened </param>
		///
		/// <returns>	A layer, or an expired weak pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorLayerWPtr open (IGW3DGUIVectorLayerPreviewPtr layer) = 0;	
	};

}
}



