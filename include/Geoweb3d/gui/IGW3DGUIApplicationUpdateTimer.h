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
	/// <summary>	A timer that provides access to the IGW3DGUIApplicationUpdateContext. </summary>
	///
	/// <remarks>	This interface is implemented by the plugin, and is registered with
	/// 			Geoweb3d via IGW3DGUIFinalConstructContext::put_ApplicationUpdateTimer. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIApplicationUpdateTimer
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback with the application update context. </summary>
		///
		/// <remarks>	This will not get called until OnFinalConstruct and OnStart have been completed
		///				for all plugins, and will stop being called before OnStop is called.</remarks>
		///
		/// <param name="context">	The application update context. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnApplicationUpdate (IGW3DGUIApplicationUpdateContext* context) = 0;

	};


}
}



