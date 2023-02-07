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
	/// <summary>	A dockable window. </summary>
	///
	/// <remarks>	A custom dockable window that can always e available to the user, or only 
	/// 			when made visible by the plugin.  Dockable windows can only be created on 
	/// 			application startup in the dock window creation context.</remarks>
	/// 
	/// <see cref="IGW3DGUIDockWindowCollection"/>
	/// <see cref="IGW3DGUIDockWindowCreationContext"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIDockWindow 
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Windows handle of the dock window's drawable area. </summary>
		///
		/// <remarks>	The operating system window handle for the drawable area of the dockable window
		///				This handle is used for drawing your UI controls onto the dock window and for
		///				processing operating system events.  This may be cast to an HWND.
		///				
		///				This will return null if called on any native Geoweb3d dockable window.  </remarks>
		///
		/// <returns>	A window handle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual OSWinHandle get_AttachHandle() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the dockable window. </summary>
		///
		/// <remarks>	This returns the internal name of the dock window as displayed on the
		///				window title bar.  For dock windows created by the plugin, this is the
		///				name that the dock window was created with.
		///				
		///				Note: This name is used as an identifier for restoring the window state and
		///				location between application sessions.</remarks>
		///
		/// <returns>	the dock window name. </returns>
		/// 
		/// <see cref="IGW3DGUIDockWindowCreationContext"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char * get_Name() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the name of the dockable window. </summary>
		///
		/// <remarks>	This set the internal name of the dock window as displayed on the
		///				window title bar.</remarks>
		///
		/// <param name="name">	the dock window name. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_Name( const char *name ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the unique id of the dock window. </summary>
		///
		/// <returns>	the id. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char * get_Id() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the dock window to be visible or hidden. </summary>
		///
		/// <remarks>	To make the dock window visible, set this to true.  To hide the window, set this to false.
		/// 			Unless the plugin has rebranding rights, this will fail if called on a dockable
		///				window that was not created by the plugin.  Dock windows are hidden by default. </remarks>
		///
		/// <param name="visible">	true to show, false to hide. </param>
		///
		/// <returns>	GW3D_sOk if successful </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Visible( bool visible ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the visibility of the dock window. </summary>
		///
		/// <remarks>	Check whether the dockable window is visible or hidden.  Dock windows are
		/// 			hidden by default. </remarks>
		///
		/// <returns>	true if visible, false if hidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Visible( ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Disable or enable a dock window. </summary>
		///
		/// <remarks>	When a dock window is disabled, it is 'greyed out' and no longer accepts user input
		///				Unless the plugin has rebranding rights, this will fail if called on a dockable
		///				window that was not created by the plugin </remarks>
		///
		/// <param name="enabled">	true to enable, false to disable. </param>
		///
		/// <returns>	GW3D_sOk if successful </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Enabled( bool enabled ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets if the dock window is enabled. </summary>
		///
		/// <remarks>	Check whether the dockable window is enabled or disabled. </remarks>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled( ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets whether the dock window should be displayed in application menus. </summary>
		///
		/// <remarks>	If this is set to false, the user will not be able to enable/disable the window
		///				from the dockable window selection menu available at the bottom right
		///				of Geoweb3d Desktop.  Also note that once closed, the user will have no
		///				ability to open the dockable window again via the standard user interface.
		///				Unless the plugin has rebranding rights, this will fail if called on a dockable
		///				window that was not created by the plugin. </remarks>
		///
		/// <param name="enabled">	true to show in menus, false to not show in menus. </param>
		///
		/// <returns>	GW3D_sOk if successful </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_ShowInMenus( bool enabled ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets if dockable window is set to be shown in menus. </summary>
		///
		/// <remarks>	Check whether the dockable window is set to be shown in application menus,
		///				specifically the dockable window selection menu at the bottom right of
		///				Geoweb3d Desktop. </remarks>
		///
		/// <returns>	true if shown in menus, false if not shown in menus. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_ShowInMenus( ) const = 0;

	};

}
}



