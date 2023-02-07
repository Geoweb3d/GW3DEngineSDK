#pragma once

#include "core/GW3DGUIInterFace.h"

#include "Geoweb3dCore\GeometryExports.h"

#include "MyCompanyExtension.h"

class Mouse3DQuery
{
public:

    Mouse3DQuery(MyCompanyExtension* app, Geoweb3d::GUI::IGW3DGUIStartContext *ctx);
    ~Mouse3DQuery();

	// true to query on a mouse click, false to query every frame (i.e. hover)
	void SetClickOnlyMode (bool val) {single_shot_query_mode_ = val;}
	bool GetClickOnlyMode () {return single_shot_query_mode_ ;}

	void Click() {clicked_ = true;};

	void SetRepresentationQueriesEnabled(bool);
	bool GetRepresentationQueriesEnabled();

	void AddRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr);
	void AddRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr);

	void RemoveRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationWPtr);
	void RemoveRepresentation (Geoweb3d::GUI::IGW3DGUIVectorRepresentationBasicWPtr);
   
	void PostDraw (Geoweb3d::GUI::IGW3DGUIPostDrawContext *ctx);
	void PreDraw (Geoweb3d::GUI::IGW3DGUIPreDrawContext *ctx);

	void SetFlashOnHover (bool doFlash) {flash_on_hover_ = doFlash;}

private:

	void UpdateGeographicMousePosition (Geoweb3d::GUI::IGW3DGUIView3dWPtr view3d);

	bool single_shot_query_mode_;
	bool clicked_;

	const Geoweb3d::GW3DPoint* eye_location_;
	Geoweb3d::GW3DPoint mouse_query_target_location_;

	Geoweb3d::GW3DPoint mouse_near_plane_intersection_location_;
	Geoweb3d::GW3DPoint mouse_far_plane_intersection_location_;

	bool mouse_hovering_over_geometry_;
	bool mouse_hovering_over_terrain_;

	Geoweb3d::GW3DPoint mouse_geoposition_over_geometry_;
	Geoweb3d::GW3DPoint mouse_geoposition_over_terrain_;
	
	Geoweb3d::GUI::IGW3DGUILineSegmentIntersectionTestWPtr mouse_query_test_;
	Geoweb3d::GUI::IGW3DGUI2DIntersectionTestWPtr mouse_query_2d_test_;

	MyCompanyExtension* app_;

	bool flash_on_hover_;
	long last_fid_;

};
