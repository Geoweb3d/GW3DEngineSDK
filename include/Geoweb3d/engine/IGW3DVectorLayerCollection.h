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
#include "../../Geoweb3dCore/Geoweb3dTypes.h"
#include "IGW3DVectorLayerStreamFilter.h"

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DFeatureObjectLoader
	{
		virtual ~IGW3DFeatureObjectLoader() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="objectid"></param>
		/// <param name="geom"></param>
		/// <param name="field_values"></param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual Geoweb3d::GW3DResult add_Feature(long objectid, const GW3DGeometry* geom, const Geoweb3d::IGW3DPropertyCollectionPtr& field_values) = 0;
	};

	struct  GW3D_DLL IGW3DVectorPipelineCallbacks
	{
		virtual ~IGW3DVectorPipelineCallbacks() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Layer callback supplying an exposed envelope of the layer.  </summary>
		///
		/// <remarks> Will be called if IGW3DVectorLayerAssistedPagingPolicy was NOT used when representing the layer.
		///			
		///			this will call back on many threads!!!!!! Use thread safety if you need it.  Also, if you take a long time here,
		///			 you will slow up loading for the whole 3d system.  Also note this api will constantly get called and will
		///			 request the same bounds again if the user moved back to this area. </remarks>
		///
		/// <param name="layer">  The layer that is being paged in.  </param>
		/// <param name="bounds"> Bounds of layer that is being exposed.  </param>
		/// <param name="loader"> Loader that handles new feature ids to be represented. 
		///							Note: If layer is not represented this feature is disabled. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_VectorPipeline_get_FeatureObjectsInBounds(const Geoweb3d::IGW3DVectorLayerWPtr layer, const GW3DEnvelope& bounds, IGW3DFeatureObjectLoader* loader) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Layer callback supplying an exposed envelope of the layer.  </summary>
		///
		/// <remarks> Will be called if IGW3DVectorLayerAssistedPagingPolicy was NOT used when representing the layer.
		///			
		///			this will call back on many threads!!!!!! Use thread safety if you need it.  Also, if you take a long time here,
		///			 you will slow up loading for the whole 3d system.  Also note this api will constantly get called and will
		///			 request the same bounds again if the user moved back to this area. </remarks>
		///
		/// <param name="layer">  The layer that is being paged in.  </param>
		/// <param name="bounds"> Bounds of layer that are no longer exposed.  </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_VectorPipeline_remove_FeatureObjectsInBounds(const Geoweb3d::IGW3DVectorLayerWPtr layer, const GW3DEnvelope& bounds) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Layer callback supplying feature ids that were exposed (defined by the IGW3DVectorLayerAssistedPagingPolicy), and anticipating user supplied features. </summary>
		///
		/// <remarks>Will be called if a IGW3DVectorLayerAssistedPagingPolicy was used when representing the layer
		///
		///			this will call back on many threads!!!!!! Use thread safety if you need it.  Also, if you take a long time here,	 
		///			 you will slow up loading for the whole 3d system.  Also note this api will constantly get called and will
		///			 request the same bounds again if the user moved back to this area. </remarks>
		///
		/// <param name="layer">  The layer that is being paged in.  </param>
		/// <param name="pagedInFeaturesIds">  The feature ids exposed (defined by IGW3DVectorLayerAssistedPagingPolicy layer) </param>
		/// <param name="loader">  Loader that handles new feature ids to be represented.  </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_VectorPipeline_get_FeatureIDs(const Geoweb3d::IGW3DVectorLayerWPtr layer, Geoweb3d::IGW3DVectorLayerStreamFilter* pagedInFeaturesIds, IGW3DFeatureObjectLoader* loader) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns>	The envelope of the layer.  </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DEnvelope GW3DTHREADED_VectorPipeline_get_GetLayerBounds() = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DGeofencePipelineCallbacks
	{
		virtual ~IGW3DGeofencePipelineCallbacks() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Layer callback supplying an exposed envelope of the layer.  </summary>
		///
		/// <remarks> Will be called if IGW3DVectorLayerAssistedPagingPolicy was NOT used when representing the layer.
		///			
		///			this will call back on many threads!!!!!! Use thread safety if you need it.  Also, if you take a long time here,
		///			 you will slow up loading for the whole 3d system.  Also note this api will constantly get called and will
		///			 request the same bounds again if the user moved back to this area. </remarks>
		///
		/// <param name="layer">  The layer that is being paged in.  </param>
		/// <param name="bounds"> Bounds of layer that is being exposed.  </param>
		/// <param name="loader"> Loader that handles new feature ids to be represented. 
		///							Note: If layer is not represented this feature is disabled. </param>
		/// <param name="user_data"> user specific data </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_GeoFencePipeline_get_FeatureObjectsInBounds(const Geoweb3d::IGW3DVectorLayerWPtr layer, const GW3DEnvelope& bounds, IGW3DFeatureObjectLoader* loader, IGW3DClientUserDataPtr user_data) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Layer callback supplying an exposed envelope of the layer.  </summary>
		///
		/// <remarks> Will be called if IGW3DVectorLayerAssistedPagingPolicy was NOT used when representing the layer.
		///			
		///			this will call back on many threads!!!!!! Use thread safety if you need it.  Also, if you take a long time here,
		///			 you will slow up loading for the whole 3d system.  Also note this api will constantly get called and will
		///			 request the same bounds again if the user moved back to this area. </remarks>
		///
		/// <param name="layer">  The layer that is being paged in.  </param>
		/// <param name="bounds"> Bounds of layer that are no longer exposed.  </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_GeoFencePipeline_remove_FeatureObjectsInBounds(const Geoweb3d::IGW3DVectorLayerWPtr layer, const GW3DEnvelope& bounds) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Layer callback supplying feature ids that were exposed (defined by the IGW3DVectorLayerAssistedPagingPolicy), and anticipating user supplied features. </summary>
		///
		/// <remarks>Will be called if a IGW3DVectorLayerAssistedPagingPolicy was used when representing the layer
		///
		///			this will call back on many threads!!!!!! Use thread safety if you need it.  Also, if you take a long time here,	 
		///			 you will slow up loading for the whole 3d system.  Also note this api will constantly get called and will
		///			 request the same bounds again if the user moved back to this area. </remarks>
		///
		/// <param name="layer">  The layer that is being paged in.  </param>
		/// <param name="pagedInFeaturesIds">  The feature ids exposed (defined by IGW3DVectorLayerAssistedPagingPolicy layer) </param>
		/// <param name="loader">  Loader that handles new feature ids to be represented.  </param>
		/// <param name="data"> user specific data </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_GeoFencePipeline_get_FeatureIDs(const Geoweb3d::IGW3DVectorLayerWPtr layer, Geoweb3d::IGW3DVectorLayerStreamFilter* pagedInFeaturesIds, IGW3DFeatureObjectLoader* loader, IGW3DClientUserDataPtr data) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="grid_bounds"> TODO </param>
		/// <param name="data"> TODO </param>
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_GeoFencePipeline_put_IntruderGridBounds(const Geoweb3d::GW3DEnvelope& grid_bounds, IGW3DClientUserDataPtr data) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="vp_bounds"> TODO </param>
		/// <param name="data"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3DTHREADED_GeoFencePipeline_put_IntruderViewBounds(const Geoweb3d::GW3DEnvelope& vp_bounds, IGW3DClientUserDataPtr data) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult GW3D_GeoFencePipeline_validate_state() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DEnvelope GW3DTHREADED_GeoFencePipeline_get_GetLayerBounds() = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Collection of vector layers. </summary>
	///
	/// <remarks>	Geoweb3d, 11/1/2012. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DVectorLayerCollection : public IGW3DCollection< IGW3DVectorLayerWPtr >
	{
		virtual ~IGW3DVectorLayerCollection() {}

#ifndef SWIGGING

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Array indexer operator. </summary>
		///
		/// <param name="index">	Zero-based index of the vector layer. </param>
		///
		/// <returns>	The indexed value. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DVectorLayerWPtr operator[](unsigned long index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets at index. </summary>
		///
		/// <param name="index">	Zero-based index of the vector layer. </param>
		///
		/// <returns>	at index. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DVectorLayerWPtr get_AtIndex(unsigned long index) const = 0;

#endif //SWIGGING

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Destroys the given layer. </summary>
		///
		/// <param name="layer">	The layer to destroy. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual  GW3DResult destroy(IGW3DVectorLayerWPtr layer) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new layer. </summary>
		///
		/// <param name="name">			   	The layer's name. </param>
		/// <param name="geometry_type">   	Type of the geometry of features in this layer. </param>
		/// <param name="field_parameters">	the field definition for the attributes of features in this layer. </param>
		/// <param name="layer_create_properties">	layer creation properties are queried off the driver IGW3DVectorDriver
		///											which you can get through the IGW3DVectorDatasource </param>
		///
		/// <returns>	The new layer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DVectorLayerWPtr create(const char* name, const GeometryType geometry_type, const Geoweb3d::IGW3DDefinitionCollectionPtr& field_parameters, const IGW3DPropertyCollectionPtr& layer_create_properties = IGW3DPropertyCollectionPtr()) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new layer. </summary>
		///
		/// <param name="name">			   	The layer's name. </param>
		/// <param name="geometry_type">   	Type of the geometry of features in this layer. </param>
		/// <param name="field_parameters">	the field definition for the attributes of features in this layer. </param>
		/// <param name="layer_create_properties">	   	(future optional) layer creation properties, currently unused. </param>
		///
		/// <returns>	The new layer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DVectorLayerWPtr create(const char* name, const GeometryType geometry_type, const Geoweb3d::IGW3DDefinitionCollection* field_parameters, const IGW3DPropertyCollection* layer_create_properties) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new proxy layer. </summary>
		///
		/// <remarks>	A proxy layer allows data to be queried out of a service or external datasource on
		///				demand in multiple threads, via the IGW3DProxyLayerCallbacks.</remarks>
		///
		/// <param name="name">			   	The name. </param>
		/// <param name="geometry_type">   	Type of the geometry of features in this layer. </param>
		/// <param name="field_parameters">	the field definition for the attributes of features in this layer. </param>
		/// <param name="vector_cb"> TODO </param>
		/// <param name="geofenc_cb"> TODO </param>
		///
		/// <returns>	The new proxy layer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DVectorLayerWPtr create_Proxy(const char* name, const GeometryType geometry_type, const Geoweb3d::IGW3DDefinitionCollectionPtr& field_parameters, IGW3DVectorPipelineCallbacks* vector_cb = nullptr, IGW3DGeofencePipelineCallbacks* geofenc_cb = nullptr) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new proxy layer. </summary>
		///
		/// <remarks>	A proxy layer allows data to be queried out of a service or external datasource on
		///				demand in multiple threads, via the IGW3DProxyLayerCallbacks.</remarks>
		///
		/// <param name="name">			   	The name. </param>
		/// <param name="geometry_type">   	Type of the geometry of features in this layer. </param>
		/// <param name="field_parameters">	the field definition for the attributes of features in this layer. </param>
		/// <param name="vector_cb"> TODO </param>
		/// <param name="geofenc_cb"> TODO </param>
		///
		/// <returns>	The new proxy layer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DVectorLayerWPtr create_Proxy(const char* name, const GeometryType geometry_type, const Geoweb3d::IGW3DDefinitionCollection* field_parameters, IGW3DVectorPipelineCallbacks* vector_cb = nullptr, IGW3DGeofencePipelineCallbacks* geofenc_cb = nullptr) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a layer by name. </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	The layer, or an expired pointer if no layer matches the given name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DVectorLayerWPtr get_ByName(const char* name) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the vector data source. </summary>
		///
		/// <remarks>	Gets the vector datasource this layer collection belongs to. </remarks>
		///
		/// <returns>	The vector data source. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DVectorDataSourceWPtr get_VectorDataSource() = 0;

	};
}