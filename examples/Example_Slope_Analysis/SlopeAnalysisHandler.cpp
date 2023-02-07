
#include "SlopeAnalysisHandler.h"

#include "engine/GW3DCamera.h"
#include "Geoweb3dCore/GeometryExports.h"

#include "engine\IGW3DSceneGraphContext.h"
#include "engine\IGW3DColorLookupTable.h"

	SlopeAnalysisHandler::SlopeAnalysisHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
		: sdk_context_ (sdk_context)
		, target_slope(1)
		, target_slope_min(1)
		, target_slope_max(1)
	{}

	void SlopeAnalysisHandler::Set_LODPolicy(const Geoweb3d::IGW3DRegionOfInterest::LODPolicy policy)
	{
		if(!slope_analysis_.expired())
        {
			slope_analysis_.lock()->put_LODPolicy( policy );
		}
	}

	void SlopeAnalysisHandler::Set_InnerRadius(const double &inner_rad)
	{
		if(!slope_analysis_.expired())
        {
			slope_analysis_.lock()->put_InnerRadius( inner_rad);
		}
	}

	double SlopeAnalysisHandler::Get_InnerRadius()
	{
		if(!slope_analysis_.expired())
        {
			return slope_analysis_.lock()->get_InnerRadius();
		}
		else
		{
			return 0.0;
		}			
	}

	void SlopeAnalysisHandler::Set_OuterRadius(const double &outer_rad)
	{
		if(!slope_analysis_.expired())
        {
			slope_analysis_.lock()->put_OuterRadius( outer_rad);
		}
	}

	double SlopeAnalysisHandler::Get_OuterRadius()
	{
		if(!slope_analysis_.expired())
        {
			return slope_analysis_.lock()->get_OuterRadius();
		}
		else
		{
			return 0.0;
		}		
	}

	void SlopeAnalysisHandler::Set_IncreaseSpan()
	{
		if (!slope_analysis_.expired())
		{
			target_slope_max++;
			target_slope_min++;

			if (target_slope_max > 90)
				target_slope_max = 90;

			if (target_slope_min < 0)
				target_slope_min = 0;

			BuildAndSetTargetedColorLut();
		}
	}

	void SlopeAnalysisHandler::Set_DecreaseSpan()
	{
		if (!slope_analysis_.expired())
		{
			target_slope_max--;
			target_slope_min--;

			if (target_slope_max > 90)
				target_slope_max = 90;

			if (target_slope_min < 0)
				target_slope_min = 0;

			BuildAndSetTargetedColorLut();
		}
	}

	void SlopeAnalysisHandler::Set_IncrementTargetedSlope()
	{
		if (!slope_analysis_.expired())
		{
			target_slope++;

			if (target_slope > 90)
				target_slope = 90;

			BuildAndSetTargetedColorLut();
		}

	}

	void SlopeAnalysisHandler::Set_DecrementTargetedSlope()
	{
		if (!slope_analysis_.expired())
		{
			target_slope--;

			if (target_slope < 0)
				target_slope = 0;

			BuildAndSetTargetedColorLut();
		}

	}

	void SlopeAnalysisHandler::BuildAndSetTargetedColorLut()
	{

			//Build the color lut for the slope analysis to use
			Geoweb3d::IGW3DColorLookupTablePtr  color_lut = sdk_context_->get_SceneGraphContext()->create_ColorLookupTable(0, 90);

			int min_val = target_slope - target_slope_min < 0 ? 0 : target_slope - target_slope_min;
			int max_val = target_slope + target_slope_max > 90 ? 90 : target_slope + target_slope_max;

			color_lut->put_Range(0, min_val, 0.0f, 1.0f, 0.0f, 0.5f);
			color_lut->put_Range(min_val, max_val, 1.0f, 0.0f, 0.0f, 0.5f);
			color_lut->put_Range(max_val, 90, 0.0f, 1.0f, 0.0f, 0.5f);

			color_lut_token_ = color_lut->create_FinalizeToken();

			slope_analysis_.lock()->put_GW3DFinalizationToken(color_lut_token_);

			printf(" Targeting slopes between %d and %d degrees\n", min_val, max_val);

	}

	void SlopeAnalysisHandler::ToggleSlopeAnalysis(Geoweb3d::IGW3DCameraWPtr& camera)
	{
		if(slope_analysis_.expired())
        {						
			slope_analysis_ = sdk_context_->get_SlopeAnalysisCollection()->create();

			if(!slope_analysis_.expired() )
			{
				//make sure our camera is added to the analysis
				//note, we should try to be smarter and only
				//add when needed, as well as only query the camera
				//position once per frame
            slope_analysis_.lock()->put_LODPolicy(Geoweb3d::IGW3DRegionOfInterest::LODPolicy::FullIn_ViewOnly);
				slope_analysis_.lock()->put_AddCamera(camera);
								
				Geoweb3d::IGW3DCameraController* camera_controller = camera.lock()->get_CameraController();


				//put the slope analysis just below the camera, pointing down a bit.
				const Geoweb3d::GW3DPoint *cloc = camera_controller->get_Location( );
				double lon = cloc->get_X();
				double lat = cloc->get_Y();

				slope_analysis_.lock()->put_Location( lon,lat );
				slope_analysis_.lock()->put_Elevation(0, Geoweb3d::IGW3DRegionOfInterest::AltitudeMode::Relative);

				BuildAndSetTargetedColorLut();

			}
        }
        else
        {		
            printf("slope analysis destroyed\n");
			Geoweb3d::IGW3DSlopeAnalysisCollection::close( slope_analysis_ );
			
        }
	}


