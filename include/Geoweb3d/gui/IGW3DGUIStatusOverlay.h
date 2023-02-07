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
	/// <summary>	Overlay window for text messages </summary>
	///
	/// <remarks>	The status overlay can be used to display text messages at the top of the 3D window.
	/// 			Creation is not necessary.  Turn on and off with set_Visible().  Updating the text
	/// 			before and after display is done with put_Text()</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIStatusOverlay 
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	 Status overlay visibility</summary>
		///
		/// <remarks>	 Toggle the display of the status overlay message</remarks>
		///
		/// <param name= "visible">	true to show, false to hide. </param>
		///
		/// <returns>	 GW3D_sOk if successful </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Visible( bool visible) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	 Status overlay visibility</summary>
		///
		/// <remarks>	 Return the current visibility of the status overlay</remarks>
		///
		/// <returns>	 True if visible, false if not </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Visible( ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	 Set the text of the status overlay</summary>
		///
		/// <remarks>	 Update the text of the status overlay.  The overlay does not need to be displayed for
		//			     the text to be modified</remarks>
		///
		/// <param name= "text"> Text for overlay </param>
		///
		/// <returns>	 GW3D_sOk if successful </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Text( const char *text ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	 Get current text</summary>
		///
		/// <remarks>	 Return the current text of the status overlay.  The overlay does not need to be
		/// 			 displayed to query the text.</remarks>
		///
		/// <returns>	 The current text string </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char *get_Text( ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	 Set the icon of the status overlay</summary>
		///
		/// <remarks>	 Update the icon of the status overlay.  The overlay does not need to be displayed for
		//			     the icon to be modified</remarks>
		///
		/// <param name= "icon"> Url of icon </param>
		///
		/// <returns>	 GW3D_sOk if successful </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Icon( const char *icon ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	 Get current icon</summary>
		///
		/// <remarks>	 Return the current icon of the status overlay.  The overlay does not need to be
		/// 			 displayed to query the icon.</remarks>
		///
		/// <returns>	 The current text string </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char *get_Icon( ) const = 0;

	};

}
}



