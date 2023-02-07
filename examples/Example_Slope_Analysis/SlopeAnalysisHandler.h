
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DSlopeAnalysis.h"
#include "engine/IGW3DSlopeAnalysisCollection.h"

#include "Geoweb3d\core\GW3DInterFace.h"

class SlopeAnalysisHandler
{
public:

	SlopeAnalysisHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context);

	void Set_LODPolicy(const Geoweb3d::IGW3DRegionOfInterest::LODPolicy policy);
	
	void Set_InnerRadius(const double &inner_radius);
	double Get_InnerRadius();

	void Set_OuterRadius(const double &outer_radius);	
	double Get_OuterRadius();

	void Set_IncreaseSpan();
	void Set_DecreaseSpan();

	void Set_IncrementTargetedSlope();
	void Set_DecrementTargetedSlope();
		
	void ToggleSlopeAnalysis(Geoweb3d::IGW3DCameraWPtr&);

private:

	void BuildAndSetTargetedColorLut();

	friend class MiniEngine;
	/// <summary>	The slope analysis. </summary>
	Geoweb3d::IGW3DSlopeAnalysisWPtr slope_analysis_;
	/// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;

	Geoweb3d::IGW3DColorLookupTableFinalizationTokenPtr color_lut_token_;

	int target_slope_min;
	int target_slope_max;
	int target_slope;
};