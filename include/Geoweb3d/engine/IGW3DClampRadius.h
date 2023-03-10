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
    /// <summary>	A clamp radius. </summary>
    ///
    /// <remarks>	A clamp radius is a filter limiting the range of distance over which something is
    /// 			done. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DClampRadius
    {
        virtual ~IGW3DClampRadius() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables or disables the clamp radius. </summary>
        ///
        /// <remarks>	If disabled, the clamp radius does not apply its filtering. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_Enabled(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if the clamp radius is enabled. </summary>
        ///
        /// <returns>	true if it is enabled, false if it is disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool  get_Enabled() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the minimum radius. </summary>
        ///
        /// <remarks>	Sets the minimum radius.  Anything closer than this distance will be ignored. </remarks>
        ///
        /// <param name="radius">	The minimum radius. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_MinRadius(double radius) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the maximum radius. </summary>
        ///
        /// <remarks>	Sets the maximum radius.  Anything further than this distance will be ignored. </remarks>
        ///
        /// <param name="radius">	The maximum radius. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_MaxRadius(double radius) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the minimum radius. </summary>
        ///
        /// <returns>	The minimum radius. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_MinRadius() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the maximum radius. </summary>
        ///
        /// <returns>	The maximum radius. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_MaxRadius() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the maximum radius' transition range. </summary>
        ///
        /// <remarks>	Sets the maximum radius' transition range.  The transition will commence from the 
        /// 			the maximum range minus this value.  
		///				Currently only applicable in the BillBoard and Draped Ring Representation. </remarks>
        ///
        /// <param name="maxRadiustransition">	The maximum radius' transition range. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_MaxRadiusTransitionRange(double maxRadiustransition) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the minimum radius' transition range. </summary>
        ///
        /// <returns>	The minimum radius' transition range. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_MinRadiusTransitionRange() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the maximum radius' transition range. </summary>
        ///
        /// <returns>	The maximum radius' transition range. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_MaxRadiusTransitionRange() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Gets whether or not this IGW3DClampRadius is applied during any analytics pass. </summary>
		/// 
		/// <returns> true if this IGW3DClampRadius is applied to analytics, false otherwise </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual bool get_ApplyToAnalytics() const = 0;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Sets whether or not this IGW3DClampRadius is applied during any analytics pass. </summary>
		///
		/// <param name="apply"> true to apply this IGW3DClampRadius to analytics. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual void put_ApplyToAnalytics( bool apply ) = 0;
    };
}