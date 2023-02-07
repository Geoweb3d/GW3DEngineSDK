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
    /// <summary>	An Event Stream object that is used to receive all events using
    /// 			IGW3DGeoweb3dSDK::run_EventStream.  The client inherits this object and implements
    /// 			OnPagingEvent, OnStereoEvent, etc, which gets called for every event in the queue. </summary>
    ///
    /// <remarks>	The event system will store up to 10000 events, thus its suggested to empty ASAP!
    /// 			All events are preserved, meaning if you removed a representation or datasource
    /// 			before processing these events, you will still get the notifications they added
    /// 			onto this event system.
    ///
    /// 			All Start Events will have a following Finished event. In most cases, you will
    /// 			have multiple outstanding start events for a RepresentationLayerHandle, but the
    /// 			const Envelope& bounds will be unique, as well as the unique_id that can be used
    /// 			to follow the start event through its finish event.
    ///
    /// 			Currently for Start and Finished loading events, "float value" will be 0.  Do not
    /// 			depend on that being the case for future behaviors. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DEventStream
    {
        virtual ~IGW3DEventStream() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Values that represent the paging event type. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum PagingEventType
        {
            /// <summary>	An enum constant representing an area of a datasource paging in. </summary>
            DataSourceStartLoading,
            /// <summary>	An enum constant representing an area of a datasource finished paging in. </summary>
            DataSourceFinishedLoading,
            /// <summary>	An enum constant representing the percent complete for paging in an area. </summary>
            DataSourceWorkEstimationComplete,
            /// <summary>	An enum constant representing starting to create a quick cache in an area. </summary>
            QuickCacheCreateStart,
            /// <summary>	An enum constant representing finishing creating a quick cache in an area. </summary>
            QuickCacheCreateFinished,
            /// <summary>	An enum constant representing the percent complete for caching an area. </summary>
            QuickCacheEstimationComplete,
            /// <summary>	Do Not Use. </summary>
            PagingEventSubTypeMax
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Callback with information about a paging event. </summary>
        ///
        /// <remarks>	Because Geoweb3d selectively pages data on and off disk as needed, it's possible that
        /// 			for larger datasets that the entire dataset may not ever be loaded at the same time.
        /// 			For this reason, OnPagingEvent provides a bounds for the area that is being processed,
        /// 			and the same event types may be received for same representation with different areas
        /// 			that are being processed simultaneously.
        ///
        /// 			The unique_id can be used to follow work from start to finish for a given bounds. </remarks>
        ///
        /// <param name="representation">	The representation. </param>
        /// <param name="etype">		 	The event type. </param>
        /// <param name="unique_id">	 	Unique identifier. </param>
        /// <param name="bounds">		 	The bounds. </param>
        /// <param name="value">		 	The value (for example, percent complete). </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnPagingEvent(IGW3DVectorRepresentationWPtr representation, PagingEventType etype, unsigned unique_id, const GW3DEnvelope& bounds, unsigned int value) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Values that represent the stereo event type. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum StereoEventType
        {
            /// <summary>	An enum constant representing an area of a datasource paging in. </summary>
            HMD_CONNECTED = 0,
            /// <summary>	An enum constant representing an area of a datasource paging in. </summary>
            HMD_DISCONNECTED = 1,
            /// <summary>	Do Not Use. </summary>
            StereoEventSubTypeMax
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Callback with information about a paging event. </summary>
        ///
        /// <param name="stereo_driver">	The stereo driver. </param>
        /// <param name="etype">		 	The event type. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnStereoEvent(IGW3DStereoDriverWPtr stereo_driver, StereoEventType etype) = 0;
    };

}