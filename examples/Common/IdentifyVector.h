// IdentifyVector.h

#include <stdio.h>
#include <iostream>

#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DWindowCollection.h"
#include "engine/IGW3DCamera.h"
#include "engine/IGW3DCameraCollection.h"
#include "engine/IGW3DWindow.h"
#include "engine/IGW3DVectorDataSource.h"
#include "engine/IGW3DVectorRepresentationCollection.h"
#include "engine/GW3DWindow.h"
#include "common/GW3DCommon.h"
#include "engine/IGW3DGeoweb3dSDK.h"

// include the error handeling 
#include "Geoweb3dCore/SystemExports.h"

// adds - start
#include "engine/IGW3DLineSegmentIntersectionTestCollection.h"
#include "engine/IGW3DLineSegmentIntersectionTest.h"
#include "engine/IGW3DLineSegmentIntersectionReport.h"
#include "engine/IGW3DPositionOrientation.h"
#include "engine/IGW3DCameraController.h"
#include "engine/IGW3DVectorRepresentationDriver.h"
#include "engine/IGW3DVectorRepresentationDriverCollection.h"
#include "engine/igw3dvectorrepresentationhelpercollection.h"
#include "engine/IGW3DVectorRepresentation.h"
#include "engine/IGW3DLineSegmentIntersectionDetail.h"



// adds = end

#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

class IdentifyVector 
{
	public:
		IdentifyVector(Geoweb3d::IGW3DGeoweb3dSDKPtr t_sdk_context);
		virtual ~IdentifyVector();
		void addAllVectorRepresentationToTest();		
		void displayVectorRepresentationInterceptReport();
protected:
		bool ProcessEvent(const Geoweb3d::WindowEvent& win_event,Geoweb3d::IGW3DWindowWPtr &p);
	private:
		Geoweb3d::IGW3DGeoweb3dSDKPtr sdk_context_;
		Geoweb3d::IGW3DLineSegmentIntersectionTestWPtr line_segment_;
		Geoweb3d::IGW3DCameraWPtr selected_camera_;
		bool possible_identify_action_to_take_place_;
};
