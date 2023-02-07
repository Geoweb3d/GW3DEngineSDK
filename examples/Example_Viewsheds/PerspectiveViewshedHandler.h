#pragma once

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DPerspectiveViewshedTest.h"
#include "engine/IGW3DPerspectiveViewshedTestCollection.h"
#include <vector>

class PerspectiveViewshedHandler
{
public:
	PerspectiveViewshedHandler( Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context );

	void SetViewshed_LODPolicy( const Geoweb3d::IGW3DViewshedTest::LODPolicy policy );

	void SetViewshed_HorizontalAngle( const double &hangle );
	double GetViewshed_HorizontalAngle();

	void SetViewshed_VerticalAngle( const double &vangle );
	double GetViewshed_VerticalAngle();

	void SetViewshed_AspectRatio( const double &value );
	double GetViewshed_AspectRatio();

	void SetViewshed_InnerRadius( const double &inner_radius );
	double GetViewshed_InnerRadius();

	void SetViewshed_OuterRadius( const double &outer_radius );
	double GetViewshed_OuterRadius();

	void SetViewshed_Elevation( const double &elev );
	double GetViewshed_Elevation();

	void SetViewshed_Pitch( const float &pitch );
	float GetViewshed_Pitch();

	void ToggleViewshed( Geoweb3d::IGW3DCameraWPtr&, bool insert_projected_image );

private:
	friend class MiniEngine;

	/// <summary>	The perspective_viewshed analysis. </summary>
	Geoweb3d::IGW3DPerspectiveViewshedTestWPtr viewshed_test_;
	Geoweb3d::IGW3DVectorRepresentationWPtr  projected_image_rep_;

	/// <summary>	Context for the sdk. </summary>
	Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
};