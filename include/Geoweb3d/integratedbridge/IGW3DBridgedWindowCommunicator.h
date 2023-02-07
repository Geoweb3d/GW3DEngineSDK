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
#include <Geoweb3d/core/GW3DInterFace.h>
#include <Geoweb3d/engine/IGW3DWindowCallback.h>
#include <Geoweb3dCore/Geoweb3dTypes.h>

/* Primary namespace */ 
namespace Geoweb3d
{
	struct  GW3D_DLL IGW3DBridgedWindowCommunicator
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	GPU Context created</summary>
		///
		/// <remarks>	The SDK calls the OnCreate callback once the rendering context is created.
		///				This is a place clients can initialize their gpu specific items as well. </remarks>
		/// 
		/// <param name="bridged_window"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnWindowCreate( IGW3DBridgedWindow* bridged_window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="bridged_window"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnWindowDestroy( IGW3DBridgedWindow* bridged_window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="bridged_window"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnWindowResize( IGW3DBridgedWindow* bridged_window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called just before drawing starts</summary>
		///
		/// <remarks>	If a client draws or uses the frame buffer at this point, it will get overwritten
		///             when the sdk starts to draw. </remarks>
		/// 
		/// <param name="bridged_window"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDrawBegin( IGW3DBridgedWindow* bridged_window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called just after all active cameras have been drawn</summary>
		///
		/// <remarks>	Note, for same frame 2D drawing per camera, its suggested to use OnDraw2D </remarks>
		/// 
		/// <param name="bridged_window"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDrawEnd( IGW3DBridgedWindow* bridged_window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called just before the active camera begins drawing</summary>
		///
		/// <remarks>	If a client draws or uses the frame buffer at this point, it will get overwritten
		///             when the sdk starts to draw. </remarks>
		/// 
		/// <param name="bridged_camera"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPreDraw( IGW3DBridgedCamera* bridged_camera ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called just after the active camera have been drawn</summary>
		///
		/// <remarks>	Note, for same frame 2D drawing per camera, its suggested to use OnDraw2D </remarks>
		/// 
		/// <param name="bridged_camera"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPostDraw( IGW3DBridgedCamera* bridged_camera ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Draw 2D Callback</summary>
		///
		/// <remarks>	The SDK calls the 2D drawing mode callback during the draw call.  Try to be smart
		/// 			about your OpenGL states.  At the point of calling OnDraw2D, the SDK finished
		/// 			drawing everything, so you can stream out things like what is in the field of view
		/// 			so you know where to place icons etc.
		///
		/// 			Note, if you have multiple cameras on a window, 'camera' will be the camera that
		///				has just finished sending the OpenGL commands to the GPU. </remarks>
		///
		/// <param name="bridged_camera"> 	The camera. </param>
		/// <param name="drawcontext"> TODO </param>
		///
		/// <see cref="IGW3DGeoweb3dSDK::draw"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDraw2D( IGW3DBridgedCamera* bridged_camera, IGW3D2DDrawContext *drawcontext ) = 0;

		/// <summary>	Process low-level event </summary>
		///
		/// <remarks>	Return 0 if you handle the message and don't want it to be sent to another handler
		/// 			downstream (the event will not be passed on to other windows).  This includes
		/// 			ProcessEvent( const WindowEvent win_event ).  This is the raw message from the
		/// 			Operating System. </remarks>
		///
		/// <param name="bridged_window"> 	The Window. </param>
		/// <param name="hWndParent">	The window parent. </param>
		/// <param name="message">   	The message. </param>
		/// <param name="wParam">	 	The wParam field of the message. </param>
		/// <param name="lParam">	 	The lParam field of the message. </param>
		///
		/// <returns>	0 to stop event from being propagated. 1 to allow events to continue propagating. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int ProcessLowLevelEvent( IGW3DBridgedWindow* bridged_window, OSWinHandle hWndParent, int64_t message, int64_t wParam, int64_t lParam ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A high-level option for processing window events </summary>
		///
		/// <remarks>	You can use this instead of ProcessLowLevelEvent if it supports all the events
		/// 			you need.  Note, if we created the 3D window off your window, you can still use
		/// 			the events off your own GUI framework. </remarks>
		///
		/// <param name="bridged_window"> 	The Window. </param>
		/// <param name="win_event">	The window event. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void ProcessEvent( IGW3DBridgedWindow* bridged_window, const WindowEvent& win_event ) = 0;
	};
}
