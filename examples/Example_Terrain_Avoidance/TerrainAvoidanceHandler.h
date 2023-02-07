
#include "engine/IGW3DGeoweb3dSDK.h"

#include "engine/IGW3DTerrainAvoidanceTest.h"
#include "engine/IGW3DTerrainAvoidanceTestCollection.h"

#include "Geoweb3d\core\GW3DInterFace.h"

class TerrainAvoidanceHandler
{
public:

	TerrainAvoidanceHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context);

	void put_ReferenceAltitudeMode(const Geoweb3d::IGW3DTerrainAvoidanceTest::ReferenceAltitudeMode ref_alt_mode);
	void put_ReferenceAltitude(const double ref_alt);
	void get_ReferenceAltitude(double& ref_alt);
	void put_IncreasedDetailMode(Geoweb3d::IGW3DTerrainAvoidanceTest::IncreasedDetailMode& mode);
	void put_TerrainLinearLOD(double& val);
	double get_TerrainLinearLOD();
	void ToggleTerrainAvoidance(Geoweb3d::IGW3DCameraWPtr&);
	void TurnOnRepInAnalytic(Geoweb3d::IGW3DVectorRepresentationWPtr& rep, bool enable);
private:

	void BuildAndSetTargetedColorLut();

	friend class MiniEngine;
	/// <summary>	The slope analysis. </summary>
	Geoweb3d::IGW3DTerrainAvoidanceTestWPtr terrain_avoidance_;
	/// <summary>	Context for the sdk. </summary>
    Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;

	Geoweb3d::IGW3DColorLookupTableFinalizationTokenPtr color_lut_token_;

	double reference_alt_;
	double linear_lod_;
};