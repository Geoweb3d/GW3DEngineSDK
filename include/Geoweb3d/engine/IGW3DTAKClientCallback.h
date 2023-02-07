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
#include <Geoweb3dCore/Geoweb3dExports.h>
#include <Geoweb3dCore/GW3DResults.h>
#include <Geoweb3d/core/IGW3DCollection.h>

struct IGW3DVectorLayer;

namespace Geoweb3d
{
	class IGW3DTAKClientContext;

	class GW3D_DLL IGW3DTAKClientCallback
	{
	public:
		virtual ~IGW3DTAKClientCallback() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever a TAK contact is added or removed from the server. </summary>
		///
		/// <param name="context"> TODO </param>
		/// <param name="modified_features"> TODO </param>
		/// <param name="layer"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onLayerContactChanged( const IGW3DTAKClientContext* context, IGW3DCollection<long>& modified_features, IGW3DVectorLayer* layer) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever a TAK Cursor on Target(CoT) chat message is received. </summary>
		///
		/// <param name="context"> TODO </param>
		/// <param name="sender_callsign"> TODO </param>
		/// <param name="sender_uid"> TODO </param>
		/// <param name="message"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onChatMessageReceived(const IGW3DTAKClientContext* context, const char* sender_callsign, const char* sender_uid, const char* message) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever a file transfer to the client is initiated. </summary>
		///
		/// <remarks> Implementer should return true to confirm/accept the transfer, or false to cancel the transfer. </remarks>
		///
		/// <param name="context"> TODO </param>
		/// <param name="sender_callsign"> TODO </param>
		/// <param name="local_file_path"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool onFileTransferRequested(const IGW3DTAKClientContext* context, const char* sender_callsign, const char* local_file_path ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever an accepted file transfer is completed. That is, fully downloaded. </summary>
		///
		/// <remarks> local_file_path is the absolute path to the received mission package on the local file system. </remarks>
		///
		/// <param name="context"> TODO </param>
		/// <param name="local_file_path"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onFileTransferCompleted(const IGW3DTAKClientContext* context, const char* local_file_path) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever an accepted file transfer( download) fails. </summary>
		///
		/// <remarks> local_file_path is the absolute path to the requested downloaded mission package. </remarks>
		///
		/// <param name="context"> TODO </param>
		/// <param name="local_file_path"> TODO </param>
		/// <param name="reason"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onFileTransferFailed(const IGW3DTAKClientContext* context, const char* local_file_path, const GW3DResult reason ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever a file upload is successfully completed. </summary>
		///
		/// <remarks> recipient_uid is the uid of the recipient of the file upload. </remarks>
		///
		/// <param name="context"> TODO </param>
		/// <param name="recipient_uid"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onFileTransferSendCompleted(const IGW3DTAKClientContext* context, const char* recipient_uid ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever a file upload is fails. </summary>
		///
		/// <remarks> recipient_uid is the uid of the recipient of the fail file upload. </remarks>
		///
		/// <param name="context"> TODO </param>
		/// <param name="recipient_uid"> TODO </param>
		/// <param name="reason"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onFileTransferSendFailed(const IGW3DTAKClientContext* context, const char* recipient_uid, const GW3DResult reason ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever connection to the server is active. </summary>
		///
		/// <remarks> Communication with the server should only occur when the server connection is active. </remarks>
		///
		/// <param name="context"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onServerConnectionActive(const IGW3DTAKClientContext* context) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever connection to the server is inactive or otherwise disabled. </summary>
		///
		/// <remarks> Communication with the server should only occur when the server connection is active. </remarks>
		///
		/// <param name="context"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onServerConnectionInActive(const IGW3DTAKClientContext* context) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked prior to the Situational Awareness( SA) update message is sent to the server. </summary>
		///
		/// <param name="context"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onSANotificationBegin(const IGW3DTAKClientContext* context) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked after to the Situational Awareness( SA) update message is sent to the server. </summary>
		///
		/// <param name="context"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onSANotificationEnd(const IGW3DTAKClientContext* context) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Invoked whenever an error occur withing the TAK client client context. </summary>
		///
		/// <param name="context"> TODO </param>
		/// <param name="error"> TODO </param>
		/// <param name="error_msg"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void onError(const IGW3DTAKClientContext* context, const GW3DResult error, const char* error_msg ) = 0;
	};
}
