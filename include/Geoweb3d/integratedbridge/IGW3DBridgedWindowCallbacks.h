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
#include <Geoweb3d/engine/GW3DExports.h>

/* Primary namespace */
namespace Geoweb3d
{
	struct  GW3D_DLL IGW3DWindowAccessor
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	const window accessor. </summary>
		///
		/// <returns>	the window </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DWindow* get_IGW3DWindow() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Create a camera on the current window. </summary>
		///
		/// <param name="camera_title">	 The name to give the camera </param>
		///
		/// <returns>	the camera </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DCameraWPtr create_Camera( const char* camera_title ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Closes the given camera. </summary>
		///
		/// <remarks>	Closes the given camera and removes it from the collection.
		///
		/// 			To close a camera: IGW3DCameraCollection::close( window );
		///
		///				IGW3DCameraWPtr will become expired.</remarks>
		///
		/// <param name="camera">	[in] The camera. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close( IGW3DCameraWPtr &camera  )
		{
			close_Camera( camera );
		}
	};

	struct  GW3D_DLL IGW3DCameraAccessor
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	const camera accessor. </summary>
		///
		/// <returns>	the camera </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DCamera* get_IGW3DCamera() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	query to know whether or not the camera can be modified. </summary>
		///
		/// <param name="requestor">	A friendly name should be passed in, representing the requestor </param>
		/// <param name="camera">	The camera, if GW3DResult is a success code, otherwise it will be null </param>
		///
		/// <returns>	GW3DResult </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult get_IGW3DCameraForUpdate( const char* requestor, IGW3DCamera*& camera ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	query for the reason why the camera currently cannot be modified.  </summary>
		///
		/// <returns>	The reason for get_IGW3DCameraForUpdate not returning a success code </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Reason() const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The client inherits from this object and implements the given interface. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DBridgedWindowCallbacks : public IGW3DCallback
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	GPU Context created. </summary>
		///
		/// <remarks>	The SDK calls the OnCreate callback once the rendering context is created.
		///				This is a place clients can initialize their gpu specific items as well. </remarks>
		///
		/// <param name="window">	The window. </param>
		///
		/// <see cref="IGW3DWindowAccessor"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnCreate( IGW3DWindowAccessor& window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	GPU Context destroyed. </summary>
		///
		/// <remarks>	The SDK calls the OnDestroy callback once the rendering context is about to be destroyed.
		///				This is a place clients can release their gpu specific items as well. </remarks>
		///
		/// <param name="window">	The window. </param>
		///
		/// <see cref="IGW3DWindowAccessor"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDestroy( const IGW3DWindowAccessor& window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The window was resized. </summary>
		///
		/// <param name="window">	The window. </param>
		///
		/// <see cref="IGW3DWindowAccessor"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnResize( const IGW3DWindowAccessor& window ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called at the beginning of each frame before any active cameras have been drawn. </summary>
		///
		/// <remarks>	This will be called for each active camera created by the bridged application. This 
		///				typically where any per-frame updates need to be made. </remarks>
		///
		/// <param name="camera">	The camera. </param>
		///
		/// <see cref="IGW3DCameraAccessor"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPreDraw( const IGW3DCameraAccessor& camera ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called just before drawing starts. </summary>
		///
		/// <remarks>	If a client draws or uses the frame buffer at this point, it will get overwritten
		///             when the sdk starts to draw.</remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDrawBegin() = 0;

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
		/// <param name="camera">	The camera. </param>
		/// <param name="drawcontext">	The 2d drawing context. </param>
		///
		/// <see cref="IGW3DGeoweb3dSDK::draw"/>
		/// <see cref="IGW3DCameraAccessor"/>
		/// <see cref="IGW3D2DDrawContext"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDraw2D( const IGW3DCameraAccessor& camera, IGW3D2DDrawContext *drawcontext ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called just after all active cameras have been drawn </summary>
		///
		/// <remarks>	Note, for same frame 2D drawing per camera, its suggested to use OnDraw2D </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDrawEnd() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Called at the end of each frame, after all active cameras have been drawn </summary>
		///
		/// <remarks>	This will be called for each active camera created by the bridged application. This 
		//				typically where any per-frame processing need to be made. Such as performing any
		//				IGW3DFrustumAnalysisStream </remarks>
		///
		/// <param name="camera">	The camera. </param>
		///
		/// <see cref="IGW3DCameraAccessor"/>
		/// <see cref="IGW3DFrustumAnalysisStream"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnPostDraw( const IGW3DCameraAccessor& camera ) = 0;
	};
}
