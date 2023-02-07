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
//#include "IGW3DRasterBandStream.h"
#include "../../Geoweb3dCore/GeometryExports.h"

/* Primary namespace */
namespace Geoweb3d
{
    class IGW3DVariant;

    struct  IGW3DRasterBandStream; //what would we want to stream?

    struct  IGW3DRasterBandRepresentationCollectionViewer;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Custom Nodata filter. </summary>
    ///
    /// <remarks>	The custom NoData filter defines how areas of no data are identified for exclusion.
    /// 			TODO -- this goes into the representation </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DCustomNoDataFilter
    {
        virtual ~IGW3DCustomNoDataFilter() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables or disables the filter. </summary>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_Enable(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Queries if filter is enabled. </summary>
        ///
        /// <returns>	true if enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_Enable()  const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the normalized pixel value that defines a NoData pixel </summary>
        ///
        /// <remarks>	Pixels with this value will be excluded, and the image with the next lowest
        /// 			priority will be displayed in its place. </remarks>
        ///
        /// <param name="value">	The normalized pixel value (0.0 to 1.0). </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_NormalizedNoDataPixelValue(float value) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the normalized NoData pixel value. </summary>
        ///
        /// <returns>	The normalized NoData pixel value. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_NormalizedNoDataPixelValue() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets allowable variation. </summary>
        ///
        /// <remarks>	Amount that NoData pixel value can vary and will still be considered invalid. </remarks>
        ///
        /// <param name="parameter1">	The first parameter. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_AllowableTolerance(float) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets allowable variation. </summary>
        ///
        /// <remarks>	Amount that NoData pixel value can vary and will still be considered invalid. </remarks>
        ///
        /// <returns>	The allowable variation. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual float get_AllowableTolerance() const = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Raster Band. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DRasterBand
    {
        virtual ~IGW3DRasterBand() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Pixel data types. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        typedef enum {
            /// <summary>Unknown or unspecified type.</summary>
            GW3D_Unknown = 0,
            /// <summary>Eight bit unsigned integer.</summary>
            GW3D_Byte = 1,
            /// <summary>Sixteen bit unsigned integer.</summary>
            GW3D_UInt16 = 2,
            /// <summary>Sixteen bit signed integer.</summary>
            GW3D_Int16 = 3,
            /// <summary>Thirty two bit unsigned integer.</summary>
            GW3D_UInt32 = 4,
            /// <summary>Thirty two bit signed integer.</summary>
            GW3D_Int32 = 5,
            /// <summary>Thirty two bit floating point.</summary>
            GW3D_Float32 = 6,
            /// <summary>Sixty four bit floating point.</summary>
            GW3D_Float64 = 7,
            /// <summary>Complex Int16.</summary>
            GW3D_CInt16 = 8,
            /// <summary>Complex Int32.</summary>
            GW3D_CInt32 = 9,
            /// <summary>Complex Float32.</summary>
            GW3D_CFloat32 = 10,
            /// <summary>Complex Float64.</summary>
            GW3D_CFloat64 = 11,
            /// <summary>GW3D_TypeCount.</summary>
            GW3D_TypeCount = 0xFF
        } GW3DPixelDataType;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the name. </summary>
        ///
        /// <returns>	The name of the band. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_Name() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the size along the x coordinate. </summary>
        ///
        /// <returns>	The pixel width. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int         get_Width() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the size along the y coordinate. </summary>
        ///
        /// <returns>	The pixel height. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int         get_Height() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the band number. </summary>
        ///
        /// <returns>	The band number. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int         get_Band() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the pixel data type. </summary>
        ///
        /// <returns>	The pixel data type. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DPixelDataType get_PixelDataType()const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Return raster unit type. </summary>
        ///
        /// <returns>	The unit type. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_UnitType() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the native NoData value </summary>
        ///
        /// <remarks>	The NoData value defined natively by the raster band.  Returns a null variant type
        /// 			if the band itself does not contain a no data value. </remarks>
        ///
        /// <returns>	The native no data value, or a null variant if none exists. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVariant get_NativeNoDataValue() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets if native NoData value should be excluded. </summary>
        ///
        /// <remarks>	If set to true, pixels with the native NoData value will be excluded, and the
        /// 			image with the next lowest priority will be displayed in its place. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_EnableNativeNoDataValue(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if native no data value is enabled. </summary>
        ///
        /// <remarks>	If true, pixels with the natively-defined NoData value are excluded.   </remarks>
        ///
        /// <param name="enable">	true if enabled, false if disabled. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_EnableNativeNoDataValue() const = 0;

    //SKIP-CODE-BEGIN

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets custom no data filter. </summary>
        ///
        /// <remarks>	Geoweb3d, 11/6/2012. </remarks>
        ///
        /// <returns>	The custom no data filter. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        //TODO, goes into the representation
        //virtual IGW3DCustomNoDataFilterWPtr get_CustomNoDataFilter() const = 0;

    //SKIP-CODE-END

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Streams the raster band. </summary>
        ///
        /// <remarks>	TODO. </remarks>
        ///
        /// <param name="streamobj">	The raster band stream object to be streamed. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void Stream(IGW3DRasterBandStream* streamobj) = 0;
    };
}