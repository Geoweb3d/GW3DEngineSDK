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
	/// <summary>	Attribute Map. </summary>
	///
	/// <remarks>	An attribute map is a 'rule' for the representation of a given set of vector 
	/// 			features, specifying what qualifies a feature for inclusion (a set of discrete 
	/// 			features, or all features with a given attribute value, for example), as well
	/// 			as what aspects of the representation should be changes (color, height, etc). </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIAttributeMap 
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Values that represent the attribute map type. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum AttributeMapType
		{
			/// <summary>	An enum constant representing an undefined attribute map. </summary>
			TypeOther = 0,
			/// <summary>	An enum constant representing the root attribute map. </summary>
			TypeRoot,
			/// <summary>	An enum constant representing the all features type. </summary>
			TypeAllFeatures,
			/// <summary>	An enum constant representing the category group type. </summary>
			TypeCategoryGroup,
			/// <summary>	An enum constant representing the category type. </summary>
			TypeCategory,
			/// <summary>	An enum constant representing the range group type. </summary>
			TypeRangeGroup,
			/// <summary>	An enum constant representing the range type. </summary>
			TypeRange,
			/// <summary>	An enum constant representing the one to one type. </summary>
			TypeOneToOne,
			/// <summary>	An enum constant representing the selection set type. </summary>
			TypeSelectionSet,
			/// <summary>	Do Not Use. </summary>
			AM_TYPE_MAX = 0xff
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the type of the attribute map. </summary>
		///
		/// <returns>	The type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual AttributeMapType get_Type() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enable/Disable the attribute map. </summary>
		///
		/// <remarks>	When disabled, the rules of an attribute map are not applied to the representation.
		///				Note that the all features attribute map defines the overall defaults for the 
		///				representation and therefore may not be disabled.</remarks>
		///
		/// <param name="enabled">	true to enable, false to disable. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Enabled(bool enabled) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the enabled state of the attribute map. </summary>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Enabled() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Expand/collapse the children of an attribute map in the Attribute Mapping Table
		/// 			of Contents. </summary>
		///
		/// <param name="expanded">	true to expand, false to collapse. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Expanded(bool expanded) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the expanded state of the attribute map. </summary>
		///
		/// <returns>	true if expanded, false if collapsed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_Expanded() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the attribute map collection containing all children of the attribute map. 
		/// 			</summary>
		///
		/// <returns>	the attribute map collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIAttributeMapCollection* get_AttributeMapCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the all features attribute map. </summary>
		///
		/// <remarks>	If the type is TypeAllFeatures, get the all features attribute map.  Note that
		/// 			the all features attribute map defines the overall defaults for the representation
		/// 			and therefore may not be disabled. </remarks>
		///
		/// <returns>	the all features attribute map interface, or an expired pointer if type does not
		/// 			match. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual IGW3DGUIAllFeaturesAttributeMapWPtr get_AsAllFeatures() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the category group attribute map. </summary>
		///
		/// <remarks>	If the type is TypeCategoryGroup, get the category group attribute map. </remarks>
		///
		/// <returns>	the category group attribute map interface, or an expired pointer if type does not
		/// 			match. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUICategoryGroupAttributeMapWPtr get_AsCategoryGroup() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the category attribute map. </summary>
		///
		/// <remarks>	If the type is TypeCategory, get the category attribute map.  </remarks>
		///
		/// <returns>	the category attribute map interface, or an expired pointer if type does not
		/// 			match. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUICategoryAttributeMapWPtr get_AsCategory() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the one to one attribute map. </summary>
		///
		/// <remarks>	If the type is TypeOneToOne, get the one to one attribute map.</remarks>
		///
		/// <returns>	the one to one attribute map interface, or an expired pointer if type does not
		/// 			match. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIOneToOneAttributeMapWPtr get_AsOneToOne() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the selection set attribute map. </summary>
		///
		/// <remarks>	If the type is TypeSelectionSet, get the selection set attribute map.  </remarks>
		///
		/// <returns>	the selection set attribute map interface, or an expired pointer if type does not
		/// 			match. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUISelectionSetAttributeMapWPtr get_AsSelectionSet() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a representation property's value. </summary>
		///
		/// <remarks>	Gets the value of a representation property for this attribute map, in the native 
		/// 			unit of Geoweb3d (meters) when applicable. </remarks>
		///
		/// <param name="index">	Zero-based index of the representation property 
		/// 						(see IGW3DGUIVectorRepresentationProperties.h) </param>
		///
		/// <returns>	The value of the representation property, or a null IGW3DVariant if property is not
		///				set, or is out of range.  </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const Geoweb3d::IGW3DVariant& get_Property (int index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets all representation property values. </summary>
		///
		/// <remarks>	Gets the values of a representation properties for this attribute map, in the native 
		/// 			unit of Geoweb3d (meters) when applicable. </remarks>
		///
		/// <returns>	The representation property values.  </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const Geoweb3d::IGW3DPropertyCollection* get_PropertyCollection() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a representation property's value. </summary>
		///
		/// <remarks>	Sets the value of a representation property for this attribute map, in the native 
		/// 			unit of Geoweb3d (meters) when applicable. </remarks>
		///
		/// <param name="index">	Zero-based index of the representation property 
		/// 						(see IGW3DGUIVectorRepresentationProperties.h) </param>
		/// 
		/// <param name="value">	The new value of the representation property. </param>
		///
		/// <returns>	GW3D_sOk if successful, or GW3D_eInvalidArgument if invalid property index is given. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult put_Property (int index, const Geoweb3d::IGW3DVariant& value) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the representation property values. </summary>
		///
		/// <remarks>	Sets the representation property values for this attribute map, in the native 
		/// 			unit of Geoweb3d (meters) when applicable. </remarks>
		/// 
		/// <param name="value">	The new values of the representation properties. Null values will not 
		///							be evaluated in cascading attribute mapping (i.e the values are 
		///							'Inherited' as displayed in the user interface).</param>
		///
		/// <returns>	GW3D_sOk if successful. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual	Geoweb3d::GW3DResult put_PropertyCollection(const Geoweb3d::IGW3DPropertyCollection * ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the representation property values. </summary>
		///
		/// <remarks>	Sets the representation property values for this attribute map, in the native 
		/// 			unit of Geoweb3d (meters) when applicable. </remarks>
		/// 
		/// <param name="value">	The new values of the representation properties. Null values will not 
		///							be evaluated in cascading attribute mapping (i.e the values are 
		///							'Inherited' as displayed in the user interface).</param>
		///
		/// <returns>	GW3D_sOk if successful. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual	Geoweb3d::GW3DResult put_PropertyCollection(const Geoweb3d::IGW3DPropertyCollectionPtr ) = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	An all features attribute map. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIAllFeaturesAttributeMap
	{
		virtual IGW3DGUIAttributeMap::AttributeMapType get_Type() = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A category group attribute map. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUICategoryGroupAttributeMap
	{
		virtual IGW3DGUIAttributeMap::AttributeMapType get_Type() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The field name that defines all categories under this category group. </summary>
		///
		/// <returns>	the field name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_FieldName() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a category. </summary>
		///
		/// <param name="field_value">	The field value that defines the category. </param>
		///
		/// <returns>	The new category, or an existing category if duplicate. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIAttributeMapWPtr create_Category( const IGW3DVariant& field_value ) = 0;  

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A category attribute map. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUICategoryAttributeMap
	{

		virtual IGW3DGUIAttributeMap::AttributeMapType get_Type() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the field name that defines the category / category group. </summary>
		///
		/// <returns>	the field name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_FieldName() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets field value that defines the category. </summary>
		///
		/// <returns>	the field value. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DVariant& get_FieldValue( ) const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A one to one attribute map. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIOneToOneAttributeMap
	{
		virtual IGW3DGUIAttributeMap::AttributeMapType get_Type() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets field name that will define the value for the representation property. 
		/// 			</summary>
		///
		/// <returns>	the field name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_FieldName() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the property index for the property that will be set to the value in the given
		/// 			field value. [not yet implemented]</summary>
		///
		/// <returns>	The property index. </returns>
		/// 
		/// <see cref="IGW3DGUIVectorRepresentationProperties.h"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual int get_PropertyIndex() const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A selection set attribute map. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUISelectionSetAttributeMap
	{
		virtual IGW3DGUIAttributeMap::AttributeMapType get_Type() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets fid collection. </summary>
		///
		/// <returns>	the fid collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIFidCollection* get_FidCollection() = 0;

	};

}
}
