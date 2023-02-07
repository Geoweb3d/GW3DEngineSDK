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
#include "../core/GW3DInterFace.h"

/* Primary namespace */
namespace Geoweb3d
{

	struct GW3D_DLL IGW3DAnalysis
	{
		virtual ~IGW3DAnalysis() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a camera to the analysis (like viewshed). </summary>
		///
		/// <remarks>	Adds a camera to the analysis, causing visual results to be rendered on this
		///				camera.  No results  will be visible in the 3D scene unless a camera is added.
		///				TODO: this will probably become a collection so that one can query which camer
		///				as were added. </remarks>
		///
		/// <param name="p">	The camera to add. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_AddCamera(IGW3DCameraWPtr camera) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Removes a camera from the viewshed. </summary>
		///
		/// <remarks>	Removes a camera from the viewshed, causing it to no longer be rendered for that
		/// 			camera. </remarks>
		///
		/// <param name="p">	The camera to remove. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_RemoveCamera(IGW3DCameraWPtr camera) = 0;
	};

}