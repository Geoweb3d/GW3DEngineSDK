#pragma once
#include "engine/IGW3DGeoweb3dSDK.h"
#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DRaster.h"

#include "engine/IGW3DVectorRepresentationCollection.h"

#include <deque>

class MiniEngineCore
{
	public:
		MiniEngineCore()
		{

		}

		virtual ~MiniEngineCore() {}

		
		static bool CreateImageryRepresentation( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::IGW3DRasterDataSourceWPtr imagery_data_source
			, int priority, Geoweb3d::GW3DEnvelope& envelope )
		{
			// Step 2. Get the layer collection from the raster data source.
			Geoweb3d::IGW3DRasterLayerCollection* layer_collection = imagery_data_source.lock()->get_RasterLayerCollection();
			if (layer_collection && layer_collection->count() > 0)
			{
				// Step 3. Retrieve the layer needed for representation via the Imagery raster layer driver.
				//         In this case, the *.ecw file used only has a single layer, so we'll retrieve just that layer.
				auto imagery_layer = layer_collection->get_AtIndex(0);

				if (!imagery_layer.expired())
				{
					// Step 4. Retrieve the Imagery raster layer driver so the layer can be properly created and visualized.
					auto imagery_rep_driver = sdk_context->get_RasterLayerRepresentationDriverCollection()->get_Driver("Imagery");
					if (!imagery_rep_driver.expired())
					{
						// Step 5. Create the actual RasterRepresentation. You can optionally capture the return of this,
						//         but is not necessary for one-time visualization purposes, like in this example App.
						Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
						params.page_level = 0;
						params.priority = priority;
						params.representation_layer_activity = true;
						imagery_rep_driver.lock()->get_RepresentationLayerCollection()->create(imagery_layer, params);

						envelope = imagery_layer.lock()->get_Envelope();
						return true;
					}
				}
			}

			return false;
		}

		void LoadDemoImageryDataset( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, Geoweb3d::GW3DEnvelope& envelope )
		{
			Geoweb3d::GW3DResult result;
			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.
			imagery_data_source0_ = sdk_context->get_RasterDriverCollection()->auto_Open("../examples/media/WSI-Earth99-2k.ecw", result);

			if (result != Geoweb3d::GW3D_sOk || (!imagery_data_source0_.expired() && !MiniEngineCore::CreateImageryRepresentation(sdk_context, imagery_data_source0_, 1, envelope )))
			{
				printf("Error Loading Imagery\n");
			}

			imagery_data_source1_ = sdk_context->get_RasterDriverCollection()->auto_Open("../examples/media/DC_1ft_demo.ecw", result);

			if (result != Geoweb3d::GW3D_sOk || (!imagery_data_source1_.expired() && !MiniEngineCore::CreateImageryRepresentation(sdk_context, imagery_data_source1_, 2, envelope )))
			{
				printf("Error Loading Iamgery.\n");
			}
		}

		void LoadDemoImageryDataset(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
		{
			Geoweb3d::GW3DEnvelope env;
			LoadDemoImageryDataset(sdk_context, env);
		}

		Geoweb3d::IGW3DRasterDataSourceWPtr CreateElevationRep(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* path, const int page_level
			                                                 , const int priority, const bool enabled, const double vertical_scale, Geoweb3d::GW3DEnvelope& envelope)
		{
			Geoweb3d::GW3DResult res;
			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.

			auto ds = sdk_context->get_RasterDriverCollection()->auto_Open(path, res);

			if (!ds.expired())
			{
				Geoweb3d::IGW3DRasterLayerCollection* layer_collection = ds.lock()->get_RasterLayerCollection();
				auto elevation_rep_driver = sdk_context->get_RasterLayerRepresentationDriverCollection()->get_Driver("Elevation");

				if (!elevation_rep_driver.expired() && layer_collection && layer_collection->count() > 0)
				{
					auto props = elevation_rep_driver.lock()->get_PropertyCollection()->create_Clone();
					props->put_Property(Geoweb3d::Raster::ElevationParameters::VERTICAL_SCALE, vertical_scale);

					Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
					params.page_level = page_level;
					params.priority = priority;
					params.representation_layer_activity = enabled;
					params.representation_default_parameters = props;
					
					auto layer = layer_collection->get_AtIndex(0);

					elevation_rep_driver.lock()->get_RepresentationLayerCollection()->create( layer, params );

					elevation_data_sources_.emplace_back(ds);

					envelope = layer.lock()->get_Envelope();
				}
			}

			return ds;
		}

		Geoweb3d::IGW3DRasterDataSourceWPtr CreateElevationRep(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* path, const int page_level
			, const int priority, const bool enabled, const double vertical_scale )
		{
			Geoweb3d::GW3DEnvelope envelope;
			return CreateElevationRep(sdk_context, path, page_level, priority, enabled, vertical_scale, envelope);
		}

		Geoweb3d::IGW3DRasterDataSourceWPtr CreateImageryRep(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* path, const int page_level
			, const int priority, const bool enabled, Geoweb3d::GW3DEnvelope& envelope, const Geoweb3d::Imagery::ImageryLoadProperties& imagery_properties
		    , int layer_index )
		{
			Geoweb3d::GW3DResult res;

			//NOTE: Using auto_Open allows the system to select the most appropriate driver to open the data source instead of the user
			//having to guess which data source driver is needed to open up a particular dataset.

			auto ds = sdk_context->get_RasterDriverCollection()->auto_Open(path, res);


			auto ImageryLoadPropertiesToPropertyCollection = [](const Geoweb3d::Imagery::ImageryLoadProperties& load_props, Geoweb3d::IGW3DPropertyCollectionPtr& out_props)
			{
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::USE_IMAGE_INVALID_COLOR, load_props.user_defined_invalid_color_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::USER_DEFINED_INVALID_COLOR, load_props.user_defined_invalid_color_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::TOLERANCE_RANGE, load_props.tolerance_range_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_RED, load_props.invalid_red_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_BLUE, load_props.invalid_blue_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_GREEN, load_props.invalid_green_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::SCREEN_RES_MODE, load_props.screen_res_mode_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::RESOLUTION_MAPPING, (int)load_props.resolution_mapping_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::USE_INVALID_REPLACEMENT_COLOR, load_props.use_invalid_replacement_color_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_REPLACEMENT_RED, load_props.invalid_replacement_red_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_REPLACEMENT_BLUE, load_props.invalid_replacement_blue_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_REPLACEMENT_GREEN, load_props.invalid_green_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::INVALID_REPLACEMENT_ALPHA, load_props.invalid_replacement_alpha_);
				out_props->put_Property(Geoweb3d::Raster::ImageryProperties::SAMPLER_ALGO, load_props.sampler_algo_);
			};

			if (!ds.expired())
			{
				Geoweb3d::IGW3DRasterLayerCollection* layer_collection = ds.lock()->get_RasterLayerCollection();
				auto imagery_rep_driver = sdk_context->get_RasterLayerRepresentationDriverCollection()->get_Driver("Imagery");

				if (!imagery_rep_driver.expired() && layer_collection && layer_collection->count() > layer_index )
				{
					auto props = imagery_rep_driver.lock()->get_PropertyCollection()->create_Clone();
					ImageryLoadPropertiesToPropertyCollection(imagery_properties, props);

					Geoweb3d::Raster::RasterRepresentationLayerCreationParameter params;
					params.page_level = page_level;
					params.priority = priority;
					params.representation_layer_activity = enabled;
					params.representation_default_parameters = props;

					auto raster_layer = layer_collection->get_AtIndex(layer_index);

					if (!raster_layer.expired() 
			        &&( ( res = imagery_rep_driver.lock()->get_CapabilityToRepresent(raster_layer) ) == Geoweb3d::GW3D_sOk )
					)
					{
						auto imagery_rep = imagery_rep_driver.lock()->get_RepresentationLayerCollection()->create(raster_layer, params);

						imagery_data_sources_.emplace_back(ds);
						imagery_representations_.emplace_back(imagery_rep);

						envelope = raster_layer.lock()->get_Envelope();
						return ds;
					}
				}
			}

			return Geoweb3d::IGW3DRasterDataSourceWPtr();
		}

		Geoweb3d::IGW3DRasterDataSourceWPtr CreateImageryRep(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* path, const int page_level
			, const int priority, const bool enabled, int layer_index = 0 )
		{
			Geoweb3d::GW3DEnvelope env;
			Geoweb3d::Imagery::ImageryLoadProperties imagery_properties = {};
			imagery_properties.priority = priority;
			imagery_properties.activity = true;

			return CreateImageryRep(sdk_context, path, page_level, priority, enabled, env, imagery_properties, layer_index );
		}

		Geoweb3d::IGW3DRasterDataSourceWPtr CreateImageryRep(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* path, const int page_level
			, const int priority, const bool enabled, Geoweb3d::GW3DEnvelope& env, int layer_index = 0)
		{
			Geoweb3d::Imagery::ImageryLoadProperties imagery_properties = {};
			imagery_properties.priority = priority;
			imagery_properties.activity = true;

			return CreateImageryRep(sdk_context, path, page_level, priority, enabled, env, imagery_properties, layer_index );
		}


		Geoweb3d::IGW3DRasterDataSourceWPtr CreateImageryRep(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context, const char* path, const int page_level
			, const int priority, const bool enabled, const Geoweb3d::Imagery::ImageryLoadProperties& imagery_properties, int layer_index = 0)
		{
			Geoweb3d::GW3DEnvelope env;
			return CreateImageryRep(sdk_context, path, page_level, priority, enabled, env, imagery_properties, layer_index );
		}

	protected:
		Geoweb3d::IGW3DRasterDataSourceWPtr						imagery_data_source0_;
		Geoweb3d::IGW3DRasterDataSourceWPtr						imagery_data_source1_;

		std::deque< Geoweb3d::IGW3DRasterDataSourceWPtr>		elevation_data_sources_;
		std::deque< Geoweb3d::IGW3DRasterDataSourceWPtr>		imagery_data_sources_;
		std::deque< Geoweb3d::IGW3DRasterRepresentationWPtr>	imagery_representations_;

};