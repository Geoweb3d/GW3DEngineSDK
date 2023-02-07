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
#include <Geoweb3dCore/GW3DResults.h>

namespace Geoweb3d
{
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A representation driver. </summary>
	///
	/// <remarks>	The representation driver, which provides access to viewing and creating 
	/// 			representations of its type. </remarks>
	/// 
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorRepresentationDriver : public IGW3DDriver
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name. </summary>
		///
		/// <remarks>	Gets the name of this driver. </remarks>
		///
		/// <returns>	the driver name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char *  get_Name() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets capability to represent. </summary>
		///
		/// <remarks>	Check if a layer may be represented using this driver. 
		/// 			This checks if the layer is of a supported geometry type for the representation,
		/// 			and optionally checks if a basic vector representation may be created.</remarks>
		///
		/// <param name="layer">	The layer. </param>
		/// <param name="basic">	(optional) Set to true to determine if a basic representation is
		/// 						supported. </param>
		///
		/// <returns>	GW3D_sOk if the layer can be represented </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult get_CapabilityToRepresent( IGW3DGUIVectorLayerWPtr layer )=0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the global defaults properties for representations using this driver. </summary>
		///
		/// <returns>	the default properties. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DPropertyCollection * get_PropertyCollection(  ) const = 0; 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check if the representation is a 2D (screenspace) representation. </summary>
		///
		/// <returns>	true if the representation is 2D, false if it is 3D. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Is2d(  ) const = 0; 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets representation layer collection. </summary>
		///
		/// <remarks>	Gets the collection of all representations that have been created or
		/// 			opened with this driver.  Representations are created and removed through this 
		/// 			collection.  Representations that are added to the Geoweb3d Desktop	table of 
		/// 			contents (i.e. opened or created with the addToToc argument set to true)
		/// 			are owned by Geoweb3d and are accessible through the table of contents interface.
		/// 			</remarks>
		///
		/// <returns>	the representation layer collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationCollection *get_RepresentationLayerCollection( ) = 0;
	};

}
}