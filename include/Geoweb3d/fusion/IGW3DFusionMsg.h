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

/* Primary namespace */
namespace Geoweb3d
{

//SKIP-CODE-BEGIN

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	///// <summary>	A message from a Geoweb3d Fusion client. </summary>
	/////
	///// <remarks>	A message received from Geoweb3d Fusion contains a data buffer with the raw
	///// 			message (which can be text or binary data) and must be given a response or
	///// 			a 'no response' before the next message is processed. </remarks>
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//struct  GW3D_DLL IGW3DFusionMsg
	//{
	//	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	/// <summary>	Gets client session information. </summary>
	//	///
	//	/// <remarks>	Gets information about the client that sent the message. </remarks>
	//	///
	//	/// <returns>	the client session information. </returns>
	//	////////////////////////////////////////////////////////////////////////////////////////////////////

	//	virtual const IGW3DClientSessionInfo * get_ClientSessionInfo() const = 0;

	//	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	/// <summary>	Gets the data buffer. </summary>
	//	///
	//	/// <remarks>	Gets the raw message sent by the Geoweb3d Fusion client.  This may be text or
	//	/// 			binary data. </remarks>
	//	///
	//	/// <returns>	null if it fails, else the data buffer. </returns>
	//	////////////////////////////////////////////////////////////////////////////////////////////////////

	//	virtual const IGW3DDataBuffer* get_DataBuffer() = 0;

	//	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	/// <summary>	Designates no response on the message. </summary>
	//	///
	//	/// <remarks>	If no response is to be assigned to the message, you must call put_NoResponse to 
	//	/// 			indicate this.  Either put_NoResponse or put_Response must be called on every 
	//	/// 			message, and must be called only once per message.</remarks>
	//	///
	//	/// <param name="process_message">	(Optional) if true (default), the fusion server will process 
	//	/// 								the information in the message from the client. </param>
	//	////////////////////////////////////////////////////////////////////////////////////////////////////

	//	virtual void  put_NoResponse(bool process_message = true ) = 0; 

	//	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	/// <summary>	Assign a response to the message. </summary>
	//	///
	//	/// <remarks>	This response will be sent back to the sender of the message. Either put_NoResponse 
	//	/// 			or put_Response must be called on every	message, and must be called only once per 
	//	/// 			message. </remarks>
	//	///
	//	/// <param name="buffer">		  	The response buffer. </param>
	//	/// <param name="process_message">	(Optional) if true (default), the fusion server will process
	//	/// 								the information in the message from the client. </param>
	//	////////////////////////////////////////////////////////////////////////////////////////////////////

	//	virtual void  put_Response( const IGW3DDataBuffer * buffer, bool process_message = true ) = 0;

	//	////////////////////////////////////////////////////////////////////////////////////////////////////
	//	/// <summary>	Assign a response to the message. </summary>
	//	///
	//	/// <remarks>	This response will be sent back to the sender of the message. Either put_NoResponse
	//	/// 			or put_Response must be called on every	message, and must be called only once per
	//	/// 			message. </remarks>
	//	///
	//	/// <param name="ptr">			  	Pointer to the response buffer. </param>
	//	/// <param name="process_message">	(Optional) if true (default), the fusion server will process
	//	/// 								the information in the message from the client. </param>
	//	////////////////////////////////////////////////////////////////////////////////////////////////////

	//	virtual void  put_Response( const IGW3DDataBufferPtr &ptr, bool process_message = true ) = 0; 
	//};

//SKIP-CODE-END

}