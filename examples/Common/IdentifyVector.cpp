// IdentifyVector.cpp
#include "IdentifyVector.h"
#include "engine/IGW3DLineSegmentIntersectionDetailCollection.h"
#include "engine/IGW3DVectorLayer.h"
#include "engine/IGW3DVectorRepresentation.h"
#include "engine/IGW3DVectorRepresentationDriver.h"

IdentifyVector::IdentifyVector(Geoweb3d::IGW3DGeoweb3dSDKPtr t_sdk_context) 
: sdk_context_(t_sdk_context)
, possible_identify_action_to_take_place_(false)
{
	line_segment_ = sdk_context_->get_LineSegmentIntersectionTestCollection()->create();
}

IdentifyVector::~IdentifyVector()
{
}

bool IdentifyVector::ProcessEvent(const Geoweb3d::WindowEvent& win_event, Geoweb3d::IGW3DWindowWPtr &p)
{		
	bool retval = false;

	switch( win_event.Type )
    {
	case win_event.MouseMoved:
		line_segment_.lock()->put_Enabled( false ); //they moved the mouse, so they are probably trying to move in he 3d scene and not identify
		possible_identify_action_to_take_place_ = false;
		break;
		
	case win_event.MouseButtonPressed:
		
		if (win_event.MouseButton.button == Geoweb3d::Mouse::Left)
		{ 
			possible_identify_action_to_take_place_ = true;
		}
		break;
		
	case win_event.MouseButtonReleased: 
		{				
			if (win_event.MouseButton.button == Geoweb3d::Mouse::Left && possible_identify_action_to_take_place_ )
			{					
				line_segment_.lock()->put_Enabled( true );
				selected_camera_ = p.lock()->get_TopCamera(win_event.MouseButton.x, win_event.MouseButton.y);
				if ( !selected_camera_.expired() ) 
				{
					Geoweb3d::IGW3DCameraController* w_camera_controller = selected_camera_.lock()->get_CameraController();
				
					int win_x, win_y;
					unsigned int win_width, win_height;
					p.lock()->get_WindowSize(win_x,win_y,win_width,win_height);

					double w_lon;
					double w_lat;
					double w_elev;
					Geoweb3d::IGW3DPositionOrientation::AltitudeMode w_mode;
					Geoweb3d::GW3DPoint camera_point;

					w_camera_controller->get_Location(w_lon, w_lat);
					w_camera_controller->get_Elevation(w_elev,w_mode);
				
					camera_point.put_X(w_lon);
					camera_point.put_Y(w_lat);
					camera_point.put_Z(w_elev);

					Geoweb3d::GW3DPoint ray_start;
					Geoweb3d::GW3DPoint ray_end;							

					w_camera_controller->get_Ray( win_event.MouseButton.x , win_event.MouseButton.y,ray_start,ray_end);
						
					line_segment_.lock()->put_StartEnd(camera_point,ray_end);
				}
			}				
		}
		break;

		default:
		break;
	};

	return retval;
}

void IdentifyVector::addAllVectorRepresentationToTest()
{
	Geoweb3d::IGW3DVectorRepresentationDriverWPtr w_driver_ptr;
	
	sdk_context_->get_VectorRepresentationDriverCollection()->reset();
	
	while(sdk_context_->get_VectorRepresentationDriverCollection()->next(&w_driver_ptr))
	{ 
		// loop through the drivers
		Geoweb3d::IGW3DVectorRepresentationWPtr w_representation_ptr;
		w_driver_ptr.lock()->get_RepresentationLayerCollection()->reset();
		
		while(w_driver_ptr.lock()->get_RepresentationLayerCollection()->next(&w_representation_ptr))
		{
			line_segment_.lock()->get_VectorRepresentationCollection()->add(w_representation_ptr);
		}
	}	
}

void IdentifyVector::displayVectorRepresentationInterceptReport()
{
	if(line_segment_.lock()->get_Enabled( ))
	{
		Geoweb3d::IGW3DLineSegmentIntersectionReportCollection* intersection_report_collection = line_segment_.lock()->get_IntersectionReportCollection();

		Geoweb3d::IGW3DLineSegmentIntersectionReport* intersection_report;

		intersection_report_collection->reset();

		if (intersection_report_collection->count())
		{
			std::cout << "** IDENTIFY INTERSECTION DETECTED\n";
		}
		while( intersection_report_collection->next( &intersection_report ) )
		{		
		   
			Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* details_collection = intersection_report->get_IntersectionDetailCollection();

			Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;

			details_collection->reset();
									
			

			while( details_collection->next( &intersection_detail ) )
			{
				std::cout << "\tVectorLayer [" << intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name() << \
							"] Representation Driver [" << intersection_report->get_VectorRepresentation().lock()->get_Driver().lock()->get_Name() << "]\n";
				
				std::cout << "\t\tIntersection Objct ID = [" << intersection_detail->get_ObjectID() << "] \n";
				std::cout << "\t\tDistance = [" << intersection_detail->get_IntersectionDistance() << "] \n";	
				std::cout << "\t\tFrameCount = [" << intersection_detail->get_FrameTimeStamp() << "] \n";
			}

			
		}	
		
		std::cout << std::endl;
		
		line_segment_.lock()->put_Enabled(false);
		line_segment_.lock()->put_RemoveCamera(selected_camera_); 
	}
 		
}
