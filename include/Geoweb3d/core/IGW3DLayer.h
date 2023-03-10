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
#include "IGW3DClientUserData.h"

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Layer base class. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DLayer : public IGW3DClientUserData
	{
		virtual ~IGW3DLayer() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type of the layer. </summary>
		///
		/// <returns>	the type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Type() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the layer. </summary>
		///
		/// <returns>	the name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Name() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the spatial envelope of the layer. </summary>
		///
		/// <remarks>	The envelope is provided in the WGS84 geographic coordinate system. </remarks>
		///
		/// <returns>	The envelope. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DEnvelope get_Envelope() const = 0;
	};
}