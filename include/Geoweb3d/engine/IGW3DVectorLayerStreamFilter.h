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
#include "../core/IGW3DCollection.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	A vector layer stream filter. </summary>
    ///
    /// <remarks>	This interface is implemented by the SDK client and defines a subset of a vector
    /// 			layer's features to process in a stream.  To process all features, return 0 for the
    /// 			count.
    ///
    /// 			Streams are designed to be very fast and avoid all or most memory copies in the
    /// 			pipeline.  Thus, when the SDK wants something within this filter, it calls back into
    /// 			the SDK client. Its probably best to use a std::vector or boost array supporting
    /// 			contiguous memory as the internal container. You can use the GW3DStreamFilterHelper
    /// 			class if you are not concerned about maximizing stream performance. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorLayerStreamFilter : public IGW3DCollection< int64_t >
    {
        virtual ~IGW3DVectorLayerStreamFilter(){}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Get the master clock time. </summary>
        ///
        /// <remarks>	This tells the SDK not to process any objects/features that were modified prior to
        /// 			the returned master clock time.  By default, features are not filtered by master
        /// 			clock time. </remarks>
        ///
        /// <returns>	The since master clock. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_SinceMasterClock() {
            return 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the count of features to be processed. </summary>
        ///
        /// <remarks>	This is required to be implemented.  Note: if count returns 0, then no other of the
        /// 			APIs used to index into the collection will get called.  What this means is if you
        /// 			wish to stream an entire layer, its safe to return 0 for everything. </remarks>
        ///
        /// <returns>	The count. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned long count() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the next feature/object id in the collection and increments the iterator.
        /// 			</summary>
        ///
        /// <param name="ppVal">	[out] If non-null, the value. </param>
        ///
        /// <returns>	true if it succeeds, false if no more features exist in the collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool next(int64_t* ppVal) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Resets the iterator. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void reset() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Array indexer operator. </summary>
        ///
        /// <remarks>	Get the feature/object id at the given index. </remarks>
        ///
        /// <param name="index">	Zero-based index of the feature/object id. </param>
        ///
        /// <returns>	The feature/object id. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int64_t operator[](unsigned long index) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets feature/object id at index. </summary>
        ///
        /// <remarks>	Get the feature/object id at the given index. </remarks>
        ///
        /// <param name="index">	Zero-based index of the feature/object id. </param>
        ///
        /// <returns>	The feature/object id. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int64_t get_AtIndex(unsigned long index) = 0;

    };
}