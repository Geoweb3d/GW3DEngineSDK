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
	/// <summary>	Describes a single vector layer within a datasource. </summary>
	///
	/// <remarks>	A vector layer preview is a lightweight read-only class containing
	/// 			information about a vector layer. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorLayerPreview : public IGW3DGUILayer
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type of the layer. </summary>
		///
		/// <returns>	null if it fails, else the type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char * get_Type() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the layer. </summary>
		///
		/// <returns>	null if it fails, else the name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char * get_Name() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the envelope of the layer. </summary>
		///
		/// <remarks>	The maximum bounding envelope of the vector layer. </remarks>
		///
		/// <returns>	The envelope. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DEnvelope get_Envelope() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the datasource the layer is apart of if one exists. </summary>
		///
		/// <returns>	null if it fails or does not exist, else the datasource name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_DataSourceName() = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A vector layer. </summary>
	///
	/// <remarks>	Once a layer is opened, the SDK returns a vector layer which may be modified
	/// 			and represented.  </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorLayer : public IGW3DGUIVectorLayerPreview
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new feature. </summary>
		///
		/// <remarks>	Creates a new feature in the layer with the given geometry and 
		/// 			attribute values.</remarks>
		///
		/// <param name="field_values">	The field values. Note that the PropertyGroup will soon be 
		/// 							deprecated and replaced by a PropertyCollection </param>
		/// 
		/// <param name="geom">		   	The geometry of the new feature. </param>
		///
		/// <returns>	The feature id (FID) of the new feature, or -1 on failure. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual long create_Feature(  const IGW3DPropertyCollection *field_values, const GW3DGeometry *geom ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new feature. </summary>
		///
		/// <remarks>	Creates a new feature in the layer with the given geometry and 
		/// 			attribute values.</remarks>
		///
		/// <param name="field_values">	The field values. Note that the PropertyGroup will soon be 
		/// 							deprecated and replaced by a PropertyCollection </param>
		/// 
		/// <param name="geom">		   	The geometry of the new feature. </param>
		///
		/// <returns>	The feature id (FID) of the new feature, or -1 on failure. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual long create_Feature(  const IGW3DPropertyCollectionPtr& field_values, const GW3DGeometry *geom ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query and/or modify the geometry and/or attributes of individual features in the layer. </summary>
		///
		/// <remarks>	Iterates through features defined in the provided stream object and provides a 
		/// 			callback per feature, allowing the attribute data and geometry for each feature
		/// 			to be viewed and modified. </remarks>
		///
		/// <param name="streamobj"> The stream object (defined by the plugin developer) that controls 
		/// 						 what features will be streamed </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void Stream( IGW3DGUIVectorLayerStream *streamobj ) = 0; 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query whether this layer was created with a table of contents (TOC) item. </summary>
		///
		/// <returns>	true if the layer has a TOC item, false if it does not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool has_TocItem() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the layer's table of contents (TOC) item. </summary>
		///
		/// <returns>	the TOC item, or an expired weak pointer if the layer does not have a TOC item. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual IGW3DGUITocItemWPtr get_TocItem() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets object feature count. </summary>
		///
		/// <returns>	The object feature count. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned  get_ObjectFeatureCount( ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets geometry type. </summary>
		///
		/// <returns>	The geometry type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GeometryType get_GeometryType( ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets attribute definition collection. </summary>
		///
		/// <returns>	null if it fails, else the attribute definition collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DDefinitionCollection*  get_AttributeDefinitionCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables the ability to edit the geometry within the IGW3DVectorLayerStream </summary>
		///
		/// <param name="bool">	[in] enable/disables "dynamic data" </param>
		/// <param name="bool">	[in] commit existing to "static" data" </param> //commit currently not supported
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_GeometryEditableMode( bool enabled, bool commit ) = 0;

		//////////////////////////////////////////////////////////////////////////////////////////////////// 
 		/// <summary>   Forces all representations to be refreshed after features are created. </summary> 
 		///   
 		/// <remarks>   If you already have a layer represented, create_Feature's retrieval will return a warning to call
 		///			    refresh.  After you are done adding all your features, you should call this so the 3D scene
 		/// 			will properly page in all its data. </remarks>
 		//////////////////////////////////////////////////////////////////////////////////////////////////// 
	 		 
		virtual void force_RepresentationRefresh() = 0; 

	};

}
}


