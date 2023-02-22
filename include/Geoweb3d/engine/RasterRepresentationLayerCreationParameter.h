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
#include "../core/IGW3DCollection.h"
#include "GW3DExports.h"

namespace Geoweb3d
{

	namespace Raster
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Representation layer creation parameter. </summary>
		///
		/// <remarks>	Defines parameters of a new representation.  It is more efficient
		/// 			to define these parameters on construction rather than setting them afterward. </remarks>
		///
		/// <see cref="IGW3DVectorRepresentationCollection::create"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		struct GW3D_DLL RasterRepresentationLayerCreationParameter
		{
			virtual ~RasterRepresentationLayerCreationParameter() {}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Default constructor. </summary>
			/// <remarks>	Page level 0 is the unset case, defaults will be applied.  </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			RasterRepresentationLayerCreationParameter() :
				page_level(0), 
				priority(0),
				representation_layer_activity(true)
			{ }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	The page level. A distance ID to start bringing in the file. </summary>
			///
			/// <remarks>	Ranges from 1 - 17 unless other low level configurations have taken place.
			/// 			Lower numbers bringing in the dataset further away, thus memory footprints
			/// 			will increase as well as performance decreasing.  This value range depends
			/// 			on other values internal to the SDK.
			/// 
			///				NOTE: If page level is not set (i.e. 0 then an appropriate default will be choosen
			///					  based on the representation.
			///					Imagery and Imagery-Overlay defaults to 1
			///					Elevation defaults to 11
			///  </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			unsigned page_level;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	The imagery priority (images with higher values display on top of lower priority images). </summary>
			///
			/// <remarks>	Priority is not currently considered for an elevation represenation. 
			///				In that case the highest resolution data source will be prioritized. </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			int priority;

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

		};
	}
}