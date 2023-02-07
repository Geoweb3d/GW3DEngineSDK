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

/* Primary namespace */
namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of frames on a shelf at the top of the Geoweb3d user interface. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIFrameCollection : public IGW3DCollection< IGW3DGUIFrameWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a frame by its unique ID. </summary>
		///
		/// <param name="shelfid">	The shelf id. </param>
		///
		/// <returns>	The frame (expired pointer if failed). </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIFrameWPtr get_ById ( const char* frameid ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates this a frame. </summary>
		///
		/// <remarks>	Note that this API will be moved to IGW3DGUIShelfModifiableCollection and be
		/// 			renamed 'create_Frame'!!! </remarks>
		///
		/// <param name="frametitle">  	The frame title. </param>
		/// <param name="frameid">	   	The frame id. </param>
		/// <param name="iconfullpath">	The icon full path. </param>
		/// <param name="width">	   	The width. </param>
		///
		/// <returns>	The newly created frame. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIFrameWPtr create( const char* frametitle, const char* frameid, const char* iconfullpath, int width ) = 0;
	};

}
}



