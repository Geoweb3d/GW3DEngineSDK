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

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	vector layer stream result base. </summary>
    ///
    /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorLayerStreamResultBase
    {
        virtual ~IGW3DVectorLayerStreamResultBase(){}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets object identifier. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <returns>	The object identifier. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int64_t get_ObjectID() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets attribute collection. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	null if it fails, else the attribute collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DAttributeCollection* get_AttributeCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets editable attribute collection. </summary>
        ///
        /// <remarks>	This is null if put_GeometryEditableMode is set to false (the default) </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	null if it fails, else the Modifiable attribute collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DAttributeCollection* get_EditableAttributeCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the geometry. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <returns>	null if it fails, else the geometry. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const GW3DGeometry* get_Geometry() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return null if the datasource in not in a mode that supports geometry to be edited.
        /// The put the datasource into an editable mode, call put_GeometryEditableMode
        /// Use the api to query if the datasource supports this mode in general.  If it does, then you
        /// can put the datasource into a mode that this api is active. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/2/2012. </remarks>
        ///
        /// <returns>	null if it fails, else the Modifiable geometry. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DGeometry* get_EditableGeometry() = 0;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	vector layer stream result. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorLayerStreamResult : public IGW3DVectorLayerStreamResultBase
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	The unique representation properties for the current feature. </summary>
        ///
        /// <remarks>	Returns null if no properties have been set for this object id. </remarks>
        ///
        /// <param name="rep">	The rep. </param>
        ///
        /// <returns>	null if no unique properties, else the representation properties. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DPropertyCollection* get_VectorRepresentationProperties(IGW3DVectorRepresentationWPtr& rep) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the value for a representation property for the current feature. </summary>
        ///
        /// <remarks>	Note: the representation must belong to the vector layer being streamed.
        ///				If this feature did not already have a property collection, one will be created for you
        ///				cloned with the representation defaults, and this this property would be set. </remarks>
        ///
        /// <param name="rep">			 	The representation. </param>
        /// <param name="property_index">	Zero-based index of the property. </param>
        /// <param name="value">		 	The value. </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool put_VectorRepresentationProperty(IGW3DVectorRepresentationWPtr& rep, unsigned property_index, const IGW3DVariant& value) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Forces a feature/object to go through the representation callback (if set)
        ///              and re-uploaded to the GPU. </summary>
        ///	
        /// <param name="rep">			 	The representation. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_RepresentationRefresh(IGW3DVectorRepresentationWPtr& rep) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the layer that you the user used to set off the stream. </summary>
        ///
        /// <returns>	The vector layer. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorLayer* get_VectorLayer() = 0;
    };
}