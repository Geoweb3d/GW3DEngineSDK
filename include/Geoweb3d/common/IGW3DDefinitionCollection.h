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
#include <Geoweb3dCore/Geoweb3dTypes.h>
#include "../core/IGW3DConstCollection.h"
#include "GW3DPropertyDescription.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of definitions. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DDefinitionCollection : public IGW3DConstCollection< const GW3DPropertyDescription* >
    {
        virtual ~IGW3DDefinitionCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Equality operator. </summary>
        ///
        /// <param name="s">	The const IGW3DDefinitionCollection * to process. </param>
        ///
        /// <returns>	true if the parameters are considered equivalent. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool operator == (const IGW3DDefinitionCollection* s) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Equality operator. </summary>
        ///
        /// <param name="s">	The const IGW3DDefinitionCollectionPtr to process. </param>
        ///
        /// <returns>	true if the parameters are considered equivalent. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool operator == (const IGW3DDefinitionCollectionPtr  s) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets index by name. </summary>
        ///
        /// <param name="name">	The name. </param>
        ///
        /// <returns>	The index, or -1 if the name doesn't exist. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int get_IndexByName(const char* name)   const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Adds a property. </summary>
        ///
        /// <remarks>	Adds a property description to the collection.  This property will now be an index
        /// 			in the associated property collection container. Note, adding properties after the
        /// 			collection has already been used may lead to unexpected results.  It is required to
        /// 			create all your properties at once, prior to using them within the sdk. </remarks>
        ///
        /// <param name="description">	The full description of the new property. </param>
        ///
        /// <returns>	The index of the new property. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int add_Property(const GW3DPropertyDescription& description) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Adds a property. </summary>
        ///
        /// <remarks>	Adds a property description to the collection.  This property will now be an index
        /// 			in the associated property collection container. Note, adding properties after the
        /// 			collection has already been used may lead to unexpected results.  It is required to
        /// 			create all your properties at once, prior to using them within the sdk. </remarks>
        ///
        /// <param name="property_name">	   	Name of the property. </param>
        /// <param name="property_type">	   	Type of the property. </param>
        /// <param name="property_description">	Information describing the property. </param>
        ///
        /// <returns>	The index of the new property. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int add_Property(const char* property_name, PropertyType property_type, const char* property_description) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Adds a property. </summary>
        ///
        /// <remarks>	Adds a property description to the collection.  This property will now be an index
        /// 			in the associated property collection container. Note, adding properties after the
        /// 			collection has already been used may lead to unexpected results.  It is required to
        /// 			create all your properties at once, prior to using them within the sdk. </remarks>
        ///
        /// <param name="property_name">	   	Name of the property. </param>
        /// <param name="property_type">	   	Type of the property. </param>
        /// <param name="property_description">	Information describing the property. </param>
        /// <param name="property_length">	   	Length of the property. </param>
        ///
        /// <returns>	The index of the new property. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int add_Property(const char* property_name, PropertyType property_type, const char* property_description, int property_length) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a definition collection. </summary>
        ///
        /// <remarks>	Note that all properties must be added before the collection is used within the SDK,
        /// 			otherwise unexpected results may occur. </remarks>
        ///
        /// <param name="share_definition_collection">	if you want this property collection to have its unique copy 
        ///				of the definition, you set this to false.  This defaults to true (shares a pointer) to 
        ///				save on memory. </param>
        ///
        /// <returns>	The new definition collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPropertyCollectionPtr create_PropertyCollection(bool share_definition_collection = true) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a clone of the definition collection. </summary>
        ///
        /// <returns>	The new clone. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DDefinitionCollectionPtr create_Clone() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the current hash. </summary>
        ///
        /// <returns>	The current hash. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int64_t get_CurrentHash() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a definition collection. </summary>
        ///
        /// <remarks>	Usage: IGW3DDefinitionCollection::create() </remarks>
        ///
        /// <param name="interface_version">	(optional) the interface version. </param>
        ///
        /// <returns>	A new definition collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static IGW3DDefinitionCollectionPtr create(unsigned interface_version = GEOWEB3D_INTERFACE_VERSION);
    };
}