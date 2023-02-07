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
	/// <summary>	Collection of all shelves that appear at the top of the Geoweb3d user interface. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIShelfCollection : public IGW3DCollection< IGW3DGUIShelfWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a shelf by its unique ID. </summary>
		///
		/// <param name="shelfid">	The shelf id. </param>
		///
		/// <returns>	The shelf (expired pointer if failed). </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIShelfWPtr get_ById ( const char* shelfid ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the active shelf. </summary>
		///
		/// <remarks>	Get the shelf that is currently active. </remarks>
		///
		/// <returns>	The active shelf. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIShelfWPtr get_Active ( ) = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of modifiable shelves. </summary>
	///
	/// <remarks>	Collection containing all shelves that the plugin has rights to modify.
	/// 			This usually includes only shelves created by the current plugin.  However,
	/// 			a plugin with rebranding rights will be able to access all dock windows through
	/// 			this collection. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIShelfModifiableCollection : public IGW3DGUIShelfCollection
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a shelf. </summary>
		///
		/// <remarks>	Create a new shelf, which can be populated with custom frames. </remarks>
		///
		/// <param name="shelfname">   	The shelf name. </param>
		/// <param name="shelfid">	   	The shelf's unique id. </param>
		/// <param name="iconfullpath">	The icon full path. </param>
		///
		/// <returns>	The new shelf. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIShelfWPtr create( const char *shelfname, const char* shelfid, const char* iconfullpath ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes the given shelf the active shelf. </summary>
		///
		/// <remarks>	This will switch the shelf so that the given shelf becomes active in the user 
		/// 			interface. </remarks>
		///
		/// <param name="shelf">	The shelf to be made active. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_Active ( IGW3DGUIShelfWPtr shelf ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the name of the shelf on the tab. </summary>
		///
		/// <remarks>	This will change the name of the tab on the shelf to the given string. </remarks>
		///
		/// <param name="shelf">	The shelf to be renamed. </param>
		/// <param name="name">		The name. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_Name ( IGW3DGUIShelfWPtr shelf, const char* name) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Excludes the given shelf from ever being visible in the user interface. </summary>
		///
		/// <remarks>	Use this API with caution.  If set to false, the shelf will never be visible
		/// 			to the user and the functions it provides will be unavailable to the user. </remarks>
		///
		/// <param name="shelf">  	The shelf. </param>
		/// <param name="excluded">	true to exclude shelf, false to allow it to become visible. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_Excluded ( IGW3DGUIShelfWPtr shelf, bool excluded ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes a frame visible or invisible on the shelf. </summary>
		///
		/// <remarks>	If set to true, the frame will be visible on the shelf.  If false, the frame will 
		/// 			not be visible. </remarks>
		///
		/// <param name="shelf">  	The shelf. </param>
		/// <param name="frame">  	The frame. </param>
		/// <param name="visible">	true to show, false to hide. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_FrameVisible( IGW3DGUIShelfWPtr shelf, IGW3DGUIFrameWPtr frame, bool visible ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Makes a frame enabled or disabled. </summary>
		///
		/// <remarks>	When disabled, a frame will be 'greyed out' and its functions will not be 
		/// 			available to the user. </remarks>
		///
		/// <param name="shelf">  	The shelf. </param>
		/// <param name="frame">  	The frame. </param>
		/// <param name="enabled">	true to enable, false to disable. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_FrameEnabled( IGW3DGUIShelfWPtr shelf, IGW3DGUIFrameWPtr frame, bool enabled ) = 0;

	};

}
}



