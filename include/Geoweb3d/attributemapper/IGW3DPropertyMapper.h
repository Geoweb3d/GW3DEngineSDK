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
#include <Geoweb3dcore/Geoweb3dTypes.h>

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> UnitOfMeasure enumeration. </summary>
    ///
    /// <remarks> Internal supported units of measure. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    enum UnitOfMeasure : int
    {
        NONE = 0
        , UNIT_METERS
        , UNIT_FEET
        , UNIT_KILOMETERS
        , UNIT_MILES
        , UNIT_CM
        , UNIT_INCHES
    };

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> IGW3DPropertyMapper. </summary>
	///
	/// <remarks> Allows the application to add custom properties to representations. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	class IGW3DPropertyMapper
	{
	public:
	
		virtual ~IGW3DPropertyMapper()
		{}
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds the user defined property for use during attribute mapping </summary>
		///
		/// <remarks> A user can add their own properties to get passed through the system and aid in attribute mapping. 	</remarks>
		/// <param name="property_name"> The name of the property. </param>
		/// <param name="property_alias"> The secondary name useful for mappings and display. </param>
		/// <param name="property_description"> The property description. </param>
		/// <param name="type"> The property type. </param>
		/// <param name="default_value"> The default value of the property. </param>
    	/// <param name="property_has_units"> Currently just a helpful flag that states whether the value is in meters of not. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
	
		virtual void AddSupportedProperty( const char* property_name, const char* property_alias, const char* property_description, Geoweb3d::PropertyType type, const Geoweb3d::IGW3DVariant& default_value, bool property_has_units ) = 0;
	};

}