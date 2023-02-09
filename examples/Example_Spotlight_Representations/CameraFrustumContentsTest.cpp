
#include "CameraFrustumContentsTest.h"

#include "engine/GW3DCamera.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DFrameAnalysis.h"

    CameraFrustumContentsTest::CameraFrustumContentsTest() 
	{}

    CameraFrustumContentsTest::~CameraFrustumContentsTest() 
	{}

    bool CameraFrustumContentsTest::OnStream( Geoweb3d::IGW3DContentsResult *contents, Geoweb3d::IGW3DFeatureFeedbackResultCollection *featurefeedback )
    {

		return false;

		//The fact we got the event on this camera for this representation means data is paged in within
		//your page level settings. Thus, is visible would mean the camera is looking at the bounding box area
		//of the paged in data. Else the data is paged in, but the camera is not looking/intersecting at the
		//bounding box of the representation data.
		if( contents->get_VectorRepresentationIsVisible() ) 
		{
			
			
			printf("Window: %s\n",    contents->get_Camera().unsafe_get()->get_Window().unsafe_get()->get_Name() );
			printf("\tCamera: %s\n",   contents->get_Camera().unsafe_get()->get_Name());
			printf("\t\tRepresentation: %s\n", contents->get_VectorRepresentation().unsafe_get()->get_Driver().unsafe_get()->get_Name());
			printf("\t\tIs In Frustum: %s\n", contents->get_VectorRepresentationIsVisible() ? "true":"false");
			printf("\t\tVector Layer: %s\n", contents->get_VectorRepresentation().unsafe_get()->get_VectorLayer().unsafe_get()->get_Name());

			static const bool print_details = true;
            
			if (print_details && featurefeedback->count())
			{
				printf("\t\t\tFeature Feedback Results: %d\n", featurefeedback->count() );

				Geoweb3d::IGW3DFeatureFeedbackResult  *val = 0;
				if(featurefeedback->count() > 1) //to protect from a divide by 0 with the rand()%
				{
            		int minc = 0;
            		int maxc = featurefeedback->count() - 1;
            		val = featurefeedback->get_AtIndex( rand()%(maxc-minc) + minc  );
				}
				else
				{
            		val = featurefeedback->get_AtIndex( 0 );
				}

			
				printf("\t\t\tObject ID: %d\n", val->get_ObjectID());
				printf("\t\t\tDistance from camera: %f\n", val->get_DistanceFromCamera());
				printf("\t\t\tIs Occluded by Earth: %s\n", val->get_OccludedByGlobe() ? "true":"false");
				printf("\t\t\tRendered In 3D Scene: %s\n", val->get_Rendered() ? "true":"false");
				printf("\t\t\tApprox number of pixels: %f\n", val->get_ApproximateNumPixelsOnScreen());
		
				if(val->get_FeatureID2DPixelLocationResult()->get_WasEnabled())
				{
					float nx,ny;
					val->get_FeatureID2DPixelLocationResult()->get_NormalizedCoordinate(nx,ny);
					printf("\t\t\t\tNormalized wc: width %f height %f\n", nx, ny);
					int pixel_x,pixel_y;
					val->get_FeatureID2DPixelLocationResult()->get_WindowCoordinate( pixel_x, pixel_y);
					printf("\t\t\t\tPixel wc: width %d height %d\n", pixel_x, pixel_y );		
				}

				//for(int i = 0; i < featurefeedback->count();++i)
				//{
				//	Geoweb3d::IGW3DFeatureFeedbackResult *val = featurefeedback->get_AtIndex(i);

				//	printf("\t\t\tObject ID: %d\n", val->get_ObjectID());
				//	printf("\t\t\tDistance from camera: %f\n", val->get_DistanceFromCamera());
				//	printf("\t\t\tIs Occluded by Earth: %s\n", val->get_OccludedByGlobe() ? "true":"false");
				//	printf("\t\t\tRendered In 3D Scene: %s\n", val->get_Rendered() ? "true":"false");
				//	printf("\t\t\tAprox number of pixels: %f\n", val->get_ApproximateNumPixelsOnScreen());
			
				//	if(val->get_FeatureID2DPixelLocationResult()->get_WasEnabled())
				//	{
				//		float nx,ny;
				//		val->get_FeatureID2DPixelLocationResult()->get_NormalizedCoordinate(nx,ny);
				//		printf("\t\t\t\tNormalized wc: width %f height %f\n", nx, ny);
				//		int pixel_x,pixel_y;
				//		val->get_FeatureID2DPixelLocationResult()->get_WindowCoordinate( pixel_x, pixel_y);
				//		printf("\t\t\t\tPixel wc: width %d height %d\n", pixel_x, pixel_y );		
				//	}
				//}

				/* TIP: This information can be used for displaying screenspace overlays 
					(i.e. icons) as long as you run the stream inside the Draw2D callback. 
					For efficiency, use an existing representation like models or 
					shapes.  Otherwise, use the "Custom" representation. */
			}

		}
		else
		{
			printf("\t\tRepresentation: %s: Paged in but not within the configured frustum settings\n", 
				contents->get_VectorRepresentation().unsafe_get()->get_Driver().unsafe_get()->get_Name());

		}


		

  //      if (print_details)
		//{
		//	printf("%s\n",   camera.lock()->get_Window().lock()->get_Name() );
		//	printf("%s\n",   camera.lock()->get_Name());
		//	printf("\t%s\n", rep.lock()->get_Driver().lock()->get_Name());
		//	printf("\t%s\n", rep.lock()->get_VectorLayer().lock()->get_Name());
		//	printf("\tTotal contents: %d\n", result->count() );
		//}

  //      if( result->count() )
  //      {
  //          if (print_details)
		//	{
		//		Geoweb3d::IGW3DCameraFrustumContentsStreamResult *val = 0;
		//		if(result->count() > 1) //to protect from a divide by 0 with the rand()%
		//		{
  //          		int minc = 0;
  //          		int maxc = result->count() - 1;
  //          		val = result->get_AtIndex( rand()%(maxc-minc) + minc  );
		//		}
		//		else
		//		{
  //          		val = result->get_AtIndex( 0 );
		//		}

		//		printf("\tObject ID: %d\n", val->get_ObjectID());
		//		printf("\tdistance from camera: %f\n", val->get_DistanceFromCamera());
		//		float nx,ny;
		//		val->get_NormalizedCoordinate(nx,ny);
		//		printf("\tNormalized wc: width %f height %f\n", nx, ny);
		//		int pixel_x,pixel_y;
		//		val->get_WindowCoordinate( pixel_x, pixel_y);
		//		printf("\tPixel wc: width %d height %d\n", pixel_x, pixel_y );

		//		/* TIP: This information can be used for displaying screenspace overlays 
		//			(i.e. icons) as long as you run the stream inside the Draw2D callback. 
		//			For efficiency, use an existing representation like models or 
		//			shapes.  Otherwise, use the "Custom" representation. */
		//	}

  //      }
  //      else
  //      {
  //          //the only time you should be here is if details are off!
  //          //This is just a check as an example, as its not needed in real life.
  //          bool details =rep.lock()->get_CameraFrustumContentsConfiguration()->get_EnableObjectIDWindowCoordinate();

  //          if(details)
  //          {
  //              printf("bug detected?\n");
  //          }
  //          else
  //          {
  //              //Just doing a one shot to know when this actually entered
  //              //into the 3d scene.  Its ok to turn off the rep here
  //              rep.lock()->get_CameraFrustumContentsConfiguration()->get_ClampRadius()->put_Enabled (false );
  //              printf("Performed a one shot, as we know this entered the scene for the 1st time...\n");
  //          }

  //      }

        return true; //go through all the representations and drivers (doing the first of each)
    }

    bool CameraFrustumContentsTest::OnError(/*todo*/ )
    {
		return true;
    }
