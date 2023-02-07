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
	/// <summary>	The result for a single feature in a vector layer stream. </summary>
	///
	/// <remarks>	The layer stream result provides interfaces for viewing and modifying geometry and attributes
	///				of the currently streamed feature. </remarks>
	/// 
	/// <see cref="IGW3DGUIVectorLayer::Stream"/>
	/// <see cref="IGW3DGUIVectorLayerStream::OnStream"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorLayerStreamResult
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the object ID for the feature currently being streamed. </summary>
		///
		/// <remarks>	The object ID, also known in most cases as the feature ID or FID, is the unique
		/// 			identifier for the object in the vector layer. </remarks>
		///
		/// <returns>	The object identifier. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int64_t get_ObjectID() const = 0;
			
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the layer's attribute collection. </summary>
		///
		/// <remarks>	Gets the vector layer's attribute field values. </remarks>
		///
		/// <returns>	The attribute collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DAttributeCollection *get_AttributeCollection()  = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the layer's Modifiable attribute collection. </summary>
		///
		/// <remarks>	Get the layer's data fields (attributes), in a form in which they may be modified.
		/// 			Any changes made to field values will be synced to Geoweb3d's internal data
		/// 			structures, not to the actual source file or database (except for the layers of a
		/// 			"Geoweb3d_DataSource" datasource, where they are the same). </remarks>
		///
		/// <returns>	the Modifiable attribute collection if layer supports it and is in a modifiable mode
		/// 			(i.e. it was created via the 'create' function), else null. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DAttributeCollection * get_EditableAttributeCollection() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the geometry for the feature. </summary>
		///
		/// <remarks>	Gets the geometry for the current feature/object.  The type of the geometry
		/// 			may be determined using Geometry::get_GeometryType, upon which it may be cast
		/// 			to the correct derived geometry class. </remarks>
		///
		/// <returns>	The feature's geometry. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const GW3DGeometry *get_Geometry() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the Modifiable geometry for the feature. </summary>
		///
		/// <remarks>	Gets the geometry for the current feature/object.  The type of the geometry
		/// 			may be determined using Geometry::get_GeometryType, upon which it may be cast
		/// 			to the correct derived geometry class. </remarks>
		///
		/// <returns>	The feature's Modifiable geometry if layer supports it and is in a modifiable mode
		/// 			(i.e. it was created via the 'create' function), else null. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DGeometry *get_EditableGeometry() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vector representation properties for the requested representation. </summary>
		///
		/// <remarks>	Gets the vector representation properties for the current feature/object, for
		/// 			the specified representation.  This is more efficient than requesting it through
		/// 			the representation by feature id. </remarks>
		///
		/// <param name="rep">	The representation. </param>
		///
		/// <returns>	null if it fails, else the vector representation properties. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DPropertyCollection* get_VectorRepresentationProperties( IGW3DGUIVectorRepresentationWPtr rep ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vector representation properties for the requested basic representation. </summary>
		///
		/// <remarks>	Gets the vector basic representation properties for the current feature/object, for
		/// 			the specified representation.  This is more efficient than requesting it through
		/// 			the representation by feature id. </remarks>
		///
		/// <param name="rep">	The basic representation. </param>
		///
		/// <returns>	null if it fails, else the vector basic representation properties. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DPropertyCollection* get_VectorRepresentationProperties( IGW3DGUIVectorRepresentationBasicWPtr rep ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a vector representation property. </summary>
		///
		/// <remarks>	Sets a property of a basic representation for the current feature.  This is more
		/// 			efficient than setting an entire property collection through the representation. 
		/// 			This is only supported for basic representations. </remarks>
		///
		/// <param name="rep">			 	The representation. </param>
		/// <param name="property_index">	Zero-based index of the property. </param>
		/// <param name="value">		 	The value. </param>
		///
		/// <returns>	GW3D_sOk on success </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_VectorRepresentationProperty(IGW3DGUIVectorRepresentationBasicWPtr rep, unsigned property_index, const IGW3DVariant &value) = 0;

			
	};

}
}