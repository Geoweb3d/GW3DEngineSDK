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
	/// <summary>	Xml read context </summary>
	///
	/// <remarks>	The xml read context is provided to the plugin for reading of project, layer, 
	//              and representation files </remarks>
	/// 
	/// <see cref="OnXMLRead"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIXMLReadContext
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Query the type of file for the XML read </summary>
		///
		/// <remarks> Returns which file type (Project, Layer, or Representation) 
		/// 		  is open for reading </remarks>
		///
		/// <returns> An enumeration of IGW3DGeoweb3dFileType indicating which file is being read</returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3dFileType get_FileType () = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Returns the XML buffer </summary>
		///
		/// <remarks> Returns a pointer to the character buffer containing the plugin custom XML.
		/// 		  This character buffer is only valid until the call to
		/// 		  OnXMLRead ( Geoweb3d::GUI::IGW3DGUIXMLReadContext *ctx ) returns.
		/// 		  Do not save this pointer for future access.  
		/// 		  If the data is needed after the call, copy the character buffer. </remarks>
		///
		/// <returns> The pointer to the character buffer if valid, otherwise null.</returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_XML () = 0;

	};
}
}



