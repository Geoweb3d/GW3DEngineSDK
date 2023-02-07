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
	/// <summary>	Context for setting up the plugin on application startup. </summary>
	///
	/// <remarks>	Once Geoweb3d Desktop has created all of its internal objects, it will make a
	/// 			function call into OnStart. This function will only be called once per plugin
	/// 			per application session. </remarks>
	/// 
	/// <see cref="OnStart"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIFinalConstructContext
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets Geoweb3d event context. </summary>
		///
		/// <remarks>	To be notified of events relating to Geoweb3d objects, use the Geoweb3d 
		/// 			event context to register event listeners.  Available event types include
		/// 			layer, representation, attribute map and table of contents item events. </remarks>
		///
		/// <returns>	The Geoweb3d event context. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIGeoweb3dEventContext* get_Geoweb3dEventContext() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the application update timer. </summary>
		///
		/// <remarks>	If the plugin requires interfaces for updating the application independent of 
		///				the 3D draw cycle and discrete events, it may subscribe to the application update 
		///				context (IGW3DGUIApplicationUpdateContext) at a chosen interval. </remarks>
		///
		/// <param name="timer">	The application update timer. </param>
		/// <param name="interval">	The interval in ms that the timer should be called. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_ApplicationUpdateTimer ( IGW3DGUIApplicationUpdateTimer* timer, int interval = 1000 ) = 0;

	};

}
}



