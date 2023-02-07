#include "stdafx.h"

#include "Mouse3DQuery.h"

#include "gui/GW3DGUICommon.h"
#include "gui/IGW3DGUIView3d.h"
#include "gui/IGW3DGUIStatusOverlay.h"
#include "gui/GW3DGUIVector.h"
#include "gui/GW3DGUIRaster.h"

Mouse3DQuery::Mouse3DQuery(MyCompanyExtension* app, Geoweb3d::GUI::IGW3DGUIStartContext *ctx)
	:	app_(app)
	,	single_shot_query_mode_(false)
	,	flash_on_hover_(false)
	,	last_fid_(-1)
	,	clicked_(false)
{
	mouse_query_test_ = ctx->get_LineSegmentIntersectionTestCollection()->create();
	mouse_query_2d_test_ = ctx->get_2DIntersectionTestCollection()->create();
}

Mouse3DQuery::~Mouse3DQuery()
{}

void Mouse3DQuery::SetRepresentationQueriesEnabled(bool enable)
{
	// If false, we will not receive line segment intersection query results
	mouse_query_test_.lock()->put_Enabled(enable);
	mouse_query_2d_test_.lock()->put_Enabled(enable);
}

bool Mouse3DQuery::GetRepresentationQueriesEnabled()
{
	return mouse_query_test_.lock()->get_Enabled();
}

void Mouse3DQuery::AddRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr rep)
{
	if (rep.lock()->get_Driver().lock()->get_Is2d())
	{
		mouse_query_2d_test_.lock()->get_VectorRepresentationCollection()->add(rep);
	}
	else
	{
		mouse_query_test_.lock()->get_VectorRepresentationCollection()->add(rep);
	}
}

void Mouse3DQuery::AddRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr rep)
{
	if (rep.lock()->get_Driver().lock()->get_Is2d())
	{
		mouse_query_2d_test_.lock()->get_VectorRepresentationBasicCollection()->add(rep);
	}
	else
	{
		mouse_query_test_.lock()->get_VectorRepresentationBasicCollection()->add(rep);
	}
}

void Mouse3DQuery::RemoveRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr rep)
{
	if (rep.lock()->get_Driver().lock()->get_Is2d())
	{
		mouse_query_2d_test_.lock()->get_VectorRepresentationCollection()->remove(rep);
	}
	else
	{
		mouse_query_test_.lock()->get_VectorRepresentationCollection()->remove(rep);
	}
}

void Mouse3DQuery::RemoveRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr rep)
{
	if (rep.lock()->get_Driver().lock()->get_Is2d())
	{
		mouse_query_2d_test_.lock()->get_VectorRepresentationBasicCollection()->remove(rep);
	}
	else
	{
		mouse_query_test_.lock()->get_VectorRepresentationBasicCollection()->remove(rep);
	}
}

void Mouse3DQuery::PostDraw (Geoweb3d::GUI::IGW3DGUIPostDrawContext *ctx)
{
	if (ctx->get_View().expired())
	{
		return;
	}

	//update to eye location for the frame just drawn
	eye_location_ = ctx->get_View().lock()->get_Location();

	UpdateGeographicMousePosition (ctx->get_View());

	double distance_to_terrain = 0;
	Geoweb3d::GW3DResult success = mouse_geoposition_over_terrain_.get_DistanceInMeters(eye_location_, true, distance_to_terrain);

	// For a use case where you are running many intersection tests at once
	// (currently you are limited to 9, but this restriction will be lifted in the future),
	// it's best to call get_IntersectionResults() for a collection of only those tests
	// that had intersection results in the last draw.
	// (So out of 1000 tests we might only need to loop through a handful.)
	Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionTestResultsCollection* intersection_results = ctx->get_LineSegmentIntersectionTestCollection()->get_IntersectionResultsCollection();
	for (unsigned i=0; i<intersection_results->count(); i++)
	{
		Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionTestWPtr a_test_with_results = intersection_results->get_AtIndex(i);
		int number_of_representations_intersected = a_test_with_results.lock()->get_IntersectionReportCollection()->count();
	}

	// For the use case of picking, where we want to focus on a single 
	// intersection test, we can just use our test directly
	
	bool feature_hit = false;

	if (!single_shot_query_mode_ || clicked_)
	{
		if (mouse_query_test_.lock()->get_Enabled())
		{
			if (mouse_query_test_.lock()->get_IsValid())
			{
				//This is the list of all representations that had intersections
				Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReportCollection* query_results_ = mouse_query_test_.lock()->get_IntersectionReportCollection();
				Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReport* intersection_report = 0;
				query_results_->reset();
				while (query_results_->next(&intersection_report))
				{
					feature_hit = true;
					std::string click_message; // send message to log window and on-screen overlay;
					switch(intersection_report->get_RepresentationClass())
					{
					case Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReport::VectorRepresentation:
						click_message = std::string("Mouse intersection: ") + intersection_report->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();
						ctx->get_View().lock()->get_StatusOverlay()->put_Text(click_message.c_str()); 
						ctx->get_View().lock()->get_StatusOverlay()->put_Visible(true);  //click 'x' to hide, but you could also call put_Visible(false) after some time
						app_->LogMessage(click_message.c_str());
						break;
					case Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReport::VectorRepresentationBasic:
						click_message = std::string("Mouse intersection: ") + intersection_report->get_VectorRepresentationBasic().lock()->get_VectorLayer().lock()->get_Name();
						ctx->get_View().lock()->get_StatusOverlay()->put_Text(click_message.c_str());
						ctx->get_View().lock()->get_StatusOverlay()->put_Visible(true); //click 'x' to hide, but you could also call put_Visible(false) after some time
						app_->LogMessage(click_message.c_str());
					}
					//This is the list of all intersection details for the representation (i.e. each feature that was intersected)
					Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* intersection_details = intersection_report->get_IntersectionDetailCollection();
					//We sort all the intersected features from nearest to the eye to farthest from the eye
					intersection_details->do_Sort(Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection::Ascending);
					Geoweb3d::IGW3DLineSegmentIntersectionDetail* intersection_detail;
					bool nearest = true;
					intersection_details->reset();
					while (intersection_details->next(&intersection_detail))
					{
						// Note that in most cases you will see more than one intersection per feature,
						// because often the line segment intersects a model (for example) in more than
						// one place.  (i.e. on the way in and on the way out)
						app_->LogMessage("   FID [%d] - Meters from eye: %5.2f"
							, intersection_detail->get_ObjectID()
							, intersection_detail->get_IntersectionDistance());
						double terrain_elevation_at_feature = 
							ctx->get_RasterRepresentationDriverCollection()->get_Elevation(
								intersection_detail->get_IntersectionPoint()->get_Y(),
								intersection_detail->get_IntersectionPoint()->get_X(),
								true);
						
						app_->LogMessage("   Elevation under feature: %5.2f meters"
							, terrain_elevation_at_feature);
						if (distance_to_terrain < intersection_detail->get_IntersectionDistance() )
						{
							app_->LogMessage("   Note: This feature is behind the terrain!");
						}
						//only flash if 'flash on hover' mode is on, and if it's a different feature from last time
						if (nearest && flash_on_hover_ && last_fid_ != intersection_detail->get_ObjectID())
						{
							last_fid_ = intersection_detail->get_ObjectID();
							//flash the closest feature
							switch(intersection_report->get_RepresentationClass())
							{
							case Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReport::VectorRepresentation:
								intersection_report->get_VectorRepresentation().lock()->flash_Feature(intersection_detail->get_ObjectID());
								break;
							case Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReport::VectorRepresentationBasic:
								intersection_report->get_VectorRepresentationBasic().lock()->flash_Feature(intersection_detail->get_ObjectID());
								break;
							}
						}
						nearest = false;
						break; //remove this break if you want to see all intersections, not just the nearest
					
					}
				}
			}
		}

		//TODO: evaluate the 2D analysis as well
		if (mouse_query_2d_test_.lock()->get_Enabled())
		{
			if (mouse_query_2d_test_.lock()->get_IsValid())
			{
				//This is the list of all representations that had intersections
				Geoweb3d::GUI::IGW3DGUI2DIntersectionCollection* query_results_ = mouse_query_2d_test_.lock()->get_IntersectionCollection();
				Geoweb3d::GUI::IGW3DGUI2DIntersection* intersection;
				query_results_->reset();
				while (query_results_->next(&intersection))
				{
					feature_hit = true;
					//app_->LogMessage("");
					switch(intersection->get_RepresentationClass())
					{
					case Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReport::VectorRepresentation:
						app_->LogMessage("Mouse intersection: %s", intersection->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name());
						break;
					case Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionReport::VectorRepresentationBasic:
						app_->LogMessage("Mouse intersection: %s", intersection->get_VectorRepresentationBasic().lock()->get_VectorLayer().lock()->get_Name());
						break;
					}
					//This is the list of all intersection details for the representation (i.e. each feature that was intersected)
					Geoweb3d::GUI::IGW3DGUI2DIntersectionDetailCollection* intersection_details = intersection->get_IntersectionDetailCollection();
					//We sort all the intersected features from nearest to the eye to farthest from the eye
					intersection_details->do_Sort(Geoweb3d::GUI::IGW3DGUI2DIntersectionDetailCollection::Ascending);
					Geoweb3d::GUI::IGW3DGUI2DIntersectionDetail* intersection_detail;
					bool nearest = true;
					intersection_details->reset();
					while (intersection_details->next(&intersection_detail))
					{
						// Note that in most cases you will see more than one intersection per feature,
						// because often the line segment intersects a model (for example) in more than
						// one place.  (i.e. on the way in and on the way out)
						app_->LogMessage("   FID [%d] - Meters from eye: %5.2f"
							, intersection_detail->get_ObjectID()
							, intersection_detail->get_Distance());
						nearest = false;
						break; //remove this break if you want to see all 2D intersections, not just the nearest
					}
				}
			}
		}
	}

	if (!feature_hit)
	{
		last_fid_ = -1;
	}

	clicked_ = false;
	
}

void Mouse3DQuery::PreDraw (Geoweb3d::GUI::IGW3DGUIPreDrawContext *ctx)
{

	if (ctx->get_View().expired())
	{
		return;
	}
	//update to new location for the next frame
	eye_location_ = ctx->get_View().lock()->get_Location();

	//Get the window coordinates of the mouse
	int pixel_x;
	int pixel_y;
	ctx->get_View().lock()->get_MouseWindowCoordinate(pixel_x, pixel_y);

	//From the window coordinates, get the 3D locations that the mouse pointer intersects the near and far plane
	ctx->get_View().lock()->get_Ray(pixel_x, pixel_y, mouse_near_plane_intersection_location_, mouse_far_plane_intersection_location_);

	//In the next draw, we will query all 3D feature representations (models, etc.)
	//between the eyepoint and the far plane 
	//Note that this is just one use case - we could also 
	mouse_query_test_.lock()->put_End(mouse_far_plane_intersection_location_);
	mouse_query_test_.lock()->put_Start(*eye_location_);

	mouse_query_2d_test_.lock()->put_WindowCoordinates(pixel_x, pixel_y);
}

void Mouse3DQuery::UpdateGeographicMousePosition( Geoweb3d::GUI::IGW3DGUIView3dWPtr view3d)
{
	mouse_hovering_over_geometry_ = view3d.lock()->get_GeometryMousePosition(mouse_geoposition_over_geometry_);
	mouse_hovering_over_terrain_ = view3d.lock()->get_TerrainMousePosition(mouse_geoposition_over_terrain_);

	if (mouse_hovering_over_terrain_)
	{
		//dw_->LogMessage("The mouse was hovering over terrain in the most recent frame.");
		//dw_->LogMessage("The mouse position over terrain is lat: %5.2f, lon: %5.2f, alt: %5.2f.",
		//	mouse_geoposition_over_terrain_.get_Y(), mouse_geoposition_over_terrain_.get_X(), mouse_geoposition_over_terrain_.get_Z());
	}


	if (mouse_hovering_over_geometry_)
	{
		//dw_->LogMessage("The mouse was hovering over geometry in the most recent frame.");
		//dw_->LogMessage("The mouse position over geometry is lat: %5.2f, lon: %5.2f, alt: %5.2f.",
		//	mouse_geoposition_over_geometry_.get_Y(), mouse_geoposition_over_geometry_.get_X(), mouse_geoposition_over_geometry_.get_Z());
	}

}
