
#include "TerrainAvoidanceHandler.h"

#include "engine/GW3DCamera.h"
#include "Geoweb3dCore/GeometryExports.h"

#include "engine\IGW3DSceneGraphContext.h"
#include "engine\IGW3DColorLookupTable.h"
#include "engine/IGW3DVectorRepresentationHelperCollection.h"

	TerrainAvoidanceHandler::TerrainAvoidanceHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
		: sdk_context_ (sdk_context)
		, reference_alt_(700.f)
	{}

	void TerrainAvoidanceHandler::put_ReferenceAltitudeMode(const Geoweb3d::IGW3DTerrainAvoidanceTest::ReferenceAltitudeMode ref_alt_mode)
	{
		if (!terrain_avoidance_.expired())
		{

			terrain_avoidance_.lock()->put_ReferenceAltitudeMode(ref_alt_mode);
			
		}
	}

	void TerrainAvoidanceHandler::put_ReferenceAltitude(const double ref_alt)
	{
		if (!terrain_avoidance_.expired())
		{
			terrain_avoidance_.lock()->put_ReferenceAltitude(ref_alt);
			reference_alt_ = terrain_avoidance_.lock()->get_ReferenceAltitude();
		}
	}

	void TerrainAvoidanceHandler::get_ReferenceAltitude(double &ref_alt)
	{
		if (!terrain_avoidance_.expired())
		{
			reference_alt_ = terrain_avoidance_.lock()->get_ReferenceAltitude();
			ref_alt = reference_alt_;

		}
	}

	void TerrainAvoidanceHandler::put_TerrainLinearLOD(double& val)
	{
		if (!terrain_avoidance_.expired())
		{
			terrain_avoidance_.lock()->put_TerrainLinearLOD(val);
			linear_lod_ = terrain_avoidance_.lock()->get_TerrainLinearLOD();
		}
	}

	void TerrainAvoidanceHandler::put_IncreasedDetailMode(Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode& mode)
	{
		if (!terrain_avoidance_.expired())
		{
			terrain_avoidance_.lock()->put_IncreasedDetailMode(mode);
		}
	}

	double TerrainAvoidanceHandler::get_TerrainLinearLOD()
	{
		if (!terrain_avoidance_.expired())
		{
			linear_lod_ = terrain_avoidance_.lock()->get_TerrainLinearLOD();
		}
		return linear_lod_;
	}

	void TerrainAvoidanceHandler::BuildAndSetTargetedColorLut()
	{
			//Build the color lut for the terrain avoidance to use
			Geoweb3d::IGW3DColorLookupTablePtr  color_lut = sdk_context_->get_SceneGraphContext()->create_ColorLookupTable(0, 90);

			int min_val = 0;
			int max_val = 1500;

			color_lut->put_Range(0, 300, 1.0f, 0.0f, 0.0f, 0.5f);
			color_lut->put_Range(301, 600, 1.0f, 1.0f, 0.0f, 0.5f);
			color_lut->put_Range(601, 900, 0.0f, 1.0f, 0.0f, 0.5f);

			color_lut->put_NoDataColor(0.2f, 0.9f, 0.9f, 1.0f);

			color_lut_token_ = color_lut->create_FinalizeToken();

			terrain_avoidance_.lock()->put_GW3DFinalizationToken(color_lut_token_);

			printf(" Targeting terrain between %d and %d meters from reference\n", min_val, max_val);

	}

	void TerrainAvoidanceHandler::ToggleTerrainAvoidance(Geoweb3d::IGW3DCameraWPtr& camera)
	{
		if(terrain_avoidance_.expired())
        {						
			terrain_avoidance_ = sdk_context_->get_TerrainAvoidanceTestCollection()->create();

			if(!terrain_avoidance_.expired() )
			{
				terrain_avoidance_.lock()->put_AddCamera(camera);
				terrain_avoidance_.lock()->put_Enabled(true);
				terrain_avoidance_.lock()->put_ReferenceAltitude(reference_alt_);
				terrain_avoidance_.lock()->put_NoDataColor(0.0f, 1.0f, 1.0f, 0.5f);
				BuildAndSetTargetedColorLut();
			}
        }
        else
		{
			static bool toggle = false;
			terrain_avoidance_.lock()->put_Enabled(toggle);
			printf("TA Mask %s\n", toggle ? "On" : "Off");
			toggle = !toggle;
			//printf("terrain avoidance destroyed\n");
			//Geoweb3d::IGW3DTerrainAvoidanceTestCollection::close(terrain_avoidance_);
        }
	}

	void TerrainAvoidanceHandler::TurnOnRepInAnalytic(Geoweb3d::IGW3DVectorRepresentationWPtr& rep, bool enable)
	{
		if (!terrain_avoidance_.expired())
		{
			Geoweb3d::GW3DResult res;
			Geoweb3d::IGW3DVectorRepresentationHelperCollection* helper = terrain_avoidance_.lock()->get_VectorRepresentationCollection(true);
			if (enable)
			{
				res = helper->add(rep);
			}
			else
			{
				res = helper->remove(rep);
			}

		}
	}


