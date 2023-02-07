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
#include "../core/GW3DInterFace.h"
#include "../../Geoweb3dCore/GeometryExports.h"
#include "GW3DExports.h"

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// namespace: Vector
	////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace Vector
	{
		struct IGW3DConstructionCallback;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Representation layer creation parameter. </summary>
		///
		/// <remarks>	Defines parameters of a new representation.  It is more efficient
		/// 			to define these parameters on construction rather than setting them afterward. </remarks>
		///
		/// <see cref="IGW3DVectorRepresentationCollection::create"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		struct GW3D_DLL RepresentationLayerCreationParameter
		{
			virtual ~RepresentationLayerCreationParameter() {}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Default constructor. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			RepresentationLayerCreationParameter() :
				cb(0),
				use_spatial_filter(false),
				page_level(11),
				representation_layer_activity(true)
			{ }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	A function to ignore/keep features for visualization with the ConstructionCallback signature. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			IGW3DConstructionCallback* cb;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	true to use spatial filter, restricting the representation to the envelope spatial_filter_bounds. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			bool use_spatial_filter;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	The spatial filter bounds, in WGS84 Geodetic.  Set use_spatial_filter to true to use. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			GW3DEnvelope spatial_filter_bounds;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	The page level. A distance ID to start bringing in the file. </summary>
			///
			/// <remarks>	Ranges from 1 - 17 unless other low level configurations have taken place.
			/// 			Lower numbers bringing in the dataset further away, thus memory footprints
			/// 			will increase as well as performance decreasing.  This value range depends
			/// 			on other values internal to the SDK. </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			unsigned page_level;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	true to have representation active on construction. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			bool representation_layer_activity;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Options for controlling the representation default properties. </summary>
			///
			/// <remarks>	Internally will call SetRepresentationLayerDefaultProperties
			/// 			with these values. Note, its best to clone the global
			/// 			properties first (from the representation driver), and then
			/// 			set the properties from it.  This way if a new property is added
			/// 			in a future version of Geoweb3d, you are ensured to be compatible.
			///				If this is not set, the global settings for this representation
			///				will be used. </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			IGW3DPropertyCollectionPtr representation_default_parameters;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	If a layer supports field grouping, you can set this handle. </summary>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			IGW3DVectorLayerFieldFilterPtr field_filter;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the image palette to be used in this representation. </summary>
			///
			/// <remarks>	The images defined in the token will be used as a palette, where the properties that
			/// 			use it will reference the palette images using the index of the image within the
			/// 			image collection that created it.  Note that changes to the image collection after
			/// 			this finalization token was generated do not affect the representation.  Any
			/// 			existing palette will be overwritten.  Undefined results if you have  feature/object
			/// 			ids mapped outside of a palette's maximum index. </remarks>
			///
			/// <param name="images">	The image finalization token. </param>
			///
			/// <returns> TODO </returns>
			///
			/// <see cref="IGW3DImageCollection::create_FinalizeToken"/>
			/// <see cref="IGW3DVectorRepresentation::put_GW3DFinalizationToken"/>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			IGW3DTokenPtr finalization_token;
		};
	}
}
