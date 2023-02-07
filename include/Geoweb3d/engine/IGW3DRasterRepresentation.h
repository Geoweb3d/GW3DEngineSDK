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
#include "../core/IGW3DClientUserData.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Raster representation. </summary>
    ///
    /// <remarks>	A raster representation defines how a raster is represented in the 3D scene. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DRasterRepresentation : public IGW3DClientUserData
    {
        virtual ~IGW3DRasterRepresentation() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the driver. </summary>
        ///
        /// <remarks>	Gets the driver that loaded this representation. </remarks>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterRepresentationDriverWPtr get_Driver() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the raster layer. </summary>
        ///
        /// <remarks>	Gets the raster layer that this is representing. </remarks>
        ///
        /// <returns>	The raster layer. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DRasterLayerWPtr get_RasterLayer() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enable or disable the raster representation. </summary>
        ///
        /// <remarks>	Enables or disables the representation.  This does not start or stop loading, it
        /// 			just enables/disables the representation from being rendered or analyzed. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_Enabled(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if the raster representation is enabled. </summary>
        ///
        /// <returns>	true if enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool  get_Enabled() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the property collection. </summary>
        ///
        /// <remarks>	put_PropertyCollection is used to change the default values of the representation
        /// 			properties from that of the driver defaults. There is minimal error checking in the
        /// 			sdk, as it only validates the property definition matches. If you set the property
        /// 			collection to null, then this representation will go back to using the driver's
        /// 			default values. </remarks>
        ///
        /// <param name="p">	The property collection. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	void put_PropertyCollection(const IGW3DPropertyCollection* p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the property collection. </summary>
        ///
        /// <remarks>	put_PropertyCollection is used to change the default values of the representation
        /// 			properties from that of the driver defaults. There is minimal error checking in the
        /// 			sdk, as it only validates the property definition matches. If you set the property
        /// 			collection to null, then this representation will go back to using the driver's
        /// 			default values. </remarks>
        ///
        /// <param name="p">	The property collection. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	void put_PropertyCollection(const IGW3DPropertyCollectionPtr p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the default properties. </summary>
        ///
        /// <remarks>	Gets the default properties for this representation. If null, the representation
        /// 			will use the driver defaults. </remarks>
        ///
        /// <returns>	The property collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

    	virtual const IGW3DPropertyCollection* get_PropertyCollection( ) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the paging level </summary>
        ///
        /// <remarks>	Sets the paging level, which must less than or equal to the maximum terrain tree
        /// 			depth. This can be a very expensive call so its usage should be limited.  It
        /// 			is recommended to set the page level at the time the representation is created.
        ///
        /// 			Note that you may determine the distance in meters that a page level will come in
        /// 			using the Geoweb3d::Coordinates::PageLevelToMeterRadius function.
        ///				
        ///				Not yet supported for representations created with IGW3DPagingPolicy.</remarks>
        ///
        /// <param name="page_level">	The page level. </param>
        ///
        /// <returns>	GW3D_Ok if succeeded. </returns>
        ///
        /// <see cref="Geoweb3d::IGW3DInitializationConfiguration"/>
        /// <see cref="Geoweb3d::Coordinates::PageLevelToMeterRadius"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_PagingLevel(unsigned page_level) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the paging level. </summary>
        ///
        /// <returns>	The paging level. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_PagingLevel() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the priority level </summary>
		///
		/// <remarks>	Sets the priority level, The imagery priority(images with higher values display on 
		///             top of lower priority images).</remarks>
		///
		/// <param name="priority">	The priority level. </param>
		///
		/// <returns>	GW3D_Ok if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual GW3DResult put_Priority(int priority) = 0;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the priority level. </summary>
		///
		/// <returns>	The priority level. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual int get_Priority() const = 0;
    };
}