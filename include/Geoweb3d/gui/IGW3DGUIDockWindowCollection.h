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
		/// <summary>	The collection of dockable windows. </summary>
		///
		/// <remarks>	Collection containing all dockable windows, both native and created by plugins. </remarks>
		/// 
		/// <see cref="IGW3DGUIDockWindow"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		struct IGW3DGUIDockWindowCollection : public IGW3DCollection< IGW3DGUIDockWindowWPtr >
		{

		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The collection of all modifiable dockable windows. </summary>
		///
		/// <remarks>	Collection containing all dockable windows that the plugin has rights to modify.
		/// 			This usually includes only dock windows created by the current plugin.  However,
		/// 			a plugin with rebranding rights will be able to access all dock windows through 
		/// 			this collection. </remarks>
		/// 
		/// <see cref="IGW3DGUIDockWindow"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		struct IGW3DGUIDockWindowModifiableCollection : public IGW3DGUIDockWindowCollection
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Values describing the docking area for a dock window. </summary>
			///
			/// <remarks>	This enum is used to define the default position of the dock window.
			/// 			This may be overridden by the user by shutting down the application
			/// 			with the dock window in a new position. </remarks>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			enum DockArea 
			{
				/// <summary>	The dockable area to the left of the application. </summary>
				LeftDockArea = 0x1,	
				/// <summary>	The dockable area to the right of the application. </summary>
				RightDockArea = 0x2,	
				/// <summary>	The dockable area to the top of the application. </summary>
				TopDockArea = 0x4,	
				/// <summary>	The dockable area to the bottom of the application. </summary>
				BottomDockArea = 0x8,
				/// <summary>	Do Not Use. </summary>
				DockAreaMax = 0xFF
			};

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates a dockable window. </summary>
			///
			/// <remarks>	A dockable window will be created with the given name, in the specified area.
			/// 			The window geometry will be saved at the end of every application session, so
			/// 			area (as well as initial window visibility) will be overridden by the user's
			/// 			own configuration.  The application uses the windowname as the identifier for
			/// 			the purpose of storing and retrieving user settings.  NOTE: this will change
			/// 			to use the window ID instead!!! </remarks>
			///
			/// <param name="windowname">	The name for the new window. </param>
			/// <param name="id">		 	The unique id for the new dock window. </param>
			/// <param name="dockarea">  	The default location for the dockable window. </param>
			///
			/// <returns>	the new dockwindow.</returns>
			////////////////////////////////////////////////////////////////////////////////////////////////////

			virtual IGW3DGUIDockWindowWPtr create( const char *windowname, const char* id, DockArea dockarea ) = 0;
		};
	}

}



