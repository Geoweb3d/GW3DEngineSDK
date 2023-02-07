#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/GW3DVector.h"
#include "engine/IGW3DVectorDataSource.h"
#include "engine/IGW3DStringCollection.h"
#include "common/GW3DCommon.h"

#include "Geoweb3d/attributemapper/IGW3DScene.h"
#include "Geoweb3d/attributemapper/IGW3DSceneCollection.h"

#include "GeoWeb3dCore/SystemExports.h"

#include <tchar.h>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>

#pragma comment( lib, "GW3DEngineSDK.lib" )
#pragma comment( lib, "GW3DCommon.lib" )


void my_fatal_function( const char* msg );
void my_info_function( const char *msg );
void SetInformationHandling();

struct ExampleGW3DVectorDataSourceCopyCallback : public Geoweb3d::IGW3DVectorDataSourceCopyCallback
{

    class RepresentationPropertyCopyStreamer : public Geoweb3d::IGW3DVectorLayerStream
    {
    public:

        RepresentationPropertyCopyStreamer( Geoweb3d::IGW3DVectorLayerWPtr& source_vector_layer, Geoweb3d::IGW3DVectorLayerWPtr& exported_vector_layer )
            : source_vector_layer_  ( source_vector_layer )
            , exported_vector_layer_( exported_vector_layer )
        {
        }

        virtual bool OnStream( Geoweb3d::IGW3DVectorLayerStreamResult* result )
        {
            bool successful = false;

            if ( source_vector_layer_.expired() || exported_vector_layer_.expired() ) return successful;

            long source_fid = result->get_AttributeCollection()->get_Property( result->get_AttributeCollection()->get_DefinitionCollection()->get_IndexByName( "SOURCE_ID" ) ).to_int();
            long export_fid = result->get_ObjectID();

            for ( unsigned long i = 0; i < source_vector_layer_.lock()->get_RepresentationLayerCollection()->count(); ++i )
            {
                Geoweb3d::IGW3DVectorRepresentationWPtr source_vector_representation = source_vector_layer_.lock()->get_RepresentationLayerCollection()->get_AtIndex( i );
                Geoweb3d::IGW3DVectorRepresentationWPtr exported_vector_representation = exported_vector_layer_.lock()->get_RepresentationLayerCollection()->get_AtIndex( i );

                if ( source_vector_representation.expired() || exported_vector_representation.expired() ) continue;

                exported_vector_representation.lock()->put_PropertyCollection( export_fid, source_vector_representation.lock()->get_PropertyCollection( source_fid )->create_Clone() );
            }

            successful = true;
            return successful;
        }

        virtual bool OnError() override
        {
            return true;
        }

        virtual unsigned long count() const override
        {
            return 0;
        }

        virtual bool next( int64_t* ppVal ) override
        {
            return true;
        }

        virtual void reset() override
        {
        }

        virtual int64_t operator[]( unsigned long index ) override
        {
            return 0;
        }

        virtual int64_t get_AtIndex( unsigned long index ) override
        {
            return 0;
        }

    private:

        Geoweb3d::IGW3DVectorLayerWPtr& source_vector_layer_;
        Geoweb3d::IGW3DVectorLayerWPtr& exported_vector_layer_;

    };


    ExampleGW3DVectorDataSourceCopyCallback()
    {
    }

    void SetGeoPackageDataSource( Geoweb3d::IGW3DVectorDataSourceWPtr* geopackage_vector_datasource_for_saving )
    {
        callee_vector_datasource_ = geopackage_vector_datasource_for_saving;
    }

    Geoweb3d::IGW3DVectorDataSourceCopyCallback::CopyDataSourceCallbackReturnType OnDataSourceCreated( Geoweb3d::IGW3DVectorDataSourceWPtr& source_vector_datasource, Geoweb3d::IGW3DVectorDataSourceWPtr& exported_vector_datasource ) override
    {
        Geoweb3d::IGW3DVectorDataSourceCopyCallback::CopyDataSourceCallbackReturnType failure( Geoweb3d::IGW3DVectorDataSourceCopyCallback::CopyDataSourceCallbackReturnType::DISCARD );
        if ( source_vector_datasource.expired() || exported_vector_datasource.expired() ) return failure;
        Geoweb3d::IGW3DVectorLayerCollection* source_vector_layer_collection = source_vector_datasource.lock()->get_VectorLayerCollection();
        Geoweb3d::IGW3DVectorLayerCollection* exported_vector_layer_collection = exported_vector_datasource.lock()->get_VectorLayerCollection();
        if ( source_vector_layer_collection == nullptr || exported_vector_layer_collection == nullptr ) return failure;
        if ( source_vector_layer_collection->count() != exported_vector_layer_collection->count() ) return failure;

        for ( unsigned long j = 0; j < exported_vector_layer_collection->count(); ++j )
        {
            std::cout << exported_vector_layer_collection->get_AtIndex( j ).lock()->get_Name() << std::endl;
        }

        for ( unsigned long i = 0; i < source_vector_layer_collection->count(); ++i )
        {
            Geoweb3d::IGW3DVectorLayerWPtr source_vector_layer = source_vector_layer_collection->get_AtIndex( i );
            if ( source_vector_layer.expired() ) continue;

            Geoweb3d::IGW3DVectorLayerWPtr exported_vector_layer = exported_vector_layer_collection->get_ByName( source_vector_layer.lock()->get_Name() );
            if ( exported_vector_layer.expired() ) continue;

            for ( unsigned long j = 0; j < source_vector_layer.lock()->get_RepresentationLayerCollection()->count(); ++j )
            {
                Geoweb3d::IGW3DVectorRepresentationWPtr vector_representation_to_import_from = source_vector_layer.lock()->get_RepresentationLayerCollection()->get_AtIndex( j );
                if ( vector_representation_to_import_from.expired() ) continue;

                const Geoweb3d::IGW3DPropertyCollectionPtr imported_properties = vector_representation_to_import_from.lock()->get_PropertyCollection()->create_Clone();
                imported_properties->put_Property( imported_properties->get_DefinitionCollection()->get_IndexByName( "URL" ), "" );
                Geoweb3d::Vector::RepresentationLayerCreationParameter imported_rep_layer_creation_params;
                imported_rep_layer_creation_params.page_level = vector_representation_to_import_from.lock()->get_PagingLevel();
                imported_rep_layer_creation_params.representation_default_parameters = imported_properties;

                // This will by default not represent it in the scene, presumably due to the fact that the source one was.
                // Do this explicitly if your intention is just to serialize the information to disk.
                // Otherwise, if you plan on representing it, you should also then disable the source's version of this VectorRepresentation.
                imported_rep_layer_creation_params.representation_layer_activity = false;

                // Get the VectorLayer's RepresentationDriver and we'll make a cloned representation with the cloned layer.
                Geoweb3d::IGW3DVectorRepresentationDriverWPtr vector_representation_driver = vector_representation_to_import_from.lock()->get_Driver();

                Geoweb3d::IGW3DVectorRepresentationWPtr vector_representation_to_export_to = vector_representation_driver.lock()->get_RepresentationLayerCollection()->create( exported_vector_layer, imported_rep_layer_creation_params );
                vector_representation_to_export_to.lock()->put_PagingLevel( vector_representation_to_import_from.lock()->get_PagingLevel() );
                vector_representation_to_export_to.lock()->get_ClampRadiusFilter()->put_Enabled  ( vector_representation_to_import_from.lock()->get_ClampRadiusFilter()->get_Enabled() );
                vector_representation_to_export_to.lock()->get_ClampRadiusFilter()->put_MinRadius( vector_representation_to_import_from.lock()->get_ClampRadiusFilter()->get_MinRadius() );
                vector_representation_to_export_to.lock()->get_ClampRadiusFilter()->put_MaxRadius( vector_representation_to_import_from.lock()->get_ClampRadiusFilter()->get_MaxRadius() );
                vector_representation_to_export_to.lock()->get_ClampRadiusFilter()->put_ApplyToAnalytics( vector_representation_to_import_from.lock()->get_ClampRadiusFilter()->get_ApplyToAnalytics() );
            }

            // Now we need to stream this new vector_representation_to_export_to and the its associated vector_representation_to_import_from.
            // This stream will facilitate copying the Properties from the Imported to the Exported. We'll stream on the exported vector layer since
            // its AttributeDefinition has the lookup table for its corresponding properties to duplicate on the Imported side.
            RepresentationPropertyCopyStreamer rep_property_copy_streamer( source_vector_layer, exported_vector_layer );
            exported_vector_layer.lock()->Stream( &rep_property_copy_streamer );
        }

        *callee_vector_datasource_ = exported_vector_datasource;
        return Geoweb3d::IGW3DVectorDataSourceCopyCallback::CopyDataSourceCallbackReturnType::KEEP;
    }

    Geoweb3d::IGW3DVectorDataSourceWPtr* callee_vector_datasource_;
    typedef std::map<long, long> FIDMap;
    std::map<Geoweb3d::IGW3DVectorRepresentationWPtr, FIDMap> source_fid_to_exported_fid_map_;

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> A basic application to exemplify how a client can properly use the
///           AttributeMapper library to save a Geoweb3d_Datasource as a scene 
///           file to disk which can then be opened by the AttributeMapper itself. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class App
{
	public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates an App instance.                        </summary>
        ///
        /// <param name="sdk_context"> A Geoweb3d SDK engine context. </param>
        /// 
        /// <returns> An instance to an App.                          </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		App( const Geoweb3d::IGW3DGeoweb3dSDKPtr& sdk_context )
			: sdk_engine_context_( sdk_context )
		{ }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Initializes the App instances' window, camera, camera navigator,
        ///           raster layer, elevation layer, Projected Texture layer, features, 
        ///           representation, Line Intersection tester, and a streamer.            </summary>
        ///         
        /// <returns> true if all initializations in the summary above completed properly. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Initialize()
		{
			
            Geoweb3d_Datasource_driver_ = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "Geoweb3d_Datasource" );
            
			if( Geoweb3d_Datasource_driver_.expired() )
			{
				std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilites example" << std::endl;
				return false;
			}

            GeoPackage_Datasource_driver_ = sdk_engine_context_->get_VectorDriverCollection()->get_Driver( "GPKG" );
            if ( GeoPackage_Datasource_driver_.expired() )
            {
                std::cout << "Could not find the requested driver! For a list of supported drivers, run the Print Capabilites example" << std::endl;
                return false;
            }

			if( !Geoweb3d_Datasource_driver_.lock()->get_IsCapable( "CreateDataSource" ) )
			{
				std::cout << "This driver is not capable of creating new datasources!" << std::endl;
				return false;
			}

            Geoweb3d_datasource_ = Geoweb3d_Datasource_driver_.lock()->get_VectorDataSourceCollection()->create( "TestDataSource" );
			if( Geoweb3d_datasource_.expired() )
			{
				std::cout << "Could not create data source." << std::endl;
				return false;
			}

            // This example app's intended purpose is to show how a single layer can create a model and
            // select it in the scene to move it around with a couple of different translation modes.        
            double longitude = -100.0;
            double latitude = 45.0;
            for ( int i = 0; i < 15; ++i  )
            {
                std::string layer_name( "Model_Layer_" + std::to_string( i + 1 ) );
                AddModel( layer_name, longitude, latitude, 0.0 );
                longitude -= 1.0;
                latitude  += 1.0;
            }            

			return true;
		}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Forwards information to CreateModelLayer and to 
        ///           CreateModelRepresentation to properly create a Model
        ///           layer and Representation to visualize it in the map.                     </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                  </param>
        /// 
        /// <param name="longitude"> Center longitude of the imagery raster envelope.          </param>
        /// 
        /// <param name="latitude"> Center latitude of the imagery raster envelope.            </param>
        /// 
        /// <param name="elevation"> Starting elevation for the model to be visible.           </param>
        /// 
        /// <returns> true when successful and false if an error occurred or something failed. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        bool AddModel( std::string layer_name, double longitude, double latitude, double elevation )
        {
            Geoweb3d::IGW3DVectorLayerWPtr model_layer = CreateModelLayer( layer_name, longitude, latitude, elevation );

            if ( model_layer.expired() )
            {
                std::cout << "Error creating " << layer_name << std::endl;
                return false;
            }

            // For Models there is a "Model" driver.
            Geoweb3d::IGW3DVectorRepresentationDriverWPtr model_rep_driver = sdk_engine_context_->get_VectorRepresentationDriverCollection()->get_Driver( "Model" );

            // For a model, it has a "URL" property that can be assigned as a relative or absolute path.
            const Geoweb3d::IGW3DPropertyCollectionPtr properties = model_rep_driver.lock()->get_PropertyCollection()->create_Clone();
            properties->put_Property( properties->get_DefinitionCollection()->get_IndexByName( "URL" ), ".//data//sdk//models//Vehicles//Nissan_GTR//Nissan GTR.flt" );

            Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;
            rep_layer_creation_params.page_level = 1;
            rep_layer_creation_params.representation_default_parameters = properties;

            Geoweb3d::IGW3DVectorRepresentationWPtr new_representation = model_rep_driver.lock()->get_RepresentationLayerCollection()->create( model_layer, rep_layer_creation_params );
            Geoweb3d::IGW3DPropertyCollectionPtr modified_property_collection = new_representation.lock()->get_PropertyCollection()->create_Clone();
            modified_property_collection->put_Property( properties->get_DefinitionCollection()->get_IndexByName( "SCALE_X" ), "2.0" );
            modified_property_collection->put_Property( properties->get_DefinitionCollection()->get_IndexByName( "SCALE_Y" ), "3.0" );
            modified_property_collection->put_Property( properties->get_DefinitionCollection()->get_IndexByName( "SCALE_Z" ), "4.0" );
            new_representation.lock()->put_PropertyCollection( 0, modified_property_collection );

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> Creates a single layer for a model.                                      </summary>
        /// 
        /// <param name="layer_name"> Custom name of the layer being created.                  </param>
        /// 
        /// <param name="layer_lon"> Center longitude of the imagery raster envelope.          </param>
        /// 
        /// <param name="layer_lat"> Center latitude of the imagery raster envelope.           </param>
        /// 
        /// <param name="layer_elev"> Starting elevation for the model to be visible.          </param>
        /// 
        /// <returns> A valid VectorLayer when successful and nullptr if an error 
        ///           occurred or something failed.                                            </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        Geoweb3d::IGW3DVectorLayerWPtr CreateModelLayer( std::string layer_name, double layer_lon, double layer_lat, double layer_elev )
        {
            // We're not using the field_definition for anything in particular for this example, but can be used for attribute mapping.
            // Without an initialized Geoweb3d::IGW3DDefinitionCollectionPtr, the VectorLayerCollection create call will fail for your layer, 
            // so it must be created / allocated. We also need it so we can create an empty / default property collection for the feature of the layer.
            Geoweb3d::IGW3DDefinitionCollectionPtr definition_collection( Geoweb3d::IGW3DDefinitionCollection::create() );
            //int new_prop_index = definition_collection->add_Property( "Test Prop", Geoweb3d::PROPERTY_STR, "This Property holds the owning layer's name." );

            Geoweb3d::IGW3DPropertyCollectionPtr property_collection = definition_collection->create_PropertyCollection();
            //property_collection->put_Property( new_prop_index, layer_name.c_str() );

            for ( unsigned int definition_index = 0; definition_index < definition_collection->count(); ++definition_index )
            {
                const Geoweb3d::GW3DPropertyDescription* current_property = definition_collection->get_AtIndex( definition_index );
                Geoweb3d::IGW3DStringPtr property_value_str = Geoweb3d::IGW3DString::create( "" );
                property_collection->get_Property( definition_index ).to_string( property_value_str );
                std::cout << "Property #" << definition_index << "( " << current_property->property_name << " ): " << property_value_str->c_str() << std::endl;
            }

            // Since we're moving a model via its center point, a gtPOINT_25D geometry type is required.            
            Geoweb3d::IGW3DVectorLayerWPtr layer = Geoweb3d_datasource_.lock()->get_VectorLayerCollection()->create( layer_name.c_str(), Geoweb3d::gtPOINT_25D, definition_collection );

            if ( layer.expired() )
            {
                return layer;
            }

            // This will be where the model's location will be.
            Geoweb3d::GW3DPoint model_point( layer_lon, layer_lat, layer_elev );

            Geoweb3d::GW3DResult result;
            // Create the feature that we're going to Represent later and eventually will copy the properties .
            layer.lock()->create_Feature( property_collection, &model_point, result );
            return layer;
        }

        bool RepresentModelLayer( Geoweb3d::IGW3DVectorLayerWPtr vector_layer_to_represent )
        {
            bool successful = false;
            if ( vector_layer_to_represent.expired() ) return successful;
            
            // First we'll retrieve the Vector Representation Driver for the given VectorRepresentationMode the Scene Editor is is.
            Geoweb3d::IGW3DVectorRepresentationDriverCollection* rep_driver_collection = sdk_engine_context_->get_VectorRepresentationDriverCollection();
            Geoweb3d::IGW3DVectorRepresentationDriverWPtr        model_rep_driver      = rep_driver_collection->get_Driver( "Model" );
            Geoweb3d::IGW3DPropertyCollectionPtr                 properties = Geoweb3d_Datasource_driver_.lock()->get_PropertyCollection()->create_Clone();
            
            // Setup the default representation creation parameters.
            Geoweb3d::Vector::RepresentationLayerCreationParameter rep_layer_creation_params;
            rep_layer_creation_params.page_level = 1;
            rep_layer_creation_params.representation_default_parameters = properties;

            // Represent the model with the previously created Vector Layer.
            Geoweb3d::IGW3DVectorRepresentationWPtr model_representation = model_rep_driver.lock()->get_RepresentationLayerCollection()->create( vector_layer_to_represent, rep_layer_creation_params );            
            successful = !model_representation.expired();
            return successful;
        }

        bool SaveSceneToFile( const std::string& save_path_and_file_name )
        {
            bool successful = ConvertGeoweb3dDataSourceToGeoPackageDataSource( save_path_and_file_name );
            if ( successful )
            {
                successful = ConvertGeoPackageDataSourceToSceneFile( save_path_and_file_name );
            }
            return successful;
        }

        bool ConvertGeoweb3dDataSourceToGeoPackageDataSource( const std::string& save_path_and_file_name )
        {
            bool successful = false;

            if ( !Geoweb3d_datasource_.expired() && !GeoPackage_Datasource_driver_.expired() )
            {
                std::experimental::filesystem::path datasource_path( save_path_and_file_name );
                std::string driver_file_extension( GeoPackage_Datasource_driver_.lock()->get_FileExtentionCollection()->get_AtIndex( 0 )->c_str() );
                std::string geopackage_output_file_name = datasource_path.stem().string() + "_models." + driver_file_extension;
                Geoweb3d::IGW3DPropertyCollectionPtr default_property_collection = GeoPackage_Datasource_driver_.lock()->get_PropertyCollection()->create_Clone();
                // If it existed on disk previously, we'll clean it up here based on the fact it isn't empty.
                if ( std::experimental::filesystem::exists( geopackage_output_file_name ) )
                {
                    GeoPackage_Datasource_driver_.lock()->get_VectorDataSourceCollection()->destroy( geopackage_output_file_name.c_str() );
                }

                // We'll use the copy callback to determine when we can safely create representations and apply properties to them based on the callback's invocations.
                copy_callback_.SetGeoPackageDataSource( &GeoPackage_datasource_ );
                successful = Geoweb3d_datasource_.lock()->create_Copy( GeoPackage_Datasource_driver_, default_property_collection.get(), geopackage_output_file_name.c_str(), &copy_callback_ );
            }
            return successful;
        }

        bool ConvertGeoPackageDataSourceToSceneFile( const std::string& save_path_and_file_name )
        {
            bool result = Geoweb3d::Succeeded( sdk_engine_context_->get_SceneCollection( true )->Create( GeoPackage_datasource_, save_path_and_file_name.c_str() ) );
            GeoPackage_Datasource_driver_.lock()->get_VectorDataSourceCollection()->close( GeoPackage_datasource_ );
            GeoPackage_datasource_.reset();
            return result;
        }
       
	private:
		Geoweb3d::IGW3DGeoweb3dSDKPtr           sdk_engine_context_;
        Geoweb3d::IGW3DVectorDriverWPtr         Geoweb3d_Datasource_driver_;
        Geoweb3d::IGW3DVectorDriverWPtr         GeoPackage_Datasource_driver_;
        Geoweb3d::IGW3DVectorDataSourceWPtr     Geoweb3d_datasource_;
        Geoweb3d::IGW3DVectorDataSourceWPtr     GeoPackage_datasource_;
        ExampleGW3DVectorDataSourceCopyCallback copy_callback_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Runs the Attribute Mapper Save Application instance. </summary>
/// 
/// <param name="sdk_context"> The Geoweb3d engine context.        </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void RunApplication( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context )
{
    App app( sdk_context );
    if ( app.Initialize() )
    {
        app.SaveSceneToFile("Example_Scene_File.scene");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Initializes the Geowebd3d engine SDK context. 
///           Initializes SDK configuration.                
///           Runs the Attribute Mapper saving example application.  </summary>
/// 
/// <param name="argc"> Number of command line arguments.            </param>
/// 
/// <param name="argv"> The contents of the command line arguments.  </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain( int argc, _TCHAR* argv[] )
{
    SetInformationHandling();

    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context( Geoweb3d::IGW3DGeoweb3dSDK::CreateInterface() );

    if ( sdk_context )
    {
        Geoweb3d::IGW3DInitializationConfigurationPtr sdk_init = sdk_context->create_InitializationConfiguration();
        sdk_init->put_ESRILicenseCheckout( false ); //If you have an ESRI license and want to be able to load data using their drivers, remove this line
        if ( Geoweb3d::Succeeded( sdk_context->InitializeLibrary( "geoweb3dsdkdemo", sdk_init, 5, 0 ) ) )
        {
            RunApplication( sdk_context );
        }
        else
        {
            std::cout << "\nError creating SDK context! Press any key to continue...\n" << std::endl;
            getchar();
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Function handler for fatal errors inside the engine.
///           For the purposes of this demo, we only print to the 
///           end user what went wrong.                             </summary>
/// 
/// <param name="msg"> Geoweb3d::Fatal categorized messages.        </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_fatal_function( const char* msg )
{
    std::cout << "Fatal Info: " << msg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> General info function, this will be used to
///           print any general info and non-fatal warnings.                           </summary>
/// 
/// <param name="msg"> Geoweb3d::Information / Geoweb3d::Warning categorized messages. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void my_info_function( const char* msg )
{
    std::cout << "General Info: " << msg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Maps GeoWeb3d_InfoType information warnings to a function. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetInformationHandling()
{
    // Tell the engine about our error function handling.
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Information, my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Warning, my_info_function );
    Geoweb3d::SDK_Notification::GeoWeb3dSetIOFunction( Geoweb3d::Fatal, my_fatal_function );
}