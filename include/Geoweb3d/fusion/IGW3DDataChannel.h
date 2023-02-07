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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A data channel callback. </summary>
	///
	/// <remarks>	The data channel callback is interface is implimented by the the SDK developer and 
	/// 			is called whenever messages or state changes are received from a Geoweb3d Fusion 
	/// 			data channel. 
	/// 			</remarks>
	/// 
	/// <see cref="IGW3DDataChannel::put_Observer"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DDataChannelCallback 
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback when the datachannel state changes. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnStateChange() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback when a message is received over the datachannel. </summary>
		///
		/// <param name="buffer">	The buffer. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnMessage(const IGW3DDataBuffer *buffer) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Information about the Geoweb3d Fusion client session. </summary>
	///
	/// <remarks>	Information on the client session, which help identify which client a message
	/// 			is coming from. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DDataChannel
	{
		// Enum of possible states of a data channel
		enum DataState 
		{
			Connecting, // The DataChannel is connecting.
			Open,		// The DataChannel is ready to send data.
			Closing,	// The DataChannel is closing.
			Closed,		// The DataChannel is closed.
			MAXVAL = 0xFFFF	//MAX - Do Not Use
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the current state of the data channel. </summary>
		///
		/// <returns>	The state. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual DataState get_State() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assigns an observer callback. </summary>
		///
		/// <returns>	Assign an object inheriting from IGW3DDataChannelCallback that will receive messages 
		///				sent over the data channel, as well as notifications of data channel state changes. 
		///				</returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Observer(IGW3DDataChannelCallback *pcallback) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Unassigns the observer callback. </summary>
		///
		/// <returns>	Unassign the observer, so that its functions are no longer called by the SDK. 
		///				</returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void remove_Observer() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sends data over the data channel. </summary>
		///
		/// <returns>	Sends a buffer of data through the data channel. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult send_Data( const Geoweb3d::IGW3DDataBufferPtr  db ) = 0;


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the label for the data channel. </summary>
		///
		/// <returns>	The label attribute represents a label that can be used to distinguish this
		///				IGW3DDataChannel object from other IGW3DDataChannel objects. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Label() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets peer identifier for the Geoweb3d Fusion client. </summary>
		///
		/// <returns>	The peer identifier. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int get_PeerID() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the processID/application this peer is connected to. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int get_ConnectedProcessID() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the friendly name of the Geoweb3d Fusion client. </summary>
		///
		/// <remarks>	The friendly name is supplied by the client. </remarks>
		///
		/// <returns>	the friendly name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char *get_PeerFriendlyName() const = 0;
	};
};
