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
#include <Geoweb3d/engine/IGW3DVectorRepresentation.h>
#include <Geoweb3d/engine/IGW3DVectorLayer.h>

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> IGW3DSceneLoadingCallbacks enumeration. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DSceneLoadingCallbacks
	{
		virtual ~IGW3DSceneLoadingCallbacks() {};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Information about the current representation that has just taken place. </summary>
		///
		/// <remarks> If you need to keep track of the vector and/or layer of what the Scene is currently
		///           working on. </remarks>
		///
		/// <param name="layer">  The layer  </param>
		/// <param name="rep"> The representation  </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void represented_VectorLayer(IGW3DVectorLayerWPtr layer, IGW3DVectorRepresentationWPtr rep) = 0;
	};

}