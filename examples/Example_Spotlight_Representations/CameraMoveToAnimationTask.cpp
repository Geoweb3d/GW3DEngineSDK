#include "CameraMoveToAnimationTask.h"
#include "MiniEngine.h"

#include "engine/GW3DCamera.h"

#include "engine/IGW3DWindowCoordinateToGeodeticQuery.h"
#include "Geoweb3dCore\GeometryExports.h"

    CameraMoveToAnimationTask::CameraMoveToAnimationTask( Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr query, MiniEngine *pengine) : mouse_wheel_delta_(0),delta_set_(false), query_(query), pengine_(pengine)
    {
        query_.lock()->put_Enabled( true );
    }

    CameraMoveToAnimationTask::~CameraMoveToAnimationTask()
    {

    }

    void CameraMoveToAnimationTask::ResetQuery(int x, int y)
    {
        query_.lock()->put_WindowCoordinate(x, y);
        query_.lock()->put_Enabled( true );
    }

    void CameraMoveToAnimationTask::RunPreDraw()
    {
        if(query_.expired()) return;
    }

    void CameraMoveToAnimationTask::MouseWheelDelta( float delta )
    {
        mouse_wheel_delta_ = delta;
        delta_set_ = true;
    }

	void CameraMoveToAnimationTask::RunPostDraw()
	{
		if(query_.expired()) return;

		if(query_.lock()->get_Enabled() && query_.lock()->get_IsValid())
		{
			//make sure the current (the last end) is now the current start animation position
			// pengine_->camera_animation_start_location = pengine_->camera_animation_end_location;

			Geoweb3d::GW3DPoint cloc =  *(pengine_->camera_controller_->get_Location( ));

			float heading,pitch,roll;
			pengine_->camera_controller_->get_Rotation(heading,pitch,roll );

			pengine_->camera_animation_start_location.longitude = cloc.get_X();
			pengine_->camera_animation_start_location.latitude  = cloc.get_Y();
			pengine_->camera_animation_start_location.elevation = cloc.get_Z();
			pengine_->camera_animation_start_location.heading = heading;
			pengine_->camera_animation_start_location.pitch   = pitch;
			pengine_->camera_animation_start_location.roll    =  roll;


			//you set the end location, as this is where you want to end up when
			//the animation is completed
			pengine_->camera_animation_end_location.longitude = query_.lock()->get_GeodeticResult()->get_X();
			pengine_->camera_animation_end_location.latitude  = query_.lock()->get_GeodeticResult()->get_Y();
			pengine_->camera_animation_end_location.elevation = cloc.get_Z();


			pengine_->camera_animation_end_location.heading = heading;
			pengine_->camera_animation_end_location.pitch   = pitch;
			pengine_->camera_animation_end_location.roll    =  roll;


			if(pengine_->camera_animation_end_location.elevation < .01)  pengine_->camera_animation_end_location.elevation = .01;

			pengine_->camera_animation_timer_ = 0; //this sets us off!
		}


		if(!query_.expired())
		{
			query_.lock()->put_Enabled(false);
		}

	}
