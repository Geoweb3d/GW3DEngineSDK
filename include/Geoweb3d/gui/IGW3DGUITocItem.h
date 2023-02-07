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
	/// <summary>	An item in the Table of Contents (TOC). </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUITocItem
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent TOC item type. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum GW3DGUITocItemType
		{
			/// <summary>	An enum constant representing an undefined item. </summary>
			Other = 0,
			/// <summary>	An enum constant representing a group layer item. </summary>
			GroupLayer,
			/// <summary>	An enum constant representing a vector layer item. </summary>
			VectorLayer,
			/// <summary>	An enum constant representing a raster layer item. </summary>
			RasterLayer,
			/// <summary>	An enum constant representing a kml layer item. </summary>
			KmlLayer,
			/// <summary>	An enum constant representing a vector representation item. </summary>
			VectorRepresentation,	
			/// <summary>	An enum constant representing a raster representation item. </summary>
			RasterRepresentation,
			/// <summary>	An enum constant representing a camera view item. </summary>
			CameraView,
			/// <summary>	An enum constant representing a tour item. </summary>
			Tour,
			/// <summary>	An enum constant representing a viewshed analysis item. </summary>
			ViewshedAnalysis,
			/// <summary>	Do Not Use. </summary>
			TIT_MAX = 0xff
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type of the TOC item. </summary>
		///
		/// <remarks>	This type indicates what the TOC item represents. </remarks>
		///
		/// <returns>	The type of the TOC item. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DGUITocItemType get_Type() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vector layer. </summary>
		///
		/// <remarks>	Get the vector layer if the item is of the VectorLayer type. </remarks>
		///
		/// <returns>	The vector layer, or an expired pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorLayerWPtr get_VectorLayer() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets vector representation. </summary>
		///
		/// <remarks>	Get the vector representation if the item is of the VectorRepresentation type. </remarks>
		///
		/// <returns>	The vector representation, or an expired pointer if failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationWPtr get_VectorRepresentation() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the name of the TOC item. </summary>
		///
		/// <remarks>	This will set the name of the item, which is displayed as the text of the TOC
		/// 			item.  This will fail if the TOC item does not support renaming. </remarks>
		///
		/// <param name="itemname">	The new name for the item. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_Name (const char* itemname) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the TOC item. </summary>
		///
		/// <remarks>	This provides the text that is displayed on the TOC item. </remarks>
		///
		/// <returns>	the name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Name () = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the parent of this item. </summary>
		///
		/// <remarks>	Gets the direct parent of the TOC item. </remarks>
		///
		/// <returns>	The parent. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemWPtr get_Parent() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the collection of all children. </summary>
		///
		/// <remarks>	Gets all the child items for this TOC item. </remarks>
		///
		/// <returns>	the collection of child TOC items. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUITocItemCollection* get_ChildCollection() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the checked state. </summary>
		///
		/// <remarks>	Gets the checked state of the TOC item. </remarks>
		///
		/// <returns>	true if checked, false if unchecked. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Checked() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the TOC item to be checked or unchecked. </summary>
		/// 
		/// <param name="checked">	true to set as checked, false to set as unchecked. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Checked (bool checked) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Visibility mode of the TOC item </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		enum GW3DGUITocItemVisibility
		{
			/// <summary>	TOC item is permanently hidden </summary>
			Hidden = 0,
			/// <summary>	TOC item is visible or hidden based on context (default) </summary>
			Contextual,
			/// <summary>	TOC item is always visible regardless of context (not recommended) </summary>
			VisibleAlways,
			/// <summary>	Do Not Use. </summary>
			TIV_MAX = 0xff
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the visibility mode of the TOC item. </summary>
		///
		/// <remarks>	This gets the visibility mode of a TOC item. </remarks>
		///
		/// <returns>	The visibility. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DGUITocItemVisibility get_Visibility() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the visibility mode of the TOC item. </summary>
		///
		/// <param name="visibility">	The visibility mode. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Visibility (GW3DGUITocItemVisibility visibility) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the current visibility. </summary>
		///
		/// <returns>	true if the item is currently visible in the TOC, false if it is hidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Visible() = 0;

	};
}
}



