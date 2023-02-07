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
#include "../core/IGW3DLayer.h"
#include "IGW3DVectorLayerStream.h"
#include "IGW3DVectorRepresentationCollectionViewer.h"
#include "../../Geoweb3dCore/GeometryExports.h"
#include "GW3DRasterizorParameters.h"

/* Primary namespace */
namespace Geoweb3d
{

	struct IGW3DProgress;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	A vector layer. </summary>
    ///
    /// <remarks>	A vector layer, which contains records (features) that contain geometry and 
    ///				a set of attributes that describe the geographic feature. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorLayer : public IGW3DLayer
    {
        virtual ~IGW3DVectorLayer() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="cb"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_ProxyLayerCallbacks(IGW3DVectorPipelineCallbacks* cb) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="cb"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_ProxyLayerCallbacks(IGW3DGeofencePipelineCallbacks* cb) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Streams the given IGW3DVectorLayerStream. </summary>
        ///
        /// <remarks>	This function will trigger the SDK to call the callbacks on the given 
        ///				IGW3DVectorLayerStream using its iterator.  If the IGW3DVectorLayerStream returns 
        ///				0 for count(), then all features will be streamed.  Otherwise, you may use the 
        ///				IGW3DVectorLayerStream to filter what features are streamed. </remarks>
        ///
        /// <param name="streamobj">	[in] If non-null, the streamobj. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void Stream(IGW3DVectorLayerStream* streamobj) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets if geometry within the IGW3DVectorLayerStream is editable </summary>
        ///
        /// <remarks>	If the layer is in editable mode, then the geometry given in
        ///				IGW3DVectorLayerStreamResult::get_EditableGeometry may be modified in order to edit 
        ///				the feature geometry. </remarks>
        ///
        /// <param name="bool">	is "dynamic data" </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsInGeometryEditableMode() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enables the ability to edit the geometry within the IGW3DVectorLayerStream </summary>
        ///
        /// <remarks>	This is not supported on some representation types, such as extruded polygons and 
        ///				water.
        ///				
        ///				Note, you should NOT enable this if you are only planning on creating new features.
        ///				This mode is ONLY required if modifying the geometry or attributes of existing
        ///				features, i.e. from inside an IGW3DVectorLayerStream.
        ///
        ///				It is also not required (and not recommended) for creating new geometry prior to 
        ///				creating a representation on the layer.</remarks>
        ///
        /// <param name="enabled">	[in] enable/disables "dynamic data" </param>
        /// <param name="commit">	[in] commit existing to "static" data (not currently supported) </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_GeometryEditableMode(bool enabled, bool commit = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	This will return the Envelope in the WGS84 coordinate reference system defined by EPSG:4326.
        ///				</summary>
        ///
        /// <returns>	The envelope. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DEnvelope get_Envelope() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Check to see if the layer is valid to be represented in the 3D scene.
        ///				</summary>
        ///
        ///	<remarks>	Typical error and warning that will be returned is GW3D_eLayerProjectionError (error) and
        ///				GW3D_sProjectionGuessed (warning) </remarks>
        ///
        /// <returns>	The is valid for vector layer representation. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult get_IsValidForVectorRepresentation() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the native spatial reference. </summary>
        ///
        /// <remarks>	Gets the native spatial reference queried from the library that was used to load 
        /// 			the layer (GDAL, ArcObjects, LibLAS, etc.).  If none exists, a null pointer will be
        /// 			returned. </remarks>
        ///
        /// <returns>	The native spatial reference, or a null pointer if none exists. </returns>
        /// 
        /// <see cref="get_SpatialReference"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSpatialReferencePtr get_NativeSpatialReference() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets spatial reference. </summary>
        ///
        /// <remarks>	Gets the spatial reference set via put_SpatialReference or in the driver properties 
        /// 			when opening the datasource. </remarks>
        ///
        /// <returns>	The spatial reference. </returns>
        /// 
        /// <see cref="get_NativeSpatialReference"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSpatialReferencePtr get_SpatialReference() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the spatial reference of the layer </summary>
        ///
        /// <remarks>	Sets the vector layer's spatial reference.  Note that this does not reproject the 
        /// 			data or alter it in any way.  If the data is not in WGS84 geographic, it is 
        /// 			required to have a spatial reference to ensure it is possible to represent the
        /// 			layer in the 3D scene.
        /// 			
        /// 			Note that if the spatial reference is already defined by the layer itself, it may 
        /// 			not be necessary to use this API.  Validate a spatial reference is needed by 
        /// 			checking what is returned by get_IsValidForVectorRepresentation().  If the result 
        /// 			returned is GW3D_eLayerProjectionError, then the spatial reference is missing, 
        /// 			incorrect, or unsupported.  If no error is returned, you should typically not use 
        /// 			put_SpatialReference as you may cause the SDK to store extra, duplicate data.
        /// 			</remarks>
        ///
        /// <param name="spatialref">	The spatial reference. </param>
        /// 
        /// <see cref="get_IsValidForVectorRepresentation"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_SpatialReference(const IGW3DSpatialReferencePtr& spatialref) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets object feature count. </summary>
        ///
        /// <returns>	The object feature count. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned  get_ObjectFeatureCount() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets geometry type. </summary>\
        ///
        /// <returns>	The geometry type. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GeometryType get_GeometryType() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets attribute definition collection. </summary>\
        ///
        /// <returns>	null if it fails, else the attribute definition collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DDefinitionCollection* get_AttributeDefinitionCollection(bool reset_next_iterator = false) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a new feature </summary>
        ///
        /// <remarks>	Returns -1 if it could not create the feature </remarks>
        ///
        /// <param name="field_values">	The field values. </param>
        /// <param name="geom">		   	The geometry. </param>
        /// <param name="retval">		Will be set to GW3D_Ok unless a warning or error occurs. </param>
        ///
        /// <returns>	The new feature id. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual long  create_Feature(const IGW3DAttributeCollection* field_values, const GW3DGeometry* geom, /*out*/ GW3DResult& retval) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a new feature </summary>
        ///
        /// <remarks>	Returns -1 if it could not create the feature </remarks>
        ///
        /// <param name="field_values">	The field values. </param>
        /// <param name="geom">		   	The geometry. </param>
        /// <param name="retval">		Will be set to GW3D_Ok unless a warning or error occurs. </param>
        ///
        /// <returns>	The new feature id. </returns>
        ///
        /// <see cref="force_RepresentationRefresh"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual long  create_Feature(const IGW3DPropertyCollectionPtr& field_values, const GW3DGeometry* geom, /*out*/ GW3DResult& retval) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Delete a feature </summary>
        ///  
        ///	<remarks>	Note that not all drivers support this capability.  Make sure you query if this is
        ///				supported. </remarks>
        ///
        /// <param name="env"> TODO </param>
        ///
        ///	<see cref="IGW3DVectorDriver::get_IsCapable"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult delete_Feature(long feature) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Force all representations to refresh to reflect changes to the data </summary>
        ///  
        ///	<remarks>	If you already have the layer represented, create_Feature's retval will return a 
        ///				warning to call refresh.  After you are done adding all your features, you should 
        ///				call this so the 3D scene will properly page in all its data. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_RepresentationRefresh() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="env"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_RepresentationPropertyUpdate(const GW3DEnvelope& env) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="collection"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_RepresentationPropertyUpdate(const IGW3DIdCollectionPtr& collection) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets vector data source. </summary>
        ///
        /// <returns>	The vector data source. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDataSourceWPtr get_VectorDataSource() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets vector data source. </summary>
        ///
        /// <returns>	The vector data source. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DVectorDataSourceWPtr get_VectorDataSource() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Collection of representations on this layer. </summary>
        ///
        /// <returns>	null if it fails, else the representation layer collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationCollectionViewer* get_RepresentationLayerCollection() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	A macro that defines is random read supported. </summary>
        ///
        /// <remarks>	if the GetFeature() method is implemented in an optimized way for this layer, as 
        ///				opposed to the default implementation using ResetReading() and GetNextFeature() 
        ///				to find the requested feature id. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

#define isRandomReadSupported			"RandomRead"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is sequential write supported. </summary>
///
/// <remarks>	if the CreateFeature() method works for this layer. Note this means that this 
///				particular layer is writable. The same OGRLayer class may returned FALSE for other 
///				layer instances that are effectively read-only. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isSequentialWriteSupported      "SequentialWrite"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is random write supported. </summary>
///
/// <remarks>	if the SetFeature() method is operational on this layer. Note this means that this 
///				particular layer is writable. The same OGRLayer class may returned FALSE for other 
///				layer instances that are effectively read-only. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isRandomWriteSupported          "RandomWrite"	

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is fast spatial filter supported. </summary>
///
/// <remarks>	if this layer implements spatial filtering efficiently. Layers that effectively 
///				read all features, and test them with the OGRFeature intersection methods should 
///				return FALSE. This can be used as a clue by the application whether it should build 
///				and maintain its own spatial index for features in this layer. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isFastSpatialFilterSupported    "FastSpatialFilter"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is fast feature count supported. </summary>
///
/// <remarks>	if this layer can return a feature count (via GetFeatureCount()) efficiently, 
///				i.e. without counting the features. In some cases this will return TRUE until a 
///				spatial filter is installed after which it will return FALSE. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isFastFeatureCountSupported     "FastFeatureCount"	

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is fast get extent supported. </summary>
///
/// <remarks>	if this layer can return its data extent (via GetExtent()) efficiently, i.e. 
///				without scanning all the features. In some cases this will return TRUE until 
///				a spatial filter is installed after which it will return FALSE. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isFastGetExtentSupported        "FastGetExtent"		

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is create field supported. </summary>
///
/// <remarks>	if this layer can perform the SetNextByIndex() call efficiently, otherwise FALSE.
///				</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isCreateFieldSupported          "CreateField"		

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is delete field supported. </summary>
///
/// <remarks>	if this layer can create new fields on the current layer, otherwise FALSE. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isDeleteFieldSupported          "DeleteField"	

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is reorder fields supported. </summary>
///
/// <remarks>	if this layer can delete existing fields on the current layer, otherwise FALSE. 
///				</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isReorderFieldsSupported        "ReorderFields"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is alter field definition supported. </summary>
///
/// <remarks>	if this layer can reorder existing fields on the current layer, otherwise FALSE. 
///				</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isAlterFieldDefnSupported       "AlterFieldDefn"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is transactions supported. </summary>
///
/// <remarks>	if this layer can alter the definition of an existing field on the current layer, 
///				otherwise FALSE. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isTransactionsSupported         "Transactions"	

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is delete feature supported. </summary>
///
/// <remarks>	if the delete_Feature() method is supported on this layer, otherwise FALSE. 
///				</remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isDeleteFeatureSupported        "DeleteFeature"		

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is fast set next by index supported. </summary>
///
/// <remarks>	TRUE if this layer can perform the OGR SetNextByIndex() call efficiently, otherwise 
///				FALSE. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isFastSetNextByIndexSupported   "FastSetNextByIndex"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is strings as UTF 8 supported. </summary>
///
/// <remarks>	if values of OFTString fields are assured to be in UTF-8 format. If FALSE the 
///				encoding of fields is uncertain, though it might still be UTF-8. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isStringsAsUTF8Supported        "StringsAsUTF8"		

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines is ignore fields supported. </summary>
///
/// <remarks>	if fields, geometry and style will be omitted when fetching features as set by 
///				the OGR SetIgnoredFields() method. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

#define isIgnoreFieldsSupported			"IgnoreFields"

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if vector layer supports a given capability. </summary>
        ///
        /// <param name="isSupported">	The capability to query. </param>
        ///
        /// <returns>	true if capable, false if not capable. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsCapable(const char* isSupported) = 0;

        //SKIP-CODE-BEGIN

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	global filter limits streams and representations to be within 
        ///             NOT FULLY SUPPORTED, more of a POC</summary>
        ///
        /// <param name="min_X"> TODO </param>
        /// <param name="min_Y"> TODO </param>
        /// <param name="max_X"> TODO </param>
        /// <param name="max_Y"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_SpatialFilter(double 	min_X, double 	min_Y, double 	max_X, double max_Y) = 0;

        //SKIP-CODE-END

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Creates a rasterization of this layer. </summary>
        ///
        /// <param name="out_file"> TODO </param>
        /// <param name="params"> TODO </param>
        /// <param name="progress_callback"> TODO </param>
        ///
        /// <returns> A Geoweb3d status code. </returns>
        /// 
        /// <see cref="GW3DRasterizorParameters"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult create_Rasterization(const char* out_file, const Vector::GW3DRasterizorParameters& params, IGW3DProgress* progress_callback) = 0;
    };
}