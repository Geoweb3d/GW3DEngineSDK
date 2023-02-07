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
#include "GW3DGuids.h"
#include "GW3DInterFace.h"
#include "GW3DSceneDatabase.h"

namespace Geoweb3d
{

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> An interface for searching and indexing CDB, TTVS, and scene files.              </summary>
    ///
    /// <remarks> The Geoweb3d Scene Data Management can be expanded to support more search types. </remarks>
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DSceneDatabaseSDK : public IGW3DIBaseObject
    {

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initialize the Geoweb3d Scene Data Management Library </summary>
        ///
        /// <remarks> This must be called before calling any other APIs. </remarks>
        ///
        /// <param name="sdk_context">			The Geoweb3d SDK context.  </param>
        /// <param name="productversion_major">	The major product version. </param>
        /// <param name="productversion_minor">	The minor product version. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult InitializeLibrary( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, 
                                              int productversion_major, 
                                              int productversion_minor ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Gets the database manager from this context for the specified disk path.      </summary>
        ///
        /// <remarks> If the manager wasn't initialized by a client yet, it will do so
        ///           and return it to the client. It is not the client's responsibility
        ///           to manage the memory. If databases exist from previous client searches,
        ///           they will be implicitly retrievable immediately. Clients can begin
        ///           multiple directory searches on one manager, but their results will
        ///           be stored at this database_path. Users can create multiple managers
        ///           and merge DatabaseDataSourceCollections from one database manager to another,
        ///           if they so choose. Normally, clients will only need one manager at a time.
        /// 
        ///           Optionally takes a IGW3DWThreadedSceneDatabaseSDKFileChangeCallbacks
        ///           so a client can listen to changes in the event multiple clients
        ///           need to communicate changes to one another or something on
        ///           disk that was persisted was modified. If a client doesn't
        ///           care about that information, they can omit deriving their own
        ///           callback and supplying it.                                                    </remarks>
        /// 
        /// <param name="database_path"> The manager for this database path.                        </param>
        /// 
        /// <returns> The Database manager held by this context for the path.                       </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSceneDatabaseManagerPtr get_Manager( IGW3DStringPtr database_path ) = 0;
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Returns the supported types to the client.     </summary>
        ///           
        /// <remarks> These types are to be used in conjunction 
        ///           with the IGW3DSceneDatabaseManager.            </remarks>
        ///           
        /// <returns> The a string collection of iterable supported 
        ///           types to use as support type keys.             </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringCollectionPtr get_SupportedTypes() = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates the Geoweb3d Scene Database SDK interface. </summary>
        ///           
        /// <remarks> Creates an instance of the Geoweb3d Scene Database SDK. </remarks>
        ///           
        /// <returns> The new interface. </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        static IGW3DSceneDatabaseSDKPtr CreateInterface()
        {
            return CreateSceneDatabaseInterfacePointer< IGW3DSceneDatabaseSDKPtr, IGW3DSceneDatabaseSDK>( GUID_GEOWEB3D_SCENEDATABASE_SDK, /*pcallback*/ 0 );
        }

    };
}