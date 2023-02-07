#include "stdafx.h"
#include "PerspectiveViewshedHandler.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DCamera.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"

#include "engine/IGW3DImageCollection.h"
#include "engine/IGW3DImage.h"
#include "engine/IGW3DSceneGraphContext.h"

#include "Geoweb3dCore/GeometryExports.h"
#include "Geoweb3dCore/SystemExports.h"

#include <iostream>

inline Geoweb3d::IGW3DVectorDataSourceWPtr create_Geoweb3dDataSource(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context,  const char *datasourcename )
{
	Geoweb3d::IGW3DVectorDriverWPtr driver = sdk_context->get_VectorDriverCollection()->get_Driver("Geoweb3d_Datasource");

	if(!driver.expired())
	{
		Geoweb3d::IGW3DVectorDataSourceCollection *datasource_collection = driver.lock()->get_VectorDataSourceCollection();

		return datasource_collection->create( datasourcename );
	}
	else
	{
		printf("SDK Bug Detected\n");
	}

	return Geoweb3d::IGW3DVectorDataSourceWPtr();
}


	PerspectiveViewshedHandler::PerspectiveViewshedHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
		: sdk_context_ (sdk_context)
	{}

	void PerspectiveViewshedHandler::SetViewshed_LODPolicy(const Geoweb3d::IGW3DViewshedTest::LODPolicy policy)
	{
		if(!viewshed_test_.expired())
        {
			viewshed_test_.lock()->put_LODPolicy( policy );
		}
	}
	
	void PerspectiveViewshedHandler::SetViewshed_HorizontalAngle(const double &hangle)
	{
		if(!viewshed_test_.expired())
        {
			// TODO
		}
	}

	double PerspectiveViewshedHandler::GetViewshed_HorizontalAngle()
	{
		if(!viewshed_test_.expired())
        {
			return 0.0;  //TODO
		}
		else
		{
			return 0.0;
		}
	}

	void PerspectiveViewshedHandler::SetViewshed_VerticalAngle(const double &vangle)
	{
		if(!viewshed_test_.expired())
        {
			// TODO
		}
	}

	double PerspectiveViewshedHandler::GetViewshed_VerticalAngle()
	{
		if(!viewshed_test_.expired())
		{
			return 0.0;  // TODO
		}
		else
		{
			return 0.0;
		}
	}

	void PerspectiveViewshedHandler::SetViewshed_AspectRatio( const double &value )
	{
		if(!viewshed_test_.expired())
		{
			// TODO
		}
	}

	double PerspectiveViewshedHandler::GetViewshed_AspectRatio()
	{
		if(!viewshed_test_.expired())
		{
			return 0.0;  //TODO
		}
		else
		{
			return 0.0;
		}
	}

	void PerspectiveViewshedHandler::SetViewshed_InnerRadius(const double &inner_rad)
	{
		if(!viewshed_test_.expired())
        {
			viewshed_test_.lock()->get_Frustum()->put_NearPlane( inner_rad );
		}
	}

	double PerspectiveViewshedHandler::GetViewshed_InnerRadius()
	{
		if(!viewshed_test_.expired())
        {
			return viewshed_test_.lock()->get_Frustum()->get_NearPlane();
		}
		else
		{
			return 0.0;
		}
	}

	void PerspectiveViewshedHandler::SetViewshed_OuterRadius(const double &outer_rad)
	{
		if(!viewshed_test_.expired())
        {
			viewshed_test_.lock()->get_Frustum()->put_FarPlane( outer_rad );
		}
	}

	double PerspectiveViewshedHandler::GetViewshed_OuterRadius()
	{
		if(!viewshed_test_.expired())
        {
			return viewshed_test_.lock()->get_Frustum()->get_FarPlane();
		}
		else
		{
			return 0.0;
		}
	}

	void PerspectiveViewshedHandler::SetViewshed_Elevation(const double &elev)
	{
		if(!viewshed_test_.expired())
        {
			viewshed_test_.lock()->put_Elevation( elev);
		}
	}

	double PerspectiveViewshedHandler::GetViewshed_Elevation()
	{
		if(!viewshed_test_.expired())
        {
			double elev;
			Geoweb3d::IGW3DPositionOrientation::AltitudeMode elevmod;

			viewshed_test_.lock()->get_Elevation(elev, elevmod);

			return elev;
		}
		else
		{
			return 0.0;
		}
	}

	void PerspectiveViewshedHandler::SetViewshed_Pitch(const float &pitch)
	{
		if(!viewshed_test_.expired())
        {
			viewshed_test_.lock()->put_Pitch( pitch);
		}
	}

	float PerspectiveViewshedHandler::GetViewshed_Pitch()
	{
		if(!viewshed_test_.expired())
        {
			return viewshed_test_.lock()->get_Pitch();
		}
		else
		{
			return 0.0;
		}		
	}

	void PerspectiveViewshedHandler::ToggleViewshed( Geoweb3d::IGW3DCameraWPtr& camera, bool insert_projected_image )
	{
		if(viewshed_test_.expired())
        {
			Geoweb3d::IGW3DCameraController* camera_controller = camera.lock()->get_CameraController();
			const Geoweb3d::GW3DPoint *cloc = camera_controller->get_Location( );
			double lon = cloc->get_X();
			double lat = cloc->get_Y();
			double target_elevation = 0.0;

			viewshed_test_ = sdk_context_->get_PerspectiveViewshedTestCollection()->create();
			if(!viewshed_test_.expired() )
			{
				//make sure our camera is added to the analysis
				//note, we should try to be smarter and only
				//add when needed, as well as only query the camera
				//position once per frame
				viewshed_test_.lock()->put_LODPolicy( Geoweb3d::IGW3DViewshedTest::LODPolicy::Normal);
				viewshed_test_.lock()->put_AddCamera( camera );

				//put the viewshed just below the camera, pointing down a bit.
				viewshed_test_.lock()->put_Location( lon,lat );
				viewshed_test_.lock()->put_Elevation( target_elevation, Geoweb3d::IGW3DViewshedTest::AltitudeMode::Relative);

				double vfov = 45.0;
				double nearp = 1.0;
				double farp = 2500.0;

				viewshed_test_.lock()->get_Frustum()->put_FrustumHelper( vfov, 1.0, nearp, farp );

				Geoweb3d::IGW3DVectorRepresentationDriverWPtr projected = insert_projected_image
					? sdk_context_->get_VectorRepresentationDriverCollection()->get_Driver( "ProjectedImage" )
					: Geoweb3d::IGW3DVectorRepresentationDriverWPtr();

				if ( !projected.expired() )
				{
					Geoweb3d::GW3DPoint p1( lon, lat, 0.0 );
					Geoweb3d::GW3DResult result;

					Geoweb3d::IGW3DVectorDataSourceWPtr created_datasource = create_Geoweb3dDataSource(sdk_context_, "Dynamic_Features.geoweb3d");

					Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
					Geoweb3d::IGW3DPropertyCollectionPtr field_values (field_definition->create_PropertyCollection());
					Geoweb3d::IGW3DVectorLayerWPtr point_layer	= created_datasource.lock()->get_VectorLayerCollection()->create( "Perspective Points", Geoweb3d::gtPOINT, field_definition );
					printf("Point FeatureID created: %d \n", point_layer.lock()->create_Feature(field_values, &p1, result) );

					Geoweb3d::Vector::RepresentationLayerCreationParameter  params;
					params.representation_default_parameters = projected.lock()->get_PropertyCollection()->create_Clone();
					params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TRANSLATION_Z_OFFSET" ), target_elevation );
					params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "TEXTURE_PALETTE_INDEX" ), 0 );
					params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "PITCH" ), 0.0 );
					params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "CAMERA_FIELD_OF_VIEW" ), vfov ); // Horizontal, Vertical, Diagonal?   (aspect is controlled by the attched image/feed)
					params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "CAMERA_NEAR_PLANE" ), nearp );
					params.representation_default_parameters->put_Property( params.representation_default_parameters->get_DefinitionCollection()->get_IndexByName( "CAMERA_FAR_PLANE" ), farp );
					projected_image_rep_ = projected.lock()->get_RepresentationLayerCollection()->create( point_layer, params );

					Geoweb3d::IGW3DImageCollectionPtr collection_token = sdk_context_->get_SceneGraphContext()->create_ImageCollection();
					collection_token->create( "../examples/media/test_images/one.png" );
					projected_image_rep_.lock()->put_GW3DFinalizationToken( collection_token->create_FinalizeToken() );
				}
			}
        }
        else
        {
            printf("viewshed destroyed\n");
			Geoweb3d::IGW3DPerspectiveViewshedTestCollection::close( viewshed_test_ );
			Geoweb3d::IGW3DVectorRepresentationCollection::close( projected_image_rep_ );
        }
	}
