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
	/// <summary>	A filter for events occurring with table of contents (TOC) items. </summary>
	///
	/// <remarks>	This interface is inherited by the developer's class, which is registered with
	/// 			Geoweb3d via IGW3DGUIGeoweb3dEventContext::put_TocItemEventFilter. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIGeoweb3dTocItemEventFilter
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback before a table of contents item becomes checked. </summary>
		///
		/// <param name="item">	The table of contents item. </param>
		///
		/// <returns>	return false to disallow the item from being checked
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool AllowChecked (const IGW3DGUITocItemWPtr item) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback before a table of contents item becomes unchecked. </summary>
		///
		/// <param name="item">	The table of contents item. </param>
		///
		/// <returns>	return false to disallow the item from being unchecked
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool AllowUnchecked (const IGW3DGUITocItemWPtr item) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback before a table of contents item becomes moved. </summary>
		///
		/// <param name="item">		The table of contents item to be moved. </param>
		/// <param name="parent">	The table of contents item of which the item is to become a child. 
		///							</param>
		///
		/// <returns>	return false to disallow the item from being moved
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool AllowMoved (const IGW3DGUITocItemWPtr item, const IGW3DGUITocItemWPtr target) = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A filter for events occurring with table of contents (TOC) items. </summary>
	///
	/// <remarks>	This interface is inherited by the developer's class, which is registered with
	/// 			Geoweb3d via IGW3DGUIGeoweb3dEventContext::put_TocItemEventFilter. 
	///
	///				Only one of the OnMenu functions will be called per menu, based on its type.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIGeoweb3dContextMenuEventFilter
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback before a generic menu is displayed to the user. </summary>
		///
		/// <param name="menu">	The menu. </param>
		///
		/// <returns>	return false to disallow the menu from being displayed
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnMenu (const IGW3DGUIMenuWPtr menu) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback before a table of contents context menu is displayed to the user. </summary>
		///
		/// <param name="menu">	The menu. </param>
		/// <param name="target">	The target of the menu, or an expired pointer if none. </param>
		///
		/// <returns>	return false to disallow the menu from being displayed
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnMenu (const IGW3DGUIMenuWPtr menu, IGW3DGUITocItemWPtr target) = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A filter for events resulting in application dialogs being displayed. </summary>
	///
	/// <remarks>	This interface is inherited by the developer's class, which is registered with
	/// 			Geoweb3d via IGW3DGUIGeoweb3dEventContext::put_DialogEventFilter. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIGeoweb3dDialogEventFilter
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback before the 'About' dialog is displayed. </summary>
		///
		/// <remarks>	This function will only be called when the plugin has a rebranding license. 
		/// 			If returning false, it is intended that the plugin display a custom 'About' dialog.
		/// 			</remarks>
		///
		/// <returns>	return false to disallow the 'About' dialog from being displayed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnAboutDialog () = 0;

	};

}
}



