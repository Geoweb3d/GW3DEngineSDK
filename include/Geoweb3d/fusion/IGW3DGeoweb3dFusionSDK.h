//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// GeoWeb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Geoweb3d/core/GW3DInterFace.h>
#include "Geoweb3d/core/GW3DGuids.h"
#include <Geoweb3dCore/Geoweb3dEngine.h>

/* Primary namespace */
namespace Geoweb3d
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	The fusion SDK callback. </summary>
///
/// <remarks>	This callback reports when data channels are added or removed from the fusion SDK. 
/// 			</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct  GW3D_DLL IGW3DWFusionSDKCallback : public IGW3DCallback
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the data channel added action. </summary>
	///
	/// <remarks>	This function is called by the Fusion SDK when a data channel is added. </remarks>
	///
	/// <param name="datachannel">	The datachannel. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void OnDataChannelAdded( IGW3DDataChannelPtr datachannel ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the data channel removed action. </summary>
	///
	/// <remarks>	This function is called by the Fusion SDK when a data channel is removed. </remarks>
	///
	/// <param name="datachannel">	The datachannel. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void OnDataChannelRemoved( IGW3DDataChannelPtr datachannel ) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	An interface for communicating with remote network clients and services. </summary>
///
/// <remarks>	The Geoweb3d Fusion SDK allows the application to receive network messages and
/// 			send network responses to a remote client application or web service.  This
/// 			API becomes active when Geoweb3dFusion.exe is running and receives a connection
/// 			from a remote client. 
/// 			
/// 			The Geoweb3d Fusion technology is still under early development and is subject to 
/// 			significant change. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct  GW3D_DLL IGW3DGeoweb3dFusionSDK : public IGW3DIBaseObject
{

//SKIP-CODE-BEGIN
// 

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Request the description of all cameras currently exposed to clients of 
	/// 			Geoweb3dFusion.exe. </summary>
	///
	/// <remarks>	Provides a snapshot of all cameras exposed to clients of Geoweb3dFusion.exe,
	/// 			including their camera IDs and friendly names.  Geoweb3dFusion.exe is a stand-alone
	/// 			process and the cameras exist in shared memory.  Therefore it can maintain 
	/// 			connections to clients even if the application using the Geoweb3d Engine SDK is 
	/// 			closed or crashes. </remarks>
	///
	/// <returns>	The camera description collection. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
//	virtual IGW3DCameraDescriptionCollection *get_CameraDescriptionCollection() = 0;

//SKIP-CODE-END

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the next message. </summary>
	///
	/// <remarks>	Gets the next message sent by a Geoweb3dFusion client. </remarks>
	///
	/// <param name="timeout">	(Optional) the timeout. </param>
	///
	/// <returns>	null if it fails, else the message. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult process_Callbacks( int timeout = -1 ) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Creates the Geoweb3d Fusion SDK interface. </summary>
    ///
    /// <remarks>	Creates an instance of the Geoweb3d Fusion SDK. </remarks>
    ///
    /// <returns>	The new interface. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static IGW3DGeoweb3dFusionSDKPtr CreateInterface( IGW3DWFusionSDKCallback *pcallback)
    {
        return CreateInterfacePtr< IGW3DGeoweb3dFusionSDKPtr, IGW3DGeoweb3dFusionSDK>( GUID_GEOWEB3D_FUSIONSERVER_SDK, pcallback );
    }
};

}