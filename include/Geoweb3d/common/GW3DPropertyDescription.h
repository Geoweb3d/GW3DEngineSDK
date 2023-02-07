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
#include <Geoweb3dCore/Geoweb3dTypes.h>

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Description of a property. </summary>
	///
	/// <remarks>	The full description of a property or attribute field. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL GW3DPropertyDescription
	{
		virtual ~GW3DPropertyDescription() {}

		GW3DPropertyDescription()
			: property_name(nullptr)
			, property_description(nullptr)
			, property_type(UNKNOWN)
			, property_length(-1) { }

		const char* property_name; /* Null terminated name of the property or 0 */
		const char* property_description; /* Null terminated description of the property or 0 */
		PropertyType property_type; /* Type of the property */
		int property_length; /* Length of the property.  This is currently only used for fixed length strings */
	};
}