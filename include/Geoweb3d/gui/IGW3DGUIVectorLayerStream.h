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
#include <Geoweb3d/engine/IGW3DVectorLayerStreamFilter.h>

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	An iterator for querying and modifying features of a layer. </summary>
	///
	/// <remarks>	The plugin implements this layer stream and receives OnStream callbacks for each
	/// 			feature. The result provided for each feature provides access to methods for 
	/// 			viewing and modifying the feature's attributes and geometry. </remarks>
	/// 
	/// <see cref="IGW3DGUIVectorLayer::Stream"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorLayerStream : public IGW3DVectorLayerStreamFilter
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback with results for one feature. </summary>
		///
		/// <remarks>	The SDK will iterate through this stream and call OnStream with results 
		/// 			for each feature. This function is implemented by the plugin.</remarks>
		///
		/// <param name="result">	The result for the feature, through which the geometry and attribute 
		/// 						data may be viewed, as well as modified if supported.</param>
		///
		/// <returns>	return true to continue streaming the next feature.  Return false to abort
		/// 			the stream, skipping all remaining features. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool OnStream( IGW3DGUIVectorLayerStreamResult* result ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Will only be called if an error occurs while streaming. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnError(/*todo*/ ) = 0;
	};


}
}