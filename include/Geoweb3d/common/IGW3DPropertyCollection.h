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
#include "../core/IGW3DCollection.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Collection of properties. </summary>
    ///
    /// <remarks>	This is a generic object, used for delivering a set of properties or attributes.
    /// 			</remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DPropertyCollection : public IGW3DCollection< IGW3DVariant* >
    {
        virtual ~IGW3DPropertyCollection() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Assignment operator. </summary>
        ///
        /// <param name="p"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPropertyCollection& operator = (const IGW3DPropertyCollection* p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Assignment operator. </summary>
        ///
        /// <param name="p"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPropertyCollection& operator = (const IGW3DPropertyCollectionPtr& p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Retrieve the item at the given index. </summary>
        ///
        /// <remarks>	Use caution when using this function in conjunction with put_Property. The pointer
        /// 			needs to be dereferenced to exhibit proper behavior. It is recommended to use 
        ///				get_Property instead. </remarks>
        ///
        /// <param name="index">	Zero-based index of the item. </param>
        ///
        /// <returns>	The item. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVariant* operator[](unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Retrieve the item at the given index. </summary>
        ///
        /// <remarks>	Use caution when using this function in conjunction with put_Property. The pointer
        /// 			needs to be dereferenced to exhibit proper behavior. It is recommended to use 
        ///				get_Property instead. </remarks>
        ///
        /// <param name="index">	Zero-based index of the item. </param>
        ///
        /// <returns>	The item. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVariant* get_AtIndex(unsigned long index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets definition collection. </summary>
        ///
        /// <remarks>	Gets the definition collection that defines the property/field names, types, and
        /// 			descriptions.  Each index in the definition coincides with the index in the
        /// 			property collection. </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	the definition collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DDefinitionCollection* get_DefinitionCollection(bool reset_next_iterator = false) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the value of a property. </summary>
        ///
        /// <param name="index">	Zero-based index of the property. </param>
        /// <param name="val">  	The value. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_Property(unsigned index, const IGW3DVariant& val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the value of a property. </summary>
        ///
        /// <param name="index">	Zero-based index of the property. </param>
        /// <param name="val">  	The value. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_Property(unsigned index, const double& val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the value of a property. </summary>
        ///
        /// <param name="index">	Zero-based index of the property. </param>
        /// <param name="val">  	The value. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_Property(unsigned index, int val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the value of a property. </summary>
        ///
        /// <param name="index">	Zero-based index of the property. </param>
        /// <param name="val">  	The value. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_Property(unsigned index, int64_t val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the value of a property. </summary>
        ///
        /// <param name="index">	Zero-based index of the property. </param>
        /// <param name="val">  	The value. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_Property(unsigned index, bool val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the value of a property. </summary>
        ///
        /// <param name="index">	Zero-based index of the property. </param>
        /// <param name="val">  	The value. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_Property(unsigned index, const char* val) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets a property value. </summary>
        ///
        /// <param name="index">	Zero-based index of the property. </param>
        ///
        /// <returns>	The property value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVariant& get_Property(unsigned index) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a clone of the property collection. </summary>
        ///
        /// <remarks>	If the SDK supplies a const property collection, you may use this API to create a
        /// 			copy of the property collection.  This can then be modified and submitted back to
        /// 			the SDK, for example in a 'set' function. </remarks>
        ///
        /// <param name="share_definition_collection">	defaults to true, where the property collections
        ///												will share the same definition collection
        ///												(pointer vs a copy of). </param>
        ///
        /// <returns>	The new clone of the property collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DPropertyCollectionPtr create_Clone(bool share_definition_collection = true) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Unique ID for the complete object based on its contents </summary>
        ///
        /// <remarks>	This is code you can use to detect if you changed the object. </remarks>
        ///
        /// <returns>	returns the current hashcode </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int64_t get_CurrentHash() const = 0;

    };
}