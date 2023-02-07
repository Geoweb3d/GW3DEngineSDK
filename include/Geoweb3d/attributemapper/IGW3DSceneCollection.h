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
#include <Geoweb3d/core/GW3DInterFace.h>
#include <Geoweb3d/core/IGW3DCollection.h>
#include "IGW3DSceneLoadingCallbacks.h"

namespace Geoweb3d
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Collection of Scene Objects </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGW3DSceneCollection : public IGW3DCollection< IGW3DSceneWPtr >
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Array indexer operator. </summary>
	///
	/// <param name="index">	Zero-based index of the vector layer. </param>
	///
	/// <returns>	The indexed value. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual const IGW3DSceneWPtr operator[]( unsigned long index ) const = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets at index. </summary>
	///
	/// <param name="index">	Zero-based index of the vector layer. </param>
	///
	/// <returns>	at index. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual const IGW3DSceneWPtr get_AtIndex( unsigned long index ) const = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Destroys the given scene. </summary>
	///
	/// <param name="scene">	The scene to destroy. </param>
	///
	/// <returns>	GW3D_sOk if succeeded. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual GW3DResult Destroy( IGW3DSceneWPtr scene ) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Loads a new Scene object from a Geoweb3d Project file (.g3proj), a Geoweb3d 
	///           Layer file (.g3lyr), or a Geoweb3d Scene file (.scene). </summary>>
	///
	/// <param name="filename">	The filename. </param>
	/// <param name="cb"> TOTDO </param>
    ///
	/// <returns>	The new scene. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual IGW3DSceneWPtr Load( const char* filename, IGW3DSceneLoadingCallbacks* cb = nullptr) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Creates a .scene file from the data source collection. </summary>
    ///
    /// <remarks> Any Representations on the VectorDataSource will be persisted to disk. 
    ///           If succeeded, you can then Load your file via this class' Load coupled
    ///           with your provided filename. </remarks>
    ///
    /// <param name="data_source_collection"> The VectorDataSources to persist to disk. </param>
    ///
    /// <param name="filename">	The filename to save the persisted "filename".scene as. </param>
    ///
    /// <returns> GW3D_sOk if succeeded. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Geoweb3d::GW3DResult Create( Geoweb3d::IGW3DVectorDataSourceCollection* data_source_collection, const char* filename ) = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Creates a .scene file from the data source collection. </summary>
    ///
    /// <remarks> Any Representations on the VectorDataSource will be persisted to disk. 
    ///           If succeeded, you can then Load your file via this class' Load coupled
    ///           with your provided filename. </remarks>
    ///
    /// <param name="data_source"> The VectorDataSource to persist to disk. </param>
    ///
    /// <param name="filename">	The filename to save the persisted "filename".scene as. </param>
    ///
    /// <returns> GW3D_sOk if succeeded. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Geoweb3d::GW3DResult Create( Geoweb3d::IGW3DVectorDataSourceWPtr data_source, const char* filename ) = 0;

};

}
