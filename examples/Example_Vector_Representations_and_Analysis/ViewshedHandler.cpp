
#include "ViewshedHandler.h"

#include "engine/GW3DCamera.h"

#include "engine/IGW3DPanoramicViewshedTest.h"
#include "engine/IGW3DPanoramicViewshedTestCollection.h"

#include "Geoweb3dCore/GeometryExports.h"
#include "Geoweb3dCore/SystemExports.h"

#include <iostream>


	ViewshedHandler::ViewshedHandler(Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context)
		: sdk_context_ (sdk_context)
	{}

	void ViewshedHandler::ToggleViewshed(Geoweb3d::IGW3DCameraWPtr& camera)
	{
		if(viewshed_test_.expired())
        {
            viewshed_test_ = sdk_context_->get_PanoramicViewshedTestCollection()->create();

			if(!viewshed_test_.expired() )
			{
				//make sure our camera is added to the analysis
				//note, we should try to be smarter and only
				//add when needed, as well as only query the camera
				//position once per frame
				viewshed_test_.lock()->put_AddCamera( camera );
				
				Geoweb3d::IGW3DCameraController* camera_controller = camera.lock()->get_CameraController();

				float yaw,pitch,roll;
				camera_controller->get_Rotation( yaw,pitch,roll);

				//put the viewshed just below the camera, pointing down a bit.
				const Geoweb3d::GW3DPoint *cloc = camera_controller->get_Location( );
				double lon = cloc->get_X();
				double lat = cloc->get_Y();

				viewshed_test_.lock()->put_Location( lon,lat );
				viewshed_test_.lock()->put_Elevation( cloc->get_Z() + 10.0 );
				viewshed_test_.lock()->put_Rotation( yaw, 45.0f ,roll);

			}
        }
        else
        {
            printf("viewshed destroyed\n");
            Geoweb3d::IGW3DPanoramicViewshedTestCollection::close( viewshed_test_ );
        }
	}


	void ViewshedHandler::RunViewshedReport()
	{
		if(!viewshed_test_.expired())
        {
			struct TESTER : public Geoweb3d::IGW3DViewshedContentsStream
			{
				virtual bool OnStream( Geoweb3d::GW3DPolygon *polygon )
				{
					std::cout << polygon->get_NumInteriorRings() << std::endl;
					return true;
				}

				virtual void OnError(Geoweb3d::IGW3DViewshedTest *viewshed)
				{
							
				}
			};

			TESTER tests;

			viewshed_test_.unsafe_get()->stream_Results(&tests);

        }
        else
        {
            printf("no viewshed created to export.  V creates a viewshed\n");
        }
	}