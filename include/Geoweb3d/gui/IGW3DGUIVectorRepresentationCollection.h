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

namespace Geoweb3d
{

namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	The collection containing all representations that exist for the driver. </summary>
	/// 
	/// <see cref="IGW3DGUIVectorRepresentation::get_PagingLevel"/>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorRepresentationCollection : public IGW3DCollection< IGW3DGUIVectorRepresentationWPtr >
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Create a representation using this driver, representing the given layer. </summary>
		///
		/// <param name="layer">	The layer to be represented in the 3D scene. </param>
		///
		/// <returns>	A vector representation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationWPtr create( IGW3DGUIVectorLayerWPtr layer ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Create a basic representation using this driver, representing the given layer. </summary>
		///
		/// <remarks>	Creates a basic vector representation.
		/// 			Basic representations are only supported on layers that were opened or created
		/// 			with the addToToc argument set to false. </remarks>
		///
		/// <param name="layer">	The layer to be represented in the 3D scene. </param>
		///
		/// <returns>	A vector representation, or if given an unsupported layer, an expired weak pointer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationBasicWPtr create_basic( IGW3DGUIVectorLayerWPtr layer ) = 0;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Closes the given representation. </summary>
		///
		/// <remarks>	Closes the given representation and removes it from the collection. 
		///
		/// 			To close a representation: IGW3DGUIVectorRepresentationCollection::close( rep );
		///
		///				IGW3DGUIVectorRepresentationWPtr will become expired.</remarks>
		///
		/// <param name="rep">	[in] The representation. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close( IGW3DGUIVectorRepresentationWPtr &rep  )
		{
			close_GUIVectorRepresentation( rep );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Closes the given representation. </summary>
		///
		/// <remarks>	Closes the given representation and removes it from the collection. 
		///
		/// 			To close a representation: IGW3DGUIVectorRepresentationCollection::close( rep );
		///
		///				IGW3DGUIVectorRepresentationWPtr will become expired.</remarks>
		///
		/// <param name="rep">	[in] The representation. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		static void close( IGW3DGUIVectorRepresentationBasicWPtr &rep  )
		{
			close_GUIVectorRepresentationBasic( rep );
		}
	};


}
}

