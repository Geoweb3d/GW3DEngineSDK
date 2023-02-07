//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../core/GW3DInterFace.h"
#include "IGW3DViewport.h"
#include "IGW3DCameraStereoSettings.h"
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum MatrixAccessOrder
	{
		MAO_ROW_MAJOR,
		MAO_COL_MAJOR
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DStereoDeviceNotificationCallbacks
	{
		virtual ~IGW3DStereoDeviceNotificationCallbacks(){}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnHmdConnected() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnHmdDisconnected() = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DStereoDeviceProjectionMatrixHelper
	{
		virtual ~IGW3DStereoDeviceProjectionMatrixHelper() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_NearPlane() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_FarPlane() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual double get_CullingFarPlane() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="view_index"> TODO </param>
		/// <param name="matrix_data"> TODO </param>
		/// <param name="culling_matrix_data"> TODO </param>
		/// <param name="access_order"> TODO </param>
		/// <param name="invert"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool put_ProjectionMatrix(int view_index, double* matrix_data, double* culling_matrix_data, MatrixAccessOrder access_order, bool invert = false) = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DStereoDeviceModelViewMatrixHelper
	{
		virtual ~IGW3DStereoDeviceModelViewMatrixHelper() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="access_order"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const double* get_CurrentCameraMatrix(MatrixAccessOrder access_order) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="x"> TODO </param>
		/// <param name="y"> TODO </param>
		/// <param name="z"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_DeviceRightVector(double x, double y, double z) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="x"> TODO </param>
		/// <param name="y"> TODO </param>
		/// <param name="z"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_DeviceViewVector(double x, double y, double z) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="x"> TODO </param>
		/// <param name="y"> TODO </param>
		/// <param name="z"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_DeviceUpVector(double x, double y, double z) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="matrix_data"> TODO </param>
		/// <param name="access_order"> TODO </param>
		/// <param name="invert"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_DeviceRootTransform(double* matrix_data, MatrixAccessOrder access_order, bool invert = false) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="view_index"> TODO </param>
		/// <param name="matrix_data"> TODO </param>
		/// <param name="access_order"> TODO </param>
		/// <param name="invert"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool put_PerViewTransform(int view_index, double* matrix_data, MatrixAccessOrder access_order, bool invert = false) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  GW3D_DLL IGW3DRenderTargetConfigurationHelper
	{
		virtual ~IGW3DRenderTargetConfigurationHelper() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="override"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_OverrideSurfaceSize(bool override) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="width"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Width(int width) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="height"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Height(int height) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="samples"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Samples(int samples) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DRenderTargetAccessor
	{
		virtual ~IGW3DRenderTargetAccessor() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DViewport* get_WindowViewport() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="view_index"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DViewport* get_Viewport(int view_index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="view_index"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned int get_TextureId(int view_index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="view_index"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned int get_TextureWidth(int view_index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="view_index"> TODO </param>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned int get_TextureHeight(int view_index) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="view_index"> TODO </param>
		/// <param name="x"> TODO </param>
		/// <param name="y"> TODO </param>
		/// <param name="w"> TODO </param>
		/// <param name="h"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void CompositeToWindow(int view_index, int x, int y, int w, int h) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> TODO </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DStereoDeviceCallbacks
	{
		virtual ~IGW3DStereoDeviceCallbacks() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_IsInitialized() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="configuration_helper"> TODO </param>
		/// <param name="projection_helper"> TODO </param>
		///
		/// <returns> true if success, false otherwise	</returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool Initialize(IGW3DRenderTargetConfigurationHelper* configuration_helper, IGW3DStereoDeviceProjectionMatrixHelper* projection_helper) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="projection_helper"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void UpdateProjection(IGW3DStereoDeviceProjectionMatrixHelper* projection_helper) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="modelview_helper"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void UpdateModelview(IGW3DStereoDeviceModelViewMatrixHelper* modelview_helper) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="callbacks"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void ProcessEvents(IGW3DStereoDeviceNotificationCallbacks* callbacks) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDrawBegin() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <param name="render_target_accessor"> TODO </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDrawEnd(IGW3DRenderTargetAccessor* render_target_accessor) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void OnDrawComplete() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the Tracking Origin for a HMD. </summary>
		///
		/// <remarks>	IF active stereo driver does not support this api then GW3D_eNotimpl is returned. </remarks>
		///
		/// <param name="origin"> TODO </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_TrackingOrigin(IGW3DCameraStereoSettings::TrackingOrigin origin) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the zero pose for the seated tracker coordinate system to the current position 
		///             and yaw of the HMD. </summary>
		///
		/// <remarks>	This function overrides the user's previously saved seated zero pose and should only 
		///             be called as the result of a user action. Users are also able to set their seated 
		///             zero pose via the HMD Dashboard.
		///
		///             IF active stereo driver does not support this api then GW3D_eNotimpl is returned. </remarks>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult reset_SeatedZeroPose() = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Generic Stereo Device. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DStereoDevice : public IGW3DStereoDeviceCallbacks
	{
		virtual ~IGW3DStereoDevice() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the unique name identifying the device. </summary>
		///
		/// <returns>	the name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const char* get_Name() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned int get_NumberOfSupportedViews() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_RequiresHardwareSupport() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_IsSystemSupported() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> TODO </summary>
		///
		/// <returns> TODO </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_IsHardwareDetected() const = 0;
	};
}