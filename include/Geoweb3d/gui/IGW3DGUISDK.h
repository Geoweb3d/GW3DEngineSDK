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

#define MAX_QT_VERSION 0x060402

//
//QT_VERSION_MAJOR, QT_VERSION_MINOR, QT_VERSION_PATCH

#if (QT_VERSION > MAX_QT_VERSION)
#error Your Qt version exceeds the maximum Qt version!  Desktop plugins must be compiled against Qt 5.4.2 or lower.
#endif

/* Primary namespace */
namespace Geoweb3d
{
/* Primary GUI namespace */
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The primary interface with the GUI (Geoweb3d Desktop) plugin SDK. </summary>
	///
	/// <remarks>	This class is to be returned inside createClassInstance when the guid 
	/// 			GUID_GEOWEB3DDESKTOP_SDK is requested. This interface is implemented by the plugin. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUISDK : public IGW3DIBaseObject
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the final construct action. </summary>
		///
		/// <remarks>	OnFinalConstruct is called once, on application startup.
		/// 			
		/// 			This function provides the final construct context, which allows the plugin to 
		/// 			finish construction of the object, including registering any event listeners.
		/// 			</remarks>
		///
		/// <param name="ctx">	the context. </param> 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnFinalConstruct( GUI::IGW3DGUIFinalConstructContext *ctx ) = 0 ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the start action. </summary>
		///
		/// <remarks>	OnStart is called once, on application startup (after OnFinalConstruct).
		/// 			
		/// 			This function provides the start context, which allows the plugin to create
		///				its user interface components and configure the application.  The developer
		///				may also create/open layers and representations at this point. </remarks>
		///
		/// <param name="ctx">	the context. </param> 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnStart( GUI::IGW3DGUIStartContext *ctx ) = 0 ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the pre draw action. </summary>
		///
		/// <remarks>	OnApplicationUpdate is called every time the main window of the application draws.
		///				Therefore, 
		/// 			 </remarks>
		///
		/// <param name="ctx">	the context. </param> 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnApplicationUpdate( GUI::IGW3DGUIApplicationUpdateContext *ctx ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the pre draw action. </summary>
		///
		/// <remarks>	OnPreDraw is called just before any 3D window draws.
		/// 			Note that this function will not cycle when a 3D window is not active,
		///				or when the active 3D window is not drawing for any reason.	Any changes 
		///				to data and/or representations that are to be visible in the next frame 
		///				should be submitted here at the latest.
		/// 			 </remarks>
		///
		/// <param name="ctx">	the context. </param> 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPreDraw( GUI::IGW3DGUIPreDrawContext *ctx ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	OnPostDraw is called just after any 3D window draws. </summary>
		///
		/// <remarks>	Note that this function will not cycle when a 3D window is not active, or when
		/// 			the active 3D window is not drawing  for any reason. Information about the last
		/// 			drawn frame is available from here, including what the location of the Eye was, 
		/// 			and what the user last clicked on. </remarks>
		///
		/// <param name="ctx">	the context. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPostDraw(  GUI::IGW3DGUIPostDrawContext *ctx ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	OnXMLRead  </summary>
		///
		/// <param name="ctx"> the context. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnXMLRead ( Geoweb3d::GUI::IGW3DGUIXMLReadContext *ctx ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> OnXMLWrite  </summary>
		///
		/// <param name="ctx"> the context. </param> 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnXMLWrite ( Geoweb3d::GUI::IGW3DGUIXMLWriteContext *ctx ) = 0;


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	OnWinEvent is called for each event in the 3D window  </summary>
		///
		/// <remarks>	This function is called for each mouse and keyboard event for each 3D window.
		///				To stop the event being handled by Geoweb3d, return false and set result to the value
		///				that the window procedure should return. If you return true, this native event 
		///				is passed back to Geoweb3d and will propagate to other handlers.
		/// 			 </remarks>
		///
		/// <param name="message">	Native Windows event. </param>
		/// <param name="result">	Value that the window procedure should return. </param>
		/// 
		/// <returns>	Return false to stop the event from being handled by Geoweb3d. Return true to
		///				have this native event passed back to Geoweb3d. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnWinEvent (MSG* message, long* result) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the stop action. </summary>
		///
		/// <remarks>	OnStop is called once, on application shutdown.
		/// 			This context is primarily available to allow the plugin to destroy its objects
		///				and perform shutdown operations.</remarks>
		///
		/// <param name="ctx">	the context. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnStop( GUI::IGW3DGUIStopContext *ctx ) = 0 ;

	};
	
}
}