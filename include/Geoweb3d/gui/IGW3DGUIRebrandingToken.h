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
#include "core/GW3DGuids.h"

/* Primary namespace */
namespace Geoweb3d
{
/* Primary GUI namespace */
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A token identifying the plugin as a licensed rebranding plugin. </summary>
	///
	/// <remarks>	The rebranding token must be created in order to access APIs available only to
	/// 			a rebranding plugin. 
	/// 			
	/// 			This class is to be returned inside createClassInstance when the guid 
	/// 			GUID_GEOWEB3DDESKTOP_REBRANDING_TOKEN is requested.  This occurs immediately after 
	/// 			GUID_GEOWEB3DDESKTOP_SDK is requested.  Next, get_ValidateRebrandingLicense will be 
	/// 			called, followed by get_DisableNativeSplashscreen. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIRebrandingToken : public IGW3DIBaseObject
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates the rebranding license. </summary>
		///
		/// <remarks>	If validation fails, the rebranding token will be ignored and the plugin will be 
		/// 			treated like a non-rebranding plugin.
		/// 			
		/// 			Note: the rebranding token is not yet designed, so this API will change.  Currently 
		/// 			any rebranding token created will be validated. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void get_ValidateRebrandingLicense( /*todo*/ ) = 0 ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Disables the native splashscreen. </summary>
		///
		/// <remarks>	If this function returns true, the native Geoweb3d splashscreen will not be 
		/// 			displayed.  The intended behavior is for the plugin to then display a custom 
		/// 			splashscreen, which should be closed inside IGW3DGUISDK::OnFinalConstruct or 
		/// 			IGW3DGUISDK::OnStart. </remarks>
		///
		/// <returns>	true to prevent splashscreen from displaying, false to allow it to display. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_DisableNativeSplashscreen( ) = 0;

	};
	
}
}