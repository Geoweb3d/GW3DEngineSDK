#pragma once

#include <Geoweb3d/engine/IGW3DStereoDevice.h>
#include <openvr.h>

struct Matrix4x4
{
	double data[16];
};

class OpenVRStereoDevice : public Geoweb3d::IGW3DStereoDevice
{
public:
	OpenVRStereoDevice();
	~OpenVRStereoDevice();

	// Geoweb3d::IGW3DStereoDevice
	virtual const char* get_Name() const override { return "External OpenVR Supported Device"; }
	virtual unsigned int get_NumberOfSupportedViews() const override { return 2; }
	virtual bool get_RequiresHardwareSupport() const override { return true; }
	virtual bool get_IsSystemSupported() const override;
	virtual bool get_IsHardwareDetected() const override;

	// Geoweb3d::IGW3DStereoDeviceCallbacks
	virtual bool get_IsInitialized() const override { return ovr_initialized_; }
	virtual bool Initialize( Geoweb3d::IGW3DRenderTargetConfigurationHelper* configuration_helper, Geoweb3d::IGW3DStereoDeviceProjectionMatrixHelper* projection_helper ) override;
	virtual void UpdateProjection( Geoweb3d::IGW3DStereoDeviceProjectionMatrixHelper* projection_helper ) override;
	virtual void UpdateModelview( Geoweb3d::IGW3DStereoDeviceModelViewMatrixHelper* modelview_helper  ) override;
	virtual void ProcessEvents( Geoweb3d::IGW3DStereoDeviceNotificationCallbacks* callbacks ) override;
	virtual void OnDrawBegin() override;
	virtual void OnDrawEnd( Geoweb3d::IGW3DRenderTargetAccessor* render_target_accessor ) override;
	virtual void OnDrawComplete() override;
	virtual Geoweb3d::GW3DResult put_TrackingOrigin( Geoweb3d::IGW3DCameraStereoSettings::TrackingOrigin origin ) override;
	virtual Geoweb3d::GW3DResult reset_SeatedZeroPose() override;


private:
	void ProcessEvent_( const vr::VREvent_t & event, Geoweb3d::IGW3DStereoDeviceNotificationCallbacks* callbacks );
	void UpdateHMDMatrixPose_();

private:
	bool ovr_initialized_ = false;
	bool ovr_is_detected_ = false;

	vr::IVRSystem* vr_context_ = nullptr;

	std::string driver_name_;
	std::string driver_serial_;
	std::string tracked_device_type_[vr::k_unMaxTrackedDeviceCount];
	vr::TrackedDevicePose_t tracked_device_pose_[vr::k_unMaxTrackedDeviceCount];
	Matrix4x4 tracked_device_matrix_[ vr::k_unMaxTrackedDeviceCount ]; // matrices for all tracked devices

	Matrix4x4 m_mat4HMDPose_; // HMD model view (Head)
	Matrix4x4 m_mat4eyePosLeft_; // offset for left eye
	Matrix4x4 m_mat4eyePosRight_; // offset for right eye

	Matrix4x4 m_mat4ProjectionLeft_; // rendering projection matrix for left eye
	Matrix4x4 m_mat4ProjectionRight_; // rendering projection matrix for right eye 

	Matrix4x4 m_mat4CullingProjectionLeft_; // culling projection matrix for left eye
	Matrix4x4 m_mat4CullingProjectionRight_; // culling projection matrix for right eye 

};

_GW3D_SMARTPTR_TYPEDEF(OpenVRStereoDevice);
