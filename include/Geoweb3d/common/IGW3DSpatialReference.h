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
    /// <summary>	A spatial reference. </summary>
    ///
    /// <remarks>	Initialized to EPSG:4326 (WGS84). </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DSpatialReference
    {
        virtual ~IGW3DSpatialReference()
        {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Values that represent WKTModeFlag. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum WKTModeFlag
        {
            /// <summary>	Horizontal Only. </summary>
            eHorizontalOnly = 1,
            /// <summary>	Compound OK. </summary>
            eCompoundOK = 2
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Equality operator. </summary>
        ///
        /// <param name="other">	The other. </param>
        ///
        /// <returns>	true if the parameters are considered equivalent. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool operator==(const IGW3DSpatialReference& other) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Inequality operator. </summary>
        ///
        /// <param name="other">	The other. </param>
        ///
        /// <returns>	true if the parameters are not considered equivalent. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool operator!=(const IGW3DSpatialReference& other) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Well-Known Text (WKT) description of the spatial reference. </summary>
        ///
        /// <remarks>	Returns the OGC WKT describing the Spatial Reference System. Uses GDAL's operations
        /// 			and	methods to determine the WKT. WKTModeFlag mode_flag may be eHorizontalOnly
        /// 			indicating the WKT will not include vertical coordinate system info (the default),
        /// 			or eCompoundOK indicating the the returned WKT may be a compound coordinate system
        /// 			if there is vertical coordinate system info available. </remarks>
        ///
        /// <param name="mode_flag">	(optional) the mode flag. </param>
        ///
        /// <returns>	The WKT. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_WKT(WKTModeFlag mode_flag = eHorizontalOnly) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Well-Known Text (WKT) description of the spatial reference. </summary>
        ///
        /// <remarks>	Returns the OGC WKT describing the Spatial Reference System. Uses GDAL's operations
        /// 			and	methods to determine the WKT. WKTModeFlag mode_flag may be eHorizontalOnly
        /// 			indicating the WKT will not include vertical coordinate system info (the default),
        /// 			or eCompoundOK indicating the the returned WKT may be a compound coordinate system
        /// 			if there is vertical coordinate system info available. </remarks>
        ///
        /// <param name="mode_flag">	The mode flag. </param>
        /// <param name="pretty">   	true to request 'pretty' formatting. </param>
        ///
        /// <returns>	The WKT. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_WKT(WKTModeFlag mode_flag, bool pretty) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the spatial reference information using user input. </summary>
        ///
        /// <remarks>	Sets the spatial reference system (SRS) using GDAL's SetFromUserInput function.
        /// 			const char* v is a string containing the definition (filename, proj4, wkt, etc).
        /// 			</remarks>
        ///
        /// <param name="v">	The const char* to process. </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool put_FromUserInput(const char* v) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the Proj.4 string. </summary>
        ///
        /// <remarks>	Returns the Proj.4 string describing the Spatial Reference System. It uses GDAL's
        /// 			operations and methods to determine the Proj.4 string. </remarks>
        ///
        /// <returns>	The Proj4 string. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_Proj4() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the spatial reference information using a Proj4 string. </summary>
        ///
        /// <remarks>	Sets the Proj.4 string describing the Spatial Reference System. It uses GDAL's
        /// 			operations and methods to determine the Proj.4 string v is a string containing
        /// 			the Proj.4 string. </remarks>
        ///
        /// <param name="v">	The const char* to process. </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool put_Proj4(const char* v) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the spatial reference information using an epsg code. </summary>
        ///
        /// <param name="epsg_code"> TODO </param>
        ///
        /// <returns>	true if it succeeds, false if it fails. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool put_FromEPSG(int epsg_code) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Makes a clone (copy) of this object. </summary>
        ///
        /// <returns>	The clone. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSpatialReferencePtr create_clone() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool        is_Empty() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool         is_Geographic() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool         is_DerivedGeographic() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool         is_Projected() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool         is_Geocentric() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool        is_Dynamic() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool         is_Local() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int         is_Vertical() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>	
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool         is_Compound() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Create a new spatial reference that is initialized to wgs84. </summary>
        ///
        /// <param name="interface_version">	(optional) the interface version. </param>
        ///
        /// <returns>	The new spatial reference. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static IGW3DSpatialReferencePtr create(unsigned interface_version = GEOWEB3D_INTERFACE_VERSION);
    };

    //SKIP-CODE-BEGIN

    ////transform happens in place
    //bool transform ( IGW3DSpatialReference target, double &x, double &y, double &z);
    //
    //
    //bool SetTarget ( IGW3DSpatialReference target);
    ////transform happens in place
    //bool transform (  double &x, double &y, double &z );

    //SKIP-CODE-END

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> TODO </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DCoordinateTransformation
    {
        virtual ~IGW3DCoordinateTransformation()
        {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSpatialReference* get_Source_SpatialReference() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSpatialReference* get_Target_SpatialReference() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="num_of_points"> TODO </param>
        /// <param name="x"> TODO </param>
        /// <param name="y"> TODO </param>
        /// <param name="z"> TODO </param>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult transform_Points(uint64_t num_of_points, double* x, double* y, double* z) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="xmin"> TODO </param>
        /// <param name="ymin"> TODO </param>
        /// <param name="xmax"> TODO </param>
        /// <param name="ymax"> TODO </param>
        /// <param name="out_xmin"> TODO </param>
        /// <param name="out_ymin"> TODO </param>
        /// <param name="out_xmax"> TODO </param>
        /// <param name="out_ymax"> TODO </param>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult transform_Bounds(
            const double xmin, const double ymin, const double xmax, const double ymax,
            double* out_xmin, double* out_ymin, double* out_xmax, double* out_ymax) = 0;
 
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DCoordinateTransformationPtr get_Inverse() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="source"> TODO </param>
        /// <param name="target"> TODO </param>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static IGW3DCoordinateTransformationPtr create_IGW3DCoordinateTransformation(IGW3DSpatialReferencePtr source, IGW3DSpatialReferencePtr target);
    };
}