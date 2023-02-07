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
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{
    namespace VirtualSystems
    {

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="filename"> TODO </param>
        /// <param name="pdata"> TODO </param>
        /// <param name="pdata_length"> TODO </param>
        /// <param name="take_pdata_ownership"> TODO </param>
        ///
        /// <returns> TODO </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        GW3D_DLL GW3DResult IGW3DCreateInMemoryFile(const char* filename, unsigned char* pdata, int64_t pdata_length, bool take_pdata_ownership);
    };

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> A callback object to be used in conjunction with IGW3DVectorDataSource::create_Copy. </summary>
	///
	/// <remarks> This callback will be invoked during streaming operations when IGW3DVectorDataSource::create_Copy
	///           is creating the features from the source VectorDataSource's VectorLayerCollection to the copied
	///           VectorDatasource's VectorLayerCollection. This information is useful if a client needs to
	///           re-represent the destination's features and needs to know the mappings from source to destination.  </remarks>
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DVectorDataSourceCopyCallback : public IGW3DCallback
	{

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> An enumeration used to determine if the user wants to keep the copied vector data source around or to close it. </summary>
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		enum CopyDataSourceCallbackReturnType
		{
		    /// Close this VectorDataSource after copying completes.
		    DISCARD = 0,
		    /// Keep this VectorDataSource open after copying completes.
		    KEEP = 1
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> During the IGW3DVectorDataSource::create_Copy this will inform the client a vector datasource was created. </summary>
		/// 
		/// <remarks> This vector datasource will have all of its layers and features copied on to it. Each feature will have 
		///           encoded which source_vector_datasource's feature as <"SOURCE_ID", int FID>. Using this information will
		///           allow for a client to re-represent the features on the exported_vector_datasource by looping over the 
		///           source_vector_datasource for the FID and getting that PropertyCollection and applying it to the
		///           exported_vector_datasource's feature for its FID. The rationale behind expanding the AttributeDefinition
		///           is that it gives the client more flexibility as to WHEN they want to represent something, rather than
		///           forcing them to do so immediately during this callback, although the interface provides ease of use to
		///           facilitate such a usage pattern. This can be necessary, especially if the source_vector_datasource 
		///           contains layers who've deleted features and may have "skipped" indices. </remarks>
		/// 
		/// <param name="source_vector_datasource"> The original vector data source that was copied from.
		///                                         Provided as a convenience to be able to allow for
		///                                         immediate Representation. It is the IGW3DVectorDataSource
		///                                         that invoked create_Copy. </param>
		/// 
		/// <param name="exported_vector_datasource"> The new vector data source that was exported to. </param>
		/// 
		/// <returns> Client-supplied return code. If returning DISCARD, the exported_vector_datasource will be immediately
		///           closed after this is invoked. If KEEPing, the datasource will be left in an open state and give the client
		///           a chance to take ownership of it. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual CopyDataSourceCallbackReturnType OnDataSourceCreated( IGW3DVectorDataSourceWPtr& source_vector_datasource, IGW3DVectorDataSourceWPtr& exported_vector_datasource ) = 0;
	};

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> TODO </summary>
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DVectorDataSourceSummary
    {
        ~IGW3DVectorDataSourceSummary() {}

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <returns> TODO </returns>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DStringCollection* get_LayerNamesCollection() = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> A base datasource (can be vector or raster for example. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DDataSource 
    {

      ////////////////////////////////////////////////////////////////////////////////////////////////////
      /// <summary> TODO </summary>
      ///
      /// <returns> TODO </returns>
      ////////////////////////////////////////////////////////////////////////////////////////////////////

      virtual IGW3DStringCollectionPtr get_MetadataDomainCollection() const = 0;

      ////////////////////////////////////////////////////////////////////////////////////////////////////
      /// <summary> TODO </summary>
      ///
      /// <param name="domain"> TODO </param>
      ///
      /// <returns> TODO </returns>
      ////////////////////////////////////////////////////////////////////////////////////////////////////

      virtual IGW3DPropertyCollectionPtr get_MetadataDomainProperties(const char* domain) const = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary> A vector datasource. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DVectorDataSource : public IGW3DDataSource
    {
        virtual ~IGW3DVectorDataSource() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the name. </summary>
        ///
        /// <remarks>	Gets the name of the datasource (what was used to open or create). </remarks>
        ///
        /// <returns>	the name. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const char* get_Name() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the driver. </summary>
        ///
        /// <remarks>	Vector driver the datasource was created or opened with. </remarks>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDriverWPtr get_Driver() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the driver properties used to open the datasource. </summary>
        ///
        /// <remarks>	If you didn't supply properties, the defaults will return.  If no default properties
        ///				this will be null. </remarks>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DPropertyCollection* get_DriverProperties() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the vector data source collection. </summary>
        ///
        /// <remarks>	The collection this datasource belongs to (this is the same collection returned by
        /// 			the GW3DVectorDriver). </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	the vector data source collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorDataSourceCollection* get_VectorDataSourceCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the vector layer collection. </summary>
        ///
        /// <remarks>	Gets the collection of all vector layers that were created or opened using this
        /// 			driver. </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	The vector layer collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorLayerCollection* get_VectorLayerCollection(bool reset_next_iterator = false) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Executes an SQL command operation. </summary>
        ///
        /// <param name="statement">	The statement. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult ExecuteSQLCommand(const char* statement) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Executes a SQL operation that results in a new layer. </summary>
        ///
        /// <param name="statement">	The statement. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult ExecuteSQLLayerCreator(const char* statement) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	A macro that defines creating a layer is supported by the driver. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

#define isCreateLayerSupported       "CreateLayer" //True if this datasource can create new layers.

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A macro that defines deleting a layer supported by the driver. </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

#define isDeleteLayerSupported       "DeleteLayer" //True if this datasource can delete existing layers.

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if driver supports a given capability. </summary>
		///
		/// <param name="isSupported">	The capability to query. </param>
		///
		/// <returns>	true if capable, false if not capable. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool get_IsCapable(const char* isSupported) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Copies a datasource to the destination, using the driver supplied as the target. </summary>
		///
		/// <remarks> If converting to an in-memory export driver, the destination_name is more or less unused. </remarks>
		///
		/// <param name="export_driver"> The targeted driver to export to. </param>
		/// <param name="export_driver_properties">	Properties to be used with the supplied export driver. (Currently unused) </param>
		/// <param name="destination_name">	The path and filename to save persistent Data Sources. </param>
		/// <param name="copy_callback"> The callback object used during copying. 
		///                              Note that this callback is only used if the source driver is a Geoweb3d_DataSource. </param> 
		///
		/// <returns> true if the copy operation completed properly, false if it's unsupported or failed. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		
		virtual bool create_Copy( IGW3DVectorDriverWPtr export_driver, IGW3DPropertyCollection *export_driver_properties, const char* destination_name, IGW3DVectorDataSourceCopyCallback* copy_callback = nullptr ) = 0;

    };
}