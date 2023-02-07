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
#include "Geoweb3d/core/IGW3DCollection.h"

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of camera descriptions in shared memory, exposed to clients via
	/// 			Geoweb3d Fusion. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DCameraDescriptionCollection : public IGW3DCollection< IGW3DCameraDescription* >
	{	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Retrieve the item at the given index. </summary>
		///
		/// <param name="index">	Zero-based index of the item. </param>
		///
		/// <returns>	The item. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DCameraDescription* operator[](  unsigned long index ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Retrieve the item at the given index. </summary>
		///
		/// <param name="index">	Zero-based index of the item. </param>
		///
		/// <returns>	The item. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DCameraDescription* get_AtIndex( unsigned long index ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Refresh the camera collection </summary>
		///
		/// <remarks>	Refreshes the camera descriptions.  This is called within the SDK when 
		/// 			get_CameraDescriptionCollection is called.  The collection represents a snapshot in
		/// 			time.  If you keep this outside (not recommended) you will probably want to call 
		/// 			refresh to ensure you are looking at the latest snapshot. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void refresh() = 0;
	};

}

