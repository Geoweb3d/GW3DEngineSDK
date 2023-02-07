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
#include "Geoweb3d/common/GW3DCommon.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> The Scene Database SDK Search callbacks.           </summary>
    ///
    /// <remarks> This callback reports when searches are begun 
    ///           and in progress within the Scene Database SDK.
    ///           Assume for each search, there will be one callback
    ///           to track the search.                               </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DWThreadedSceneDatabaseSDKSearchCallbacks : public IGW3DCallback
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> While a search is active, all files found during the search,
        ///           regardless if they're a match for the type, will be reported
        ///           back to the client via this interface. This is used to show that
        ///           the search is active and the clients can poll this as often as
        ///           they need or ignore callbacks to this if too many pump per second.     </summary>
        /// 
	    /// <param name="last_found_path"> The most recent path found in the current search. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

	    virtual void OnActiveSearchPathChanged( IGW3DStringPtr last_found_path ) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
	    /// <summary> This is invoked whenever a search is:
        ///               1. Started
        ///               2. Completed
        ///               3. Canceled                                    </summary>
        ///
        /// <remarks> The ERROR value will be implied on a cancel due to
        ///           the fact the results are discarded and not usable. </remarks>
        /// 
	    /// <param name="SearchStatus"> The current state of the search. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

	    virtual void OnSearchStatusChanged( const SearchStatus status ) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This is invoked whenever a search is Completed.          </summary>
        ///
        /// <remarks> This is the only time a client can capture this data, 
        ///           otherwise it's lost. It's very important if you need to 
        ///           compare results from one search with previous results.
        ///           Otherwise, query the Database Manager with the key
        ///           for its complete merged results.                         </remarks>
        ///                                                                    
        /// <param name="search_results"> The results of the search.           </param>
        /// 
        /// <param name="status"> If completed normally, it'll be COMPLETED.
        ///                       If canceled, it'll be CANCELED and the 
        ///                       search_results will be empty.                 </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnSearchCompleted( IGW3DSceneDatabaseDataSourceCollectionPtr search_results, const SearchStatus status ) = 0;

    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> Database Manager which encapsulates all of the handling 
    ///           of a Databases' DataSources and collections.
    ///           This is the main client interface.                      </summary>
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct IGW3DSceneDatabaseManager
    {

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will return the file path to the current persistent disk location. </summary>
        ///
        /// <remarks> The searched directories and differing IGW3DRaster/VectorDrivers
        ///           will differentiate themselves as unique keys to avoid overwriting 
        ///           other searches.                                                         </remarks>
        /// 
        /// <returns> The database driver's persistent disk location.                         </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringPtr get_ManagerFilePath() const = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability 
        ///           to loop over a single database data source collection.   </summary>
        /// 
        /// <param name="supported_type"> The supported type key.              </param>
        /// 
        /// <remarks> A valid supported_type must be queried from:
        ///               get_SupportedTypes()
        ///           Some clients will only care about data 
        ///           sources marked as being available and or valid.          </remarks>
        /// 
        /// <returns> This database driver's data 
        ///           source collection to a client.                           </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DSceneDatabaseDataSourceCollectionPtr get_DataSources( IGW3DStringPtr supported_type ) const = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability to create a single
        ///           database data source. It is up to the client to put it in themselves. </summary>
        /// 
        /// <param name="supported_type"> The supported type this datasource represents.    </param>
        /// 
        /// <param name="database_filepath"> The filepath location to this datasource.      </param>
        /// 
        /// <remarks> The user can capture this return if implicitly_put_in_manager is 
        ///           false, otherwise it will immediately deallocate and nothing will
        ///           occur. The client can ignore the return if it properly inserts
        ///           into the database, or use it for local book keeping.                  </remarks>
        /// 
        /// <returns> IGW3DSceneDatabaseDataSourcePtr if the datasource was properly set
        ///           up, else a nullptr.                                                   </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DSceneDatabaseDataSourcePtr create_DataSource( IGW3DStringPtr supported_type
                                                                 , IGW3DStringPtr database_filepath ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability to merge multiple 
        ///           database data sources into this key's data source collection.        </summary>
        /// 
        /// <param name="data_sources"> The data source collection to merge into this key. </param>
        /// 
        /// <remarks> This will add to the collection entries of this supported_type.
        ///           Only some clients may need to use this.
        ///           This will modify the persistent data on disk immediately.
        ///           Pre-existing data source's availability will be unaffected.           </remarks>
        ///
        /// <returns> GW3D_sOk if the the supplied data_sources were properly added.       </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_DataSources( IGW3DSceneDatabaseDataSourceCollectionPtr data_sources ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability to remove multiple 
        ///           database data sources from this driver's data source collection.       </summary>
        /// 
        /// <param name="data_sources"> The data sources to remove from this key.            </param>
        /// 
        /// <remarks> This will remove the matching collection entries from this 
        ///           supported_type. Only some clients may need to use this.
        ///           This will modify the persistent data on disk immediately.
        ///           To "clear" this driver's collection, call this with get_DataSources(). </remarks>
        ///
        /// <returns> GW3D_sOk if the supplied data_sources were properly removed.           </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult RemoveDataSources( IGW3DSceneDatabaseDataSourceCollectionPtr data_sources ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability to modify a single
        ///           database data sources' flags ( e.g. Available, Loaded ) and
        ///           flush the changes to disk which will notify all clients of the change. </summary>
        /// 
        /// <param name="datasource"> The data source whose flags were modified.             </param>
        /// 
        /// <remarks> If the library lazily evaluates that no changes occurred,
        ///           it will not serialize changes to disk.                                 </remarks>
        ///                                                                                  
        /// <returns> GW3D_sOk if the data source was properly updates and serialized.       </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult UpdateDataSource( IGW3DSceneDatabaseDataSourcePtr datasource ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability to modify a multiple
        ///           database data sources' flags ( e.g. Available, Loaded ) and
        ///           flush the changes to disk which will notify all clients of the change.           </summary>
        /// 
        /// <param name="datasource_collection"> The data source collection whose flags were modified. </param>
        /// 
        /// <remarks> If the library lazily evaluates that no changes occurred,
        ///           it will not serialize changes to disk.                                           </remarks>
        ///                                                                                            
        /// <returns> GW3D_sOk if the data source was properly updates and serialized.                 </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult UpdateDataSources( IGW3DSceneDatabaseDataSourceCollectionPtr datasource_collection ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Grabs business rules from the supplied driver to do asynchronous work.
        ///           Clients will receive callbacks while the search completes.
        ///           Clients can cancel the work which will discard the results.                          </summary>
        /// 
        /// <param name="directory_path"> The path to begin the search from. The higher up
        ///                               this directory, the longer the search will likely take.
        ///                               This will make a unique database for the associated 
        ///                               driver with this path. Multiple searches with the same 
        ///                               directory_path will merge new results if changes on disk
        ///                               occurred since the last initialization of the database. 
        ///                               If no results were collected for this path, no persistent 
        ///                               database on disk will be generated.                              </param>
        /// 
        /// <param name="supported_type"> The associated supported type the client is 
        ///                               using for their database search.                                 </param>
        /// 
        /// <param name="merge_results_into_manager"> Whether or not the client wants the results
        ///                                           of the search to implicitly merge into this
        ///                                           associated IGW3DSceneDatabaseManager. This 
        ///                                           is defaulted to true. Specifying false implies
        ///                                           it is the client's responsibility to collect the
        ///                                           IGW3DSceneDatabaseDataSourceCollectionPtr from the
        ///                                           IGW3DWThreadedSceneDatabaseSDKSearchCallbacks::OnSearchCompleted
        ///                                           callback and call put_DataSource on an instance 
        ///                                           of a IGW3DSceneDatabaseManager. If this is 
        ///                                           true, the results will be merged into the 
        ///                                           IGW3DSceneDatabaseManager prior to callbacks firing,
        ///                                           in the event the client only wants to query for 
        ///                                           the entire database, and not just what was searched. </param>
        /// 
        /// <remarks> All new Data Source results are marked as unavailable by default.
        ///           If the search criteria finds data that already existed, it will
        ///           persist that pre-existing data_source's current availability.
        ///           This work will be done in another thread and clients can check
        ///           this driver's current state to determine what to do next via callbacks.               </remarks>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult SearchDirectoryThreaded( IGW3DStringPtr directory_path, IGW3DStringPtr supported_type, 
            IGW3DWThreadedSceneDatabaseSDKSearchCallbacks* search_callbacks, bool merge_results_into_manager = true ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability to cancel a search
        ///           that's begun for the given starting directory_path and type.            </summary>
        /// 
        /// <param name="directory_path"> The directory_path SearchDirectory was called with. </param>
        /// 
        /// <param name="supported_type"> The supported_type SearchDirectory was called with. </param>
        /// 
        /// <remarks> This will remove the matching collection entries from this driver.
        ///           Only some clients may need to use this.
        ///           This will modify the persistent data on disk immediately.
        ///           To "clear" this driver's collection, call this with get_DataSources().  </remarks>
        ///
        /// <returns> GW3D_sOk if the search was properly canceled.                           </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult CancelSearch( IGW3DStringPtr directory_path, IGW3DStringPtr supported_type ) = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> This will give the clients the availability to check for changes made
        ///           since the last time this function was called. It should be polled 
        ///           occasionally to ensure that the state of the database manager is
        ///           current and that any client-side's front-end or back-end are also up
        ///           to date with the current state of the database.                           </summary>        
        /// 
        /// <remarks> This manager will internally maintain its state properly such that
        ///           by the time a client queries for the type, it will be current with 
        ///           any changes that may have occurred.                                       </remarks>
        ///
        /// <returns> A IGW3DStringCollectionPtr containing which supported_types were changed. </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DStringCollectionPtr CheckForDatabaseChanges() = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Clients can use this function to diff their local copy of datasources 
        ///           that they have a snapshot of. In the event the client is listening to 
        ///           callbacks for a change in the database, it's not guaranteed that a
        ///           client may have a snapshot per change updating properly. This function
        ///           will allow the client to determine easily what was removed, added, or
        ///           updated from their local snapshot.                                           </summary>
        /// 
        /// <remarks> It's required that all of the datasource collections are allocated properly. </remarks>
        /// 
        /// <param name="client_datasource_collection_to_diff"> The client's current snapshot.     </param>
        /// 
        /// <param name="removed_from_collection"> What is in the client's snapshot, 
        ///                                        but not the database.                           </param>
        /// 
        /// <param name="added_to_collection"> What is not in the client's snapshot, 
        ///                                    but is in the database.                             </param>
        /// 
        /// <param name="updated_in_collection"> What is in both the client's snapshot 
        ///                                      and the database still.                           </param>
        /// 
        /// <param name="supported_type"> An optional filter to specify if the diff is
        ///                               based on the entire database's datasource collections,
        ///                               or just a particular type. Note if forgotten about,
        ///                               the removed_from_collection and added_to_collection
        ///                               will report additional erroneous results if you as a
        ///                               client truly wanted a single type to be diffed against.  </param>
        ///
        /// <returns> GW3D_sOk if the diff was properly completed.                                 </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult DiffDatabase( IGW3DSceneDatabaseDataSourceCollectionPtr client_datasource_collection_to_diff
                                       , IGW3DSceneDatabaseDataSourceCollectionPtr removed_from_collection
                                       , IGW3DSceneDatabaseDataSourceCollectionPtr added_to_collection
                                       , IGW3DSceneDatabaseDataSourceCollectionPtr updated_in_collection
                                       , IGW3DStringPtr                            supported_type = IGW3DStringPtr() ) = 0;

    };
}