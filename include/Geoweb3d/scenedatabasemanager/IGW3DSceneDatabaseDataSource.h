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

/* Primary namespace */
namespace Geoweb3d
{

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Database Data Sources encapsulate path information and availability
    ///           of all associated Database layers. Database Data Sources also will 
    ///           inform the client that it is loaded by its respective drive or invalid. </summary>
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DSceneDatabaseDataSource
    {

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will allow a client to name their 
        ///           datasource and override the default provided name. </summary>
        /// 
        /// <param name="datasource_name"> The new datasource name.      </param>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_DataSourceName( IGW3DStringPtr datasource_name ) = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return the name of the data source. </summary>
        ///
        /// <returns> The database data source's name.              </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_DataSourceName() const = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return the friendly file path to the data source. </summary>
        ///
        /// <remarks> This path is used by clients to display paths
        ///           to an end user. This is not guaranteed as being
        ///           the path being fed to the corresponding engine or
        ///           driver for the corresponding type. Some friendly
        ///           names and regular names may be the same, but 
        ///           like said above, there's no guarantee. Use the
        ///           the normal path when interfacing with back-end
        ///           loading code and the friendly one for the front-end
        ///           displays.                                                   </remarks>
        /// 
        /// <returns> The database data source's filepath.                        </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_DataSourceFriendlyFilePath() const = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return the file path to the data source. </summary>
        ///
        /// <remarks> This path is used by clients to load them into 
        ///           their respective Raster/Vector/Scene Drivers.      </remarks>
        /// 
        /// <returns> The database data source's filepath.               </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_DataSourceFilePath() const = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return the name of the driver this data source is associated with. </summary>
        ///
        /// <returns> The associated driver's name.                                                </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_SupportedTypeName() const = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will be used in conjunction with the Scene Database Manager
        ///           front-end to mark this data source as being made available to clients. </summary>
        /// 
        /// <param name="is_available"> Will modify the availability of this 
        ///                             data source for certain clients.                     </param>
        /// 
        /// <remarks> Availability is a distinction for certain clients to "see"
        ///           this datasource. Some clients may override this to be on if
        ///           they see this datasource is still loaded, even if made unavailable.    </remarks>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_IsAvailable( bool is_available ) = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return whether or not this data source is
        ///           made available for clients who care about this information. </summary>
        ///
        /// <remarks> This does not imply that the data source is loaded if true. </remarks>
        /// 
        /// <returns> The database data source's availability to clients.         </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsAvailable() const = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return whether or not this data
        ///           source's file path still exists on disk or not.             </summary>
        ///
        /// <remarks> Invalid data sources usually imply the location on disk to
        ///           this data source's path does not exist or is inaccessible. 
        ///           While a data source is invalid it may not be loaded or 
        ///           its availability toggled by clients. It may be removed
        ///           from its parental data source collection though.            </remarks>
        /// 
        /// <returns> The database data source's validity to clients.             </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsValid() const = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will be used in conjunction with the FV Overlay Option Pages'
        ///           front-end to mark this data source as being loaded by the engine SDK. </summary>
        /// 
        /// <param name="is_loaded"> Will modify the loaded state of this 
        ///                          data source for certain clients.                       </param>
        /// 
        /// <remarks> Loaded will generally imply that this DataSource
        ///           is currently loaded by the engine SDK.                                </remarks>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_IsLoaded( bool is_loaded ) = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return whether or not this data source is
        ///           currently loaded into the scene by its associated driver. </summary>
        ///
        /// <returns> The database data source's loaded state to clients.       </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsLoaded() const = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will be used in conjunction with the FV Overlay Option Pages'
        ///           front-end to display extents for this datasource by the engine SDK. </summary>
        /// 
        /// <param name="are_extents_shown"> Will modify the shown extents state of
        ///                                  this data source for certain clients.        </param>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_AreExtentsShown( bool are_extents_shown ) = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return whether or not this data source's extents
        ///           are currently loaded into the scene by its associated driver. </summary>
        ///
        /// <returns> The database data source's extents loaded state to clients.   </returns>
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_AreExtentsShown() const = 0;

    };
}