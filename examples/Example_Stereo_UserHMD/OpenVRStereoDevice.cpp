#include "stdafx.h"
#include "Util.h"
#include "OpenVRStereoDevice.h"

//--------------------1---------------------------------------------------------
// Purpose: helper to get a string from a tracked device property and turn it into a std::string
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceString( vr::IVRSystem* pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = nullptr )
{
	std::uint32_t requiredBufferLen = pHmd->GetStringTrackedDeviceProperty( unDevice, prop, nullptr, 0, peError );
	if ( requiredBufferLen == 0 )
		return "";

	std::string pchBuffer( requiredBufferLen, ' ' );
	requiredBufferLen = pHmd->GetStringTrackedDeviceProperty( unDevice, prop, &pchBuffer[0], requiredBufferLen, peError );
	return pchBuffer;
}

//-----------------------------------------------------------------------------
// Purpose: helper to get a string from a tracked device type class
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceClassString( vr::ETrackedDeviceClass td_class )
{
	std::string str_td_class = "Unknown class";

	switch ( td_class )
	{
	case vr::TrackedDeviceClass_Invalid:			// = 0, the ID was not valid.
		str_td_class = "invalid";
		break;
	case vr::TrackedDeviceClass_HMD:				// = 1, Head-Mounted Displays
		str_td_class = "hmd";
		break;
	case vr::TrackedDeviceClass_Controller:			// = 2, Tracked controllers
		str_td_class = "controller";
		break;
	case vr::TrackedDeviceClass_GenericTracker:		// = 3, Generic trackers, similar to controllers
		str_td_class = "generic tracker";
		break;
	case vr::TrackedDeviceClass_TrackingReference:	// = 4, Camera and base stations that serve as tracking reference points
		str_td_class = "base station";
		break;
	case vr::TrackedDeviceClass_DisplayRedirect:	// = 5, Accessories that aren't necessarily tracked themselves, but may redirect video output from other tracked devices
		str_td_class = "display redirect";
		break;
	}

	return str_td_class;
}

//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
Matrix4x4 ConvertVRMatrixToMatrix4x4( const vr::HmdMatrix34_t &matPose )
{
	Matrix4x4 matrixObj;

	for ( unsigned int col = 0; col < 4; ++col )
	{
		for ( unsigned int row = 0; row < 3; ++row )
		{
			int index = ( col * 4 ) + row;
			matrixObj.data[index] = matPose.m[row][col];
		}
	}

	matrixObj.data[3] = 0.0;
	matrixObj.data[7] = 0.0;
	matrixObj.data[11] = 0.0;
	matrixObj.data[15] = 1.0;
	return matrixObj;
}

Matrix4x4 ConvertVRMatrixToMatrix4x4( const vr::HmdMatrix44_t &matPose )
{
	Matrix4x4 matrixObj;

	for ( unsigned int col = 0; col < 4; ++col )
	{
		for ( unsigned int row = 0; row < 4; ++row )
		{
			int index = ( col * 4 ) + row;
			matrixObj.data[index] = matPose.m[row][col];
		}
	}

	return matrixObj;
}

OpenVRStereoDevice::OpenVRStereoDevice()
{
}

OpenVRStereoDevice::~OpenVRStereoDevice()
{
	vr::VR_Shutdown();
}

bool OpenVRStereoDevice::get_IsSystemSupported() const
{
	return vr::VR_IsRuntimeInstalled();
}

bool OpenVRStereoDevice::get_IsHardwareDetected() const
{
	return vr::VR_IsHmdPresent();
}

bool OpenVRStereoDevice::Initialize( Geoweb3d::IGW3DRenderTargetConfigurationHelper* configuration_helper, Geoweb3d::IGW3DStereoDeviceProjectionMatrixHelper* projection_helper )
{
	if ( ovr_initialized_ )
		return true;

	if ( !get_IsSystemSupported() )
	{
		printf( "SteamVR Runtime was not found. \n" );
		return false;
	}

	if ( !get_IsHardwareDetected() )
	{
		printf( "HMD was not found. \n" );
		return false;
	}

	// call VR_Init and if the returns is HmdError_Init_PathRegistryNotFound or HmdError_Init_InstallationNotFound then SteamVR is not installed & you will need to prompt the user to install SteamVR via steam://run/250820

	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;
	vr_context_  = vr::VR_Init( &eError, vr::VRApplication_Scene );

	if ( vr_context_ == nullptr || eError != vr::VRInitError_None )
	{
		vr_context_ = nullptr;
		printf( "Unable to init VR runtime: %s \n", vr::VR_GetVRInitErrorAsEnglishDescription( eError ) );
		return false;
	}

	ovr_initialized_ =  vr_context_ && eError == vr::VRInitError_None;

	vr::VRCompositor()->SetTrackingSpace( vr::TrackingUniverseStanding );

	// Obtain some basic information given by the runtime
	int base_stations_count = 0;
	for ( uint32_t td = vr::k_unTrackedDeviceIndex_Hmd; td < vr::k_unMaxTrackedDeviceCount; td++ )
	{
		if ( vr_context_->IsTrackedDeviceConnected( td ) )
		{
			vr::ETrackedDeviceClass tracked_device_class = vr_context_->GetTrackedDeviceClass( td );

			std::string td_type = GetTrackedDeviceClassString( tracked_device_class );
			std::string system_name = GetTrackedDeviceString( vr_context_, td, vr::Prop_TrackingSystemName_String );
			std::string model_name = GetTrackedDeviceString( vr_context_, td, vr::Prop_RenderModelName_String );
			tracked_device_type_[td] = td_type;

			printf( "Tracking device %i is connected \n", td );
			printf( "  Device type: %s. Name: %s Model: %s \n", td_type.c_str(), system_name.c_str(), model_name.c_str() );

			if ( tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference )
				base_stations_count++;

			if ( td == vr::k_unTrackedDeviceIndex_Hmd )
			{
				// Fill variables used for obtaining the device name and serial ID (used later for naming the SDL window)
				driver_name_ = GetTrackedDeviceString( vr_context_, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String );
				driver_serial_ = GetTrackedDeviceString( vr_context_, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String );
			}
		}
	}

	// Check whether both base stations are found, not mandatory but just in case...
	if ( base_stations_count < 2 )
	{
		printf( "There was a problem indentifying the base stations, please check they are powered on \n" );
	}

	// Setup Cameras ....
	float near_plane = (float)projection_helper->get_NearPlane();
	float far_plane = (float) projection_helper->get_FarPlane();
	float culling_far_plane = (float) projection_helper->get_CullingFarPlane();

	m_mat4ProjectionLeft_ =  ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Left, near_plane, far_plane ) );
	m_mat4ProjectionRight_ = ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Right, near_plane, far_plane ) );
	m_mat4CullingProjectionLeft_ =  ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Left, near_plane, culling_far_plane ) );
	m_mat4CullingProjectionRight_ = ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Right, near_plane, culling_far_plane ) );
	m_mat4eyePosLeft_ =  ConvertVRMatrixToMatrix4x4( vr_context_->GetEyeToHeadTransform( vr::Eye_Left ) );
	m_mat4eyePosRight_ = ConvertVRMatrixToMatrix4x4( vr_context_->GetEyeToHeadTransform( vr::Eye_Right ) );

	projection_helper->put_ProjectionMatrix( vr::Eye_Left, &m_mat4ProjectionLeft_.data[0], &m_mat4CullingProjectionLeft_.data[0], Geoweb3d::MAO_COL_MAJOR );
	projection_helper->put_ProjectionMatrix( vr::Eye_Right, &m_mat4ProjectionRight_.data[0], &m_mat4CullingProjectionRight_.data[0], Geoweb3d::MAO_COL_MAJOR );

	// Setup Stereo Render Targets
	std::uint32_t render_width = 0;
	std::uint32_t render_height = 0;
	vr_context_->GetRecommendedRenderTargetSize( &render_width, &render_height );

	configuration_helper->put_OverrideSurfaceSize( true );
	configuration_helper->put_Width( render_width );
	configuration_helper->put_Height( render_height );
	configuration_helper->put_Samples( 4 );
	return ovr_initialized_;
}

void OpenVRStereoDevice::UpdateProjection( Geoweb3d::IGW3DStereoDeviceProjectionMatrixHelper* projection_helper ) // MAKE OBJECT
{
	if ( vr_context_ )
	{
		float near_plane = (float)projection_helper->get_NearPlane();
		float far_plane = (float) projection_helper->get_FarPlane();
		float culling_far_plane = (float) projection_helper->get_CullingFarPlane();

		m_mat4ProjectionLeft_ =  ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Left, near_plane, far_plane ) );
		m_mat4ProjectionRight_ = ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Right, near_plane, far_plane ) );
		m_mat4CullingProjectionLeft_ =  ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Left, near_plane, culling_far_plane ) );
		m_mat4CullingProjectionRight_ = ConvertVRMatrixToMatrix4x4( vr_context_->GetProjectionMatrix( vr::Eye_Right, near_plane, culling_far_plane ) );

		projection_helper->put_ProjectionMatrix( vr::Eye_Left, &m_mat4ProjectionLeft_.data[0], &m_mat4CullingProjectionLeft_.data[0], Geoweb3d::MAO_COL_MAJOR );
		projection_helper->put_ProjectionMatrix( vr::Eye_Right, &m_mat4ProjectionRight_.data[0], &m_mat4CullingProjectionRight_.data[0], Geoweb3d::MAO_COL_MAJOR );
	}
}

void OpenVRStereoDevice::UpdateModelview( Geoweb3d::IGW3DStereoDeviceModelViewMatrixHelper* modelview_helper )
{
	if ( vr_context_ )
	{
		m_mat4eyePosLeft_ =  ConvertVRMatrixToMatrix4x4( vr_context_->GetEyeToHeadTransform( vr::Eye_Left ) );
		m_mat4eyePosRight_ = ConvertVRMatrixToMatrix4x4( vr_context_->GetEyeToHeadTransform( vr::Eye_Right ) );

		modelview_helper->put_DeviceRootTransform( &m_mat4HMDPose_.data[0], Geoweb3d::MAO_COL_MAJOR, true );
		modelview_helper->put_PerViewTransform( vr::Eye_Left, &m_mat4eyePosLeft_.data[0], Geoweb3d::MAO_COL_MAJOR );
		modelview_helper->put_PerViewTransform( vr::Eye_Right, &m_mat4eyePosRight_.data[0], Geoweb3d::MAO_COL_MAJOR );

		// right-handed system (distance unit is meters)
		// +x is to the right
		// +y is up
		// -z is forward
		modelview_helper->put_DeviceRightVector( 1.0, 0.0, 0.0 );
		modelview_helper->put_DeviceUpVector( 0.0, 1.0, 0.0 );
		modelview_helper->put_DeviceViewVector( 0.0, 0.0, -1.0 );
	}
}

void OpenVRStereoDevice::ProcessEvents( Geoweb3d::IGW3DStereoDeviceNotificationCallbacks* callbacks )
{
	if ( !vr_context_ )
		return;

	// Process SteamVR events
	vr::VREvent_t vr_event;
	while ( vr_context_->PollNextEvent( &vr_event, sizeof( vr_event ) ) )
	{
		ProcessEvent_( vr_event, callbacks );
	}

	// for somebody asking for the default figure out the time from now to photons.
	float fSecondsSinceLastVsync;
	vr_context_->GetTimeSinceLastVsync( &fSecondsSinceLastVsync, NULL );

	float fDisplayFrequency = vr_context_->GetFloatTrackedDeviceProperty( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float );
	float fFrameDuration = 1.f / fDisplayFrequency;
	float fVsyncToPhotons = vr_context_->GetFloatTrackedDeviceProperty( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float );
	float fPredictedSecondsFromNow = fFrameDuration - fSecondsSinceLastVsync + fVsyncToPhotons;
}

void OpenVRStereoDevice::ProcessEvent_( const vr::VREvent_t& event, Geoweb3d::IGW3DStereoDeviceNotificationCallbacks* callbacks )
{
	std::string str_td_class = GetTrackedDeviceClassString( vr_context_->GetTrackedDeviceClass( event.trackedDeviceIndex ) );
	vr::EVREventType event_type = static_cast<vr::EVREventType>( event.eventType );

	switch ( event_type )
	{
	case vr::VREvent_TrackedDeviceActivated:
	{
		printf("[OpenVR Event] Device %i attached (%s)\n", event.trackedDeviceIndex, str_td_class.c_str() );
		tracked_device_type_[event.trackedDeviceIndex] = str_td_class;

		vr::ETrackedDeviceClass tracked_device_class = vr_context_->GetTrackedDeviceClass(event.trackedDeviceIndex);
		if (tracked_device_class == vr::TrackedDeviceClass_HMD)
		{
			if (callbacks)
				callbacks->OnHmdConnected();
		}
	}
	break;
	case vr::VREvent_TrackedDeviceDeactivated:
	{
		printf("[OpenVR Event] Device %i detached (%s)\n", event.trackedDeviceIndex, str_td_class.c_str() );
		tracked_device_type_[event.trackedDeviceIndex] = "";

		vr::ETrackedDeviceClass tracked_device_class = vr_context_->GetTrackedDeviceClass(event.trackedDeviceIndex);
		if (tracked_device_class == vr::TrackedDeviceClass_HMD)
		{
			if (callbacks)
				callbacks->OnHmdDisconnected();
		}
	}
	break;
	case vr::VREvent_TrackedDeviceUpdated:
	{
		printf("[OpenVR Event] Device %i updated (%s)\n", event.trackedDeviceIndex, str_td_class.c_str() );
	}
	break;
	case vr::VREvent_ButtonPress:
	{
		vr::VREvent_Controller_t controller_data = event.data.controller;
		printf("[OpenVR Event] Pressed button %s of device %i (%s)\n", vr_context_->GetButtonIdNameFromEnum( ( vr::EVRButtonId ) controller_data.button ), event.trackedDeviceIndex, str_td_class.c_str() );
	}
	break;
	case vr::VREvent_ButtonUnpress:
	{
		vr::VREvent_Controller_t controller_data = event.data.controller;
		printf("[OpenVR Event] Unpressed button %s of device %i (%s)\n", vr_context_->GetButtonIdNameFromEnum( ( vr::EVRButtonId ) controller_data.button ), event.trackedDeviceIndex, str_td_class.c_str() );
	}
	break;
	case vr::VREvent_ButtonTouch:
	{
		vr::VREvent_Controller_t controller_data = event.data.controller;
		printf("[OpenVR Event] Touched button %s of device %i (%s)\n", vr_context_->GetButtonIdNameFromEnum( ( vr::EVRButtonId ) controller_data.button ), event.trackedDeviceIndex, str_td_class.c_str() );
	}
	break;
	case vr::VREvent_ButtonUntouch:
	{
		vr::VREvent_Controller_t controller_data = event.data.controller;
		printf("[OpenVR Event] Untouched button %s of device %i (%s)\n", vr_context_->GetButtonIdNameFromEnum( ( vr::EVRButtonId ) controller_data.button ), event.trackedDeviceIndex, str_td_class.c_str() );
	}
	break;
	case vr::VREvent_SeatedZeroPoseReset:
	{
		printf( "[OpenVR Event] SeatedZeroPoseReset \n" );
	}
	break;
	default:
	{
		printf("[OpenVR Event] %s of device %i (%s) \n", vr_context_->GetEventTypeNameFromEnum( event_type ), event.trackedDeviceIndex, str_td_class.c_str() );
	}
	break;

	}
}

void OpenVRStereoDevice::OnDrawBegin()
{
	if ( !vr_context_ )
		return;

	UpdateHMDMatrixPose_();
}

void OpenVRStereoDevice::UpdateHMDMatrixPose_()
{
	vr::EVRCompositorError result = vr::VRCompositor()->WaitGetPoses( tracked_device_pose_, vr::k_unMaxTrackedDeviceCount, NULL, 0 );

	for ( int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice )
	{
		if ( tracked_device_pose_[nDevice].bPoseIsValid )
		{
			tracked_device_matrix_[nDevice] = ConvertVRMatrixToMatrix4x4( tracked_device_pose_[nDevice].mDeviceToAbsoluteTracking );
		}
	}

	if ( tracked_device_pose_[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid )
	{
		m_mat4HMDPose_ = tracked_device_matrix_[vr::k_unTrackedDeviceIndex_Hmd];
	}
}

//UV Min is the upper left corner and UV Max is the lower right corner.
void ViewportToTextureBounds( const Geoweb3d::IGW3DViewport* viewport, int width, int height, vr::VRTextureBounds_t& bounds )
{
	if ( !viewport )
	{
		bounds.uMin = 0.0f;
		bounds.vMin = 0.0f;
		bounds.uMax = 1.0f;
		bounds.vMax = 1.0f;
		return;
	}

	bounds.uMin = viewport->get_X() / (float)width;
	bounds.vMin = (height - (viewport->get_Y() + viewport->get_Height())) / (float)height;

	bounds.uMax = (viewport->get_X() + viewport->get_Width()) / (float)width;
	bounds.vMax = ( height - viewport->get_Y() ) / (float)height;

	bounds.vMax = bounds.vMax;
	bounds.vMin = bounds.vMin;
}

void OpenVRStereoDevice::OnDrawEnd( Geoweb3d::IGW3DRenderTargetAccessor* render_target_accessor ) // MAKE OBJECT - pass in texture ids
{
	if ( !vr_context_ )
		return;

	vr::Texture_t leftEyeTexture;
	leftEyeTexture.handle = (void*)(uintptr_t)render_target_accessor->get_TextureId( vr::Eye_Left );
	leftEyeTexture.eType = vr::TextureType_OpenGL;
	leftEyeTexture.eColorSpace = vr::ColorSpace_Gamma;

	vr::VRTextureBounds_t leftEyeBounds;
	ViewportToTextureBounds( render_target_accessor->get_Viewport( vr::Eye_Left ), render_target_accessor->get_TextureWidth( vr::Eye_Left ), render_target_accessor->get_TextureHeight( vr::Eye_Left ), leftEyeBounds );

	vr::EVRCompositorError lerror = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture, &leftEyeBounds );

	vr::Texture_t rightEyeTexture;
	rightEyeTexture.handle = (void*)(uintptr_t)render_target_accessor->get_TextureId( vr::Eye_Right );
	rightEyeTexture.eType = vr::TextureType_OpenGL;
	rightEyeTexture.eColorSpace = vr::ColorSpace_Gamma;

	vr::VRTextureBounds_t rightEyeBounds;
	ViewportToTextureBounds( render_target_accessor->get_Viewport( vr::Eye_Right ), render_target_accessor->get_TextureWidth( vr::Eye_Right ), render_target_accessor->get_TextureHeight( vr::Eye_Right ), rightEyeBounds );

	vr::EVRCompositorError rerror = vr::VRCompositor()->Submit( vr::Eye_Right, &rightEyeTexture, &rightEyeBounds );

	const bool show_mirror_texture = true;
	if ( show_mirror_texture )
	{
		const Geoweb3d::IGW3DViewport* viewport = render_target_accessor->get_WindowViewport();
		if ( viewport )
		{
			int window_x = viewport->get_X();
			int window_y = viewport->get_Y();
			int window_w = viewport->get_Width();
			int window_h = viewport->get_Height();

			render_target_accessor->CompositeToWindow( vr::Eye_Left, window_x, window_y, window_x + window_w / 2, window_y + window_h );
			render_target_accessor->CompositeToWindow( vr::Eye_Right, window_x + window_w / 2, window_y, window_x + window_w, window_y + window_h );
		}
	}
}

void OpenVRStereoDevice::OnDrawComplete()
{
	if ( !vr_context_ )
		return;

	vr::VRCompositor()->PostPresentHandoff();
}

Geoweb3d::GW3DResult OpenVRStereoDevice::put_TrackingOrigin( Geoweb3d::IGW3DCameraStereoSettings::TrackingOrigin origin )
{
	if ( !vr_context_ )
		return Geoweb3d::GW3D_eInvalidHandle;

	switch ( origin )
	{
	case Geoweb3d::IGW3DCameraStereoSettings::SEATED:
		vr::VRCompositor()->SetTrackingSpace( vr::TrackingUniverseSeated );
		break;

	case Geoweb3d::IGW3DCameraStereoSettings::STANDING:
		vr::VRCompositor()->SetTrackingSpace( vr::TrackingUniverseStanding );
		break;

	default:
		break;
	}

	return Geoweb3d::GW3D_sOk;

}

Geoweb3d::GW3DResult OpenVRStereoDevice::reset_SeatedZeroPose()
{
	if ( !vr_context_ )
		return Geoweb3d::GW3D_eInvalidHandle;

	vr_context_->ResetSeatedZeroPose();
	return Geoweb3d::GW3D_sOk;
}
