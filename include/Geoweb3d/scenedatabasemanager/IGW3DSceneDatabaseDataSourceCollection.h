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
#include "GW3DInterFace.h"
#include "GW3DSceneDatabaseExports.h"
#include <Geoweb3d/core/IGW3DCollection.h>

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Collection of Scene Database Data Source Objects </summary>
    ///
    /// <remarks> DataSource Collections will be homogeneous for the supported type only. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DSceneDatabaseDataSourceCollection : public IGW3DCollection< IGW3DSceneDatabaseDataSourcePtr >
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Array index operator.                                     </summary>
        ///
        /// <param name="index"> Zero-based index of the database data source.  </param>
        /// 
        /// <remarks> This will return a shared pointer, and as such the client 
        ///           does not need to manage the memory returned by this.      </remarks>
        /// 
        /// <returns> The indexed database data source object.                  </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSceneDatabaseDataSourcePtr operator[]( unsigned long index ) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Gets the database data source at index.                   </summary>
        ///
        /// <param name="index"> Zero-based index of the database layer.        </param>
        ///
        /// <remarks> This will return a shared pointer, and as such the client 
        ///           does not need to manage the memory returned by this.      </remarks>
        /// 
        /// <returns> The database data source object at index.                 </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSceneDatabaseDataSourcePtr get_AtIndex( unsigned long index ) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Pushes a datasource into the collection.     </summary>
        ///
        /// <param name="datasource"> The datasource to push back. </param>
        /// 
        /// <remarks> This operation can fail if the datasource is
        ///           already in this collection, or if the client
        ///           attempts to push_back on a collection they
        ///           don't have permissions for.
        ///           ( e.g. a collection owned by a manager )     </remarks>
        /// 
        /// <returns> GW3D_sOk if the push_back succeeded.         </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult push_back( IGW3DSceneDatabaseDataSourcePtr datasource ) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Removes a datasource from the collection.    </summary>
        ///
        /// <param name="datasource"> The datasource to remove.    </param>
        /// 
        /// <remarks> This operation can fail if the datasource
        ///           is not in this collection, or if the client
        ///           attempts to remove on a collection they
        ///           don't have permissions for.
        ///           ( e.g. a collection owned by a manager )     </remarks>
        /// 
        /// <returns> GW3D_sOk if the remove succeeded.            </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult remove( IGW3DSceneDatabaseDataSourcePtr datasource ) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Allows the client to create their own collection.  </summary>
        ///
        /// <param name="interface_version"> The interface_version.      </param>
        /// 
        /// <remarks> The above push_back and remove functions
        ///           should be used in conjunction with a created
        ///           collection from this interface. Afterwards,
        ///           clients can use a IGW3DSceneDatabaseManager
        ///           to, for example, makes calls to 
        ///           put_DataSources or RemoveDataSources.               </remarks>
        /// 
        /// <returns> The creates IGW3DSceneDatabaseDataSourceCollection
        ///           to the client.                                      </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        static GW3D_SCENE_DATABASE_API IGW3DSceneDatabaseDataSourceCollectionPtr create( unsigned interface_version = GEOWEB3D_INTERFACE_VERSION );

    };

}
