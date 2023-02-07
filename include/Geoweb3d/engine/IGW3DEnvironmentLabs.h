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
#include "../../Geoweb3dCore/GW3DResults.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Fogging parameters that are use to control both haze and ground fog. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL GW3DFoggingParameters
    {
        virtual ~GW3DFoggingParameters() {};
        GW3DFoggingParameters()
            : haze_visibility_range(100000.0f)
            , haze_transition_range(2000.0f)
            //, haze_density( 0.04f )
            , fog_height(750.0f)
            , fog_visibility_range(500.0f)
            , fog_transition_range(250.0f)
            //, fog_density( 0.04f )
        {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> distance in meters at which haze will fully obscure the scene. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        float haze_visibility_range;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> distance in meters over which the scene will transition from 0% haze to 100% haze. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        float haze_transition_range;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> placeholder for future use. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //float haze_density;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> maximum distance in meters above msl that will be fully obscured by ground fog
        /// reserved for future use. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        float fog_height;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> distance in meters at which ground fog will fully obscure the scene. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        float fog_visibility_range;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> distance in meters over which the scene will transition from 0% fog to 100% fog. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        float fog_transition_range;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> placeholder for future use </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //float fog_density;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Cloud layer handle. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct CloudLayerHandle_;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Defines an alias representing handle of the cloud layer. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef CloudLayerHandle_* CloudLayerHandle;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Environment 'labs' features. </summary>
    ///
    /// <remarks>	Experimental features.  if you use this interface, you can be sure things may not
    ///				fully function correctly.  This interface will also change a lot.  It's suggested
    ///				to not use it for production usage. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DEnvironmentLabs
    {
        virtual ~IGW3DEnvironmentLabs() {};

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets shadow activity. </summary>
        ///
        /// <remarks>	If true, shadows will be enabled. </remarks>
        ///
        /// <param name="enable_shadows">	true to enable, false to disable the shadows. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_ShadowActivity(bool enable_shadows) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets shadow ambience. </summary>
        ///
        /// <remarks>	This function sets how bright or dark shadows should be for a given camera.  Values
        /// 			can range from 0.0 to 1.0, where 0.0 causes shadows to be transparent, and 1.0
        /// 			causes shadows to be pure black. </remarks>
        ///
        /// <param name="shadow_ambience">	The shadow ambience. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_ShadowAmbience(float shadow_ambience) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the number of shadow cascades. </summary>
        ///
        /// <remarks>	Geoweb3d uses the cascaded shadow maps technique for generating dynamic shadows.
        /// 			This function sets the number of cascades to use.  Generally speaking, fewer
        /// 			cascades will result in better performance but decreased quality, while more
        /// 			cascades will result in better quality but decreased performance. 
        ///
        ///				For more information, refer to the following article:
        ///
        ///				http://developer.download.nvidia.com/SDK/10.5/opengl/src/cascaded_shadow_maps/doc/cascaded_shadow_maps.pdf
        ///				</remarks>
        ///
        /// <param name="num_cascades">	Number of cascades. </param>
        ///
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_NumShadowCascades(unsigned num_cascades) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the minimum shadow quality. </summary>
        ///
        /// <remarks>	Gets the minimum value for shadow quality that can be used in SetShadowQuality. </remarks>
        ///
        /// <returns>	The minimum shadow quality. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_MinimumShadowQuality() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the maximum shadow quality. </summary>
        ///
        /// <remarks>	Gets the maximum value for shadow quality that can be used in SetShadowQuality. </remarks>
        ///
        /// <returns>	The maximum shadow quality. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_MaximumShadowQuality() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the shadow quality. </summary>
        ///
        /// <remarks>	Set the shadow quality to any value from GetMinimumShadowQuality() to
        /// 			GetMaximumShadowQuality(). Each value corresponds to a different algorithm for
        /// 			generating shadows, ranked from lowest quality to highest quality.
        ///
        /// 			Lower quality algorithms will result in more visual anomalies and aliasing, but
        /// 			have less impact on performance.  Higher quality algorithms will result in smoother,
        /// 			better looking shadows, but with a potential performance cost. </remarks>
        ///
        /// <param name="shadow_quality">	The shadow quality. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_ShadowQuality(unsigned shadow_quality) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets Fog activity. </summary>
        ///
        /// <remarks>	If true, fog and haze will be enabled. </remarks>
        ///
        /// <param name="enable_fog">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_FogActivity(bool enable_fog) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the fogging parameters. </summary>
        ///
        /// <remarks>	GW3DFoggingParameters contains members for controlling the visibility of both fog
        /// 			and haze. </remarks>
        ///
        /// <param name="parameters"> the desired GW3DFoggingParameters. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_FogParameters(const GW3DFoggingParameters& parameters) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the fogging parameters. </summary>
        ///
        /// <remarks>	GW3DFoggingParameters contains members for controlling the visibility of both fog
        /// 			and haze. </remarks>
        ///
        /// <returns>	the GW3DFoggingParameters. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const GW3DFoggingParameters& get_FogParameters() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Values that represent CloudTypes. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum CloudTypes
        {
            /// <summary>	High planar cloud puffs. </summary>
            CIRROCUMULUS,
            /// <summary>	High, thicker and fibrous clouds that signal changing weather. </summary>
            CIRRUS_FIBRATUS,
            /// <summary>	Low clouds represented as a slab. </summary>
            STRATUS,
            /// <summary>	Low, puffy clouds on fair days. </summary>
            CUMULUS_MEDIOCRIS,
            /// <summary>	Large cumulus clouds that could turn into a thunderhead. </summary>
            CUMULUS_CONGESTUS,
            /// <summary>	Big storm clouds. </summary>
            CUMULONIMBUS_CAPPILATUS,
            /// <summary>	Low, lumpy layer of clouds. </summary>
            STRATOCUMULUS,
            /// <summary>	Very large, tall cumulus clouds in the process of becoming a thunderstorm. </summary>
            TOWERING_CUMULUS,
            /// <summary>	A type of intense dust storm like a weather front.  Cloud of dust intended to be positioned at ground level. </summary>
            SANDSTORM,
            /// <summary>	Total number of cloud types. </summary>
            NUM_CLOUD_TYPES
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates cloud layer. </summary>
        ///
        /// <param name="properties">	The properties. </param>
        /// <param name="cloudtype"> TODO </param>
        /// <param name="handle">	 	[out] The handle. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult create_CloudLayer(CloudLayerHandle& handle, IGW3DEnvironmentLabs::CloudTypes cloudtype, const IGW3DPropertyCollectionPtr& properties = IGW3DPropertyCollectionPtr()) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets cloud layer properties. </summary>
        ///
        /// <param name="handle">	 	The handle. </param>
        /// <param name="cloudtype"> TODO </param>
        /// <param name="properties">	The properties. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_CloudLayer(CloudLayerHandle handle, IGW3DEnvironmentLabs::CloudTypes cloudtype, const IGW3DPropertyCollectionPtr& properties = IGW3DPropertyCollectionPtr()) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Closes cloud layer. </summary>
        ///
        /// <param name="handle">	[out] The handle. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult close_CloudLayer(CloudLayerHandle& handle) = 0;

        //SKIP-CODE-BEGIN

            ///* Adds a cloud layer to the simulated atmospheric conditions cloud deck. Cloud layers are created via
            //* the "CloudLayerHandle Create" method, and won't become part of your scene until they
            //* are added.
            //*/

            //struct SceneCloudLayerHandle_;
            //typedef SceneCloudLayerHandle_* SceneCloudLayerHandle;
            //SceneCloudLayerHandle AddCloudLayerToScene(CloudLayerHandle layer);
            //
            ///* Removes a previously added cloud deck from the scene. If the specified CloudLayer handle,
            // * if found, will be removed from the simulation and its SceneCloudLayerHandle object deleted.
            // */
            //bool RemoveCloudLayer( SceneCloudLayerHandle layerHandle);


            //struct WindVolumeHandle_;
            //typedef WindVolumeHandle_* WindVolumeHandle;
            //GEOWEB3DENGINE_API WindVolumeHandle Create();
            //
            ///* Set the minimum altitude affected by this object's wind settings.
            //* aram metersMSL The minimum altitude of this WindVolume in meters above mean sea level.
            //* sa GetMinAltitude()
            //*/
            //void   SetMinAltitude( WindVolumeHandle wh, double metersMSL);
            ///*Retrieves the minimum altitude, in meters above mean sea level, affected by this object.
            //* sa SetMinAltitude()
            //*/
            //double GetMinAltitude( WindVolumeHandle wh) const;

            ///* Set the maximum altitude affected by this object's wind settings.
            // * param metersMSL The maximum altitude of this WindVolume in meters above mean sea level.
            // * sa GetMaxAltitude()
            // */
            //void   SetMaxAltitude( WindVolumeHandle wh, double metersMSL);
            //
            ///* Retrieves the maximum altitude, in meters above mean sea level, affected by this object.
            // * sa SetMaxAltitude()
            // */
            //double GetMaxAltitude( WindVolumeHandle wh, ) const;
            //
            ///* Set the wind velocity within this WindVolume, in meters per second.
            // *sa GetWindSpeed()
            // */
            //void   SetWindSpeed( WindVolumeHandle wh, double metersPerSecond);
            //
            ///* Retrieves the wind velocity within this WindVolume, in meters per second.
            // * sa SetWindSpeed()
            // */
            //double GetWindSpeed( WindVolumeHandle wh, ) const;
            //
            ///* Sets the wind direction, in degrees East from North. This is the direction the wind is
            // * blowing toward, not the direction it is coming from.
            // * sa GetDirection()
            // */
            //void   SetDirection( WindVolumeHandle wh, double degreesFromNorth);
            //
            ///* Retrieves the wind direction, in degrees East from North. This is the direction the wind
            // * is blowing toward, not the direction it is coming from.
            // * sa SetDirection()
            // */
            //double GetDirection( WindVolumeHandle wh ) const;
            //
            ///* Evaluates if a given altitude is affected by this WindVolume.
            // * param metersMSL The altitude to query on, in meters above mean sea level.
            // * return True if the given altitude is affected by this WindVolume object.
            // */
            //bool   Inside( WindVolumeHandle wh, double metersMSL) const;

        //SKIP-CODE-END

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> If you need to disable drawing of the sky (sky will always be black) you can control 
        /// such via the put_DrawSkyEnabled. </summary>
        ///
        /// <param name="enable"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_DrawSky(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_DrawSky() = 0;

    };

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
///
/// <param name="cloudtype"> TODO </param>
///
/// <returns> TODO </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" GW3D_DLL const IGW3DPropertyCollection * get_DefaultCloudProperties( IGW3DEnvironmentLabs::CloudTypes cloudtype );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> TODO </summary>
///
/// <param name="handle"> TODO </param>
///
/// <returns> TODO </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" GW3D_DLL const IGW3DPropertyCollection *get_Properties( CloudLayerHandle handle);
}