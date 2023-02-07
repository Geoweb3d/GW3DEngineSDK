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
    /// <summary>	Environment effects API. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DEnvironmentEffects
    {
        virtual ~IGW3DEnvironmentEffects() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Values that represent PrecipitationType. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum PrecipitationType
        {
            /// <summary>	No precipitation. </summary>
            NONE = 0,
            /// <summary>	Rain. </summary>
            RAIN = 1,
            /// <summary>	Dry snow. </summary>
            DRY_SNOW = 2,
            /// <summary>	Wet snow. </summary>
            WET_SNOW = 3,
            /// <summary>	Sleet. </summary>
            SLEET = 4
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets camera exposure. </summary>
        ///
        /// <param name="exposure">	The exposure. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_Exposure(float exposure) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the camera exposure. </summary>
        ///
        /// <returns>	The exposure. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_Exposure() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enable/disable lens flare. </summary>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableLensFlare(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if lens flare enabled. </summary>
        ///
        /// <returns>	true if enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_EnableLensFlare() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the simulated amount of nighttime light pollution, in watts per square meter.
        /// 			</summary>
        ///
        /// <param name="Wm2">	Light pollution in watts per square meter. Default is 0.001. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_LightPollution(double Wm2) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the currently simulated light pollution, in watts per square meter. </summary>
        ///
        /// <returns>	The light pollution in watts per square meter. . Default is 0.001. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_LightPollution() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets simulated global precipitation of a specified type. </summary>
        ///
        /// <param name="precipitationType">	The type of precipitation to simulate. </param>
        ///
        /// <param name="precipitationRate">	The simulated rate of precipitation, in millimeters per
        /// 									hour. Reasonable ranges might be between 1 for light rain or
        /// 									20 for heavier rain, to a default maximum of 30. </param>
        ///
        /// <param name="nearClip">				How close to the camera the closest precipitation
        /// 									particles will be rendered. The near clipping plane will be
        /// 									adjusted to include this distance while the precipitation is
        /// 									being rendered. Values less than zero will result in the
        /// 									default value being applied. </param>
        ///
        /// <param name="farClip">				The farthest distance from the camera that precipitation
        /// 									particles will be rendered. Since there is an upper bound on
        /// 									the number of particles rendered per frame, changing the
        /// 									difference between nearClip and farClip may result in changes
        /// 									to the intensity of the precipitation. By default this
        /// 									difference is 7.5 world units. Values less than zero will
        /// 									result in the default value being applied. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Precipitation(PrecipitationType precipitationType, double precipitationRate, double nearClip = -1, double farClip = -1) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the current global precipitation. </summary>
        /// 
        /// <remarks>	Simulates global precipitation of a specified type. Precipitation effects will 
        /// 			display if the type is set to something other than NONE.
        /// 			
        /// 			The simulated rate of precipitation in millimeters per hour. Reasonable ranges
        ///				might be between 1 for light rain or 20 for heavier rain.
        ///				
        ///				The near and far clip values control the nearest and farthest distances that 
        ///				precipitation particles will be rendered.  Values less than zero will result in the 
        ///				default value being applied.
        ///
        /// <param name="precipitationType">	[out] Type of the precipitation. </param>
        /// <param name="precipitationRate">	[out] The precipitation rate. </param>
        /// <param name="nearClip">				[out] The near clip </param>
        /// <param name="farClip">				[out] The far clip. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_Precipitation(PrecipitationType& precipitationType, double& precipitationRate, double& nearClip, double& farClip) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the simulated atmospheric turbidity. </summary>
        ///
        /// <remarks>	Sets the "turbidity" of the simulated atmosphere.
        ///
        ///				You can think of this as a measure of "haziness." 1.0 would be a perfectly clear
        ///				day and is the minimum value. Some guidelines for setting this value:
        ///
        ///				2 = very clear, range 50 km 3 = clear, range 15 km 7 = light haze, range 8 km
        ///
        ///				Setting turbidity does not simulate fog.  Turbidity just lets you simulate the number
        ///				of particles in the air. In practice it will vary the color of the sky from a pure
        ///				light blue to a hazy, darker, yellowish color. Valid range is between 1.8 and 20.0.
        ///				</remarks>
        ///
        /// <param name="turbidity">	the turbidity </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Turbidity(double turbidity) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the simulated atmospheric turbidity. </summary>
        ///
        /// <returns>	The turbidity. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual double get_Turbidity() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the sunlight diffuse lighting offset. </summary>
        ///
        /// <remarks>	Offsets the diffuse lighting. </remarks>
        ///
        /// <param name="red">  	The red. </param>
        /// <param name="green">	The green. </param>
        /// <param name="blue"> 	The blue. </param>
        /// <param name="alpha">	The alpha. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_SunlightDiffuseOffset(float red, float green, float blue, float alpha) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the sunlight diffuse lighting offset. </summary>
        ///
        /// <param name="red">  	[out] The red. </param>
        /// <param name="green">	[out] The green. </param>
        /// <param name="blue"> 	[out] The blue. </param>
        /// <param name="alpha">	[out] The alpha. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_SunlightDiffuseOffset(float& red, float& green, float& blue, float& alpha) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the sunlight ambient lighting offset. </summary>
        ///
        /// <remarks>	Offsets the ambient lighting. </remarks>
        ///
        /// <param name="red">  	The red. </param>
        /// <param name="green">	The green. </param>
        /// <param name="blue"> 	The blue. </param>
        /// <param name="alpha">	The alpha. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_SunlightAmbientOffset(float red, float green, float blue, float alpha) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the sunlight ambient lighting offset. </summary>
        ///
        /// <param name="red">  	[out] The red. </param>
        /// <param name="green">	[out] The green. </param>
        /// <param name="blue"> 	[out] The blue. </param>
        /// <param name="alpha">	[out] The alpha. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void get_SunlightAmbientOffset(float& red, float& green, float& blue, float& alpha) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the terrain alpha. </summary>
        ///
        /// <remarks>	Sets the transparency of the terrain surface.  Valid values range from 0.0 to 1.0. </remarks>
        ///
        /// <param name="alpha">	The alpha. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_TerrainAlpha(float alpha) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the terrain alpha. </summary>
        ///
        /// <remarks>	Gets the transparency of the terrain surface. </remarks>
        ///
        /// <returns>	The terrain alpha. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_TerrainAlpha() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the terrain brightness. </summary>
        ///
        /// <remarks>	Sets the brightness of the terrain surface.  Valid values range from -1.0 to 1.0.
        ///				-1.0 will be fully darkened and 1.0 will be fully brightened. </remarks>
        ///
        /// <param name="brightness">	The brightness. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_TerrainBrightness(float brightness) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the terrain brightness. </summary>
        ///
        /// <remarks>	Gets the brightness of the terrain surface. </remarks>
        ///
        /// <returns>	The terrain brightness. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_TerrainBrightness() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Environment 'Labs' API</summary>
        ///
        /// <remarks>	For experimental features.  If you use this interface, you can be sure things may
        /// 			not fully function correctly.  This interface will also change a lot.  It's
        /// 			suggested to not use it for production usage. </remarks>
        ///
        /// <returns>	the environment labs API. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DEnvironmentLabs* get_EnvironmentLabs() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Lighting Configuration API</summary>
        ///
        /// <remarks>	Use this interface to access lighting configuration that allows the scene lighting
        ///				to be controlled by the user rather than controlled by time-of-day. </remarks>
        ///
        /// <returns>	the lighting configuration API. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DLightingConfiguration* get_LightingConfiguration() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Lighting Configuration API</summary>
        ///
        /// <remarks>	Use this interface to access lighting configuration that allows the scene lighting
        ///				to be controlled by the user rather than controlled by time-of-day. </remarks>
        ///
        /// <returns>	the lighting configuration API. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DLightingConfiguration* get_LightingConfiguration() const = 0;

    };
}