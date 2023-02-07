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
    /// <summary>	A query of the geodetic position at a given window pixel coordinate. </summary>
    ///
    /// <remarks>	This query may be used to query the position of what is at a given window
    /// 			coordinate, and may be filtered to return only the terrain position or the position
    /// 			of the terrain or any scene geometry, whichever is nearer.
    ///
    /// 			Note that this is useful for panning and other navigation if the window coordinate
	/// 			is set to the mouse position every frame.
	/// 
	///             Also of note, ensure an object's life time of this type lives beyond the window's
	///             camera it was created from. It cannot be used immediately in-place and discarded.
	///             To check if the query is working, its get_IsValid should be true. A common usage
	///             pattern for simplicity would be to make a local / member std::map of an
	///             IGW3DCameraPtr to an IGW3DWindowCoordinateToGeodeticQueryWPtr. The reasoning
	///             behind this concept is for efficiency, since if you don't use it, it won't
	///             cause any overhead.  </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DWindowCoordinateToGeodeticQuery : public IGW3DClientUserData//, public IGW3DFilter
    {
        virtual ~IGW3DWindowCoordinateToGeodeticQuery(){}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Values that represent the query filter type. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum QueryFilterType
        {
            /// <summary>	Query terrain only. </summary>
            TERRAIN_ONLY = 0,
            /// <summary>	Query terrain and any scene geometry. </summary>
            TERRAIN_AND_ALL_OTHER_GEOMETRY = 1,
            /// <summary>	Do Not Use. </summary>
            QueryFilterMAX = 0xFFFF
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets whether the query is enabled or disabled. </summary>
        ///
        /// <remarks>	If the query is disabled, it will not be processed when the scene is drawn and
        /// 			therefore its result will not be updated. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Enabled(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Check if the query is enabled. </summary>
        ///
        /// <returns>	true if enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_Enabled() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the query filter. </summary>
        ///
        /// <remarks>	Sets the query filter that determines what aspects of the scene will return a
        /// 			position at the given window coordinate. </remarks>
        ///
        /// <param name="type">	The filter type. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_QueryFilter(QueryFilterType type) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the query filter type. </summary>
        ///
        /// <returns>	The query filter type. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual QueryFilterType get_QueryFilter() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the window coordinate origin. </summary>
        ///
        /// <remarks>	Sets the window coordinate origin to be used in defining window coordinates.
        ///				in this class.  LOWER_LEFT is the assumed origin for OpenGL, where UPPER_LEFT
        ///				is the assumed origin for GDI, X11 and Direct3D through DirectX version 9. </remarks>
        ///
        /// <param name="origin">	The origin. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_WindowCoordinateOrigin(WindowCoordinateOrigin origin) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the window coordinate origin. </summary>
        ///
        /// <returns>	[out] The currently configured origin. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual WindowCoordinateOrigin get_WindowCoordinateOrigin() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the window coordinate to query. </summary>
        ///
        /// <remarks>	Sets the window coordinate on which to read back the geodetic information.
        ///
        /// 			VAA note: might change to be using the normalized camera position within the
        /// 			window..hmm.. but they can do this too..use case?  </remarks>
        ///
        /// <param name="x">	The x pixel coordinate. </param>
        /// <param name="y">	The y pixel coordinate. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_WindowCoordinate(int x, int y) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the window coordinate to query. </summary>
        ///
        /// <param name="x">	[out] The x coordinate. </param>
        /// <param name="y">	[out] The y coordinate. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_WindowCoordinate(int& x, int& y) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the geodetic result. </summary>
        ///
        /// <remarks>	Gets the resulting geodetic position based on the given window coordinate and
        /// 			query filter.
        ///
        /// 			It is important to check is_Valid() before using this result, in case there was no
        /// 			result at the given window coordinate (for example, if the window coordinate is
        /// 			over the sky or outside the width/height of the window).  In this case, the result
        /// 			is not defined.
        ///
        /// 			Note that there can be some z error with terrain results due to how it is
        /// 			calculated.  If the result represents the terrain surface it is recommended that
        /// 			the terrain elevation be queried using Geoweb3d::Dem::GetElevationAt at the
        /// 			latitude and longitude given by the geodetic result. </remarks>
        ///
        /// <returns>	The geodetic result. </returns>
        ///
        /// <see cref="Geoweb3d::Dem::GetElevationAt"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const GW3DPoint* get_GeodeticResult() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the frame time stamp of the last frame the query was last analyzed. </summary>
        ///
        /// <returns>	The frame time stamp. </returns>
        ///
        /// <see cref="Geoweb3d::SDK_Statistics::GetMasterFrameCount"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_FrameTimeStamp()const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Checks if the query result is valid. </summary>
        ///
        /// <remarks>	If this returns false, the results of the query are invalid and not defined.  This
        /// 			can occur if an error occurs, or if there is nothing matching the filter type at
        /// 			the specified window coordinate. </remarks>
        ///
        /// <returns>	true if result is valid, false if it is invalid. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool	get_IsValid() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the pixel radius for the query. </summary>
        ///
        /// <remarks>	Sets the pixel radius around the window coordinate. All window pixels within the
        /// 			radius will be tested, such that the average of all results within the radius will
        /// 			be returned as the result. </remarks>
        ///
        /// <param name="radius">	The radius (0 means get the exact pixel). </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_PixelRadius(unsigned radius) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the pixel radius. </summary>
        ///
        /// <remarks>	Gets the pixel radius within which an average location is derived for the result.
        /// 			</remarks>
        ///
        /// <returns>	The pixel radius. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_PixelRadius() const = 0;
    };
}