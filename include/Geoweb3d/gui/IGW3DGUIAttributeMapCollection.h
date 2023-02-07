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
/// <summary>	Collection of attribute maps. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DGUIAttributeMapCollection : public IGW3DCollection< IGW3DGUIAttributeMapWPtr >
{

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	The root collection of attribute maps for a representation. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DGUIAttributeMapRootCollection : public IGW3DGUIAttributeMapCollection
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a selection set. </summary>
	///
	/// <returns>	The new selection set. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual IGW3DGUIAttributeMapWPtr create_SelectionSet ( ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a category group attribute map. </summary>
	///
	///	<remarks>	By itself, a category group attribute map does nothing except act as a container 
	///				for all categories based on the same field name.  Disabling a category attribute 
	///				map causes the effects of all its categories to be ignored.
	///
	/// <param name="field_name">	Name of the field that defines categories in the group. </param>
	///
	/// <returns>	The new category group. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual IGW3DGUIAttributeMapWPtr create_CategoryGroup ( const char* field_name ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a category attribute map. </summary>
	///
	/// <remarks>	Note that this will implicitly create a category group if none exists with the given 
	/// 			field name. </remarks>
	///
	/// <param name="field_name"> 	Name of the field. </param>
	/// <param name="field_value">	The field value. </param>
	///
	/// <returns>	The new category. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual IGW3DGUIAttributeMapWPtr create_Category ( const char* field_name, const IGW3DVariant& field_value ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates a one to one attribute map. </summary>
	///
	/// <param name="field_name">	 	Name of the field. </param>
	/// <param name="property_index">	Zero-based index of the property. </param>
	///
	/// <returns>	The new one to one. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual IGW3DGUIAttributeMapWPtr create_OneToOne ( const char* field_name, int property_index ) = 0; 

};

}
}
