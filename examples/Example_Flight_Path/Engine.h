#pragma once

using namespace Geoweb3d;

class Engine : IGW3DWindowCallback
{
	typedef enum { Follow = 0, Map } CameraMode;

public:
	Engine();
	Engine(const IGW3DGeoweb3dSDKPtr& context);

	Engine& operator=(const IGW3DGeoweb3dSDKPtr& context);

	~Engine();

	bool SetSdkContext(const IGW3DGeoweb3dSDKPtr& context);
	bool Render();
	bool ResizeWindow(int x, int y, unsigned int cx, unsigned int cy);
	bool SetCameraPosition(const IGW3DCameraWPtr& camera, double longitude, double latitude, double elevation);
	bool SetCameraPosition(const IGW3DCameraWPtr& camera, const GW3DPoint& point);
	bool SetCameraOrientation(const IGW3DCameraWPtr& camera, float roll, float pitch, float yaw);

public:
	virtual bool CreateSdkCameras();
	virtual bool CreateSdkWindow(const std::string& name);
	virtual bool LoadFlightData(const std::string& filename);
	virtual bool LoadElevationData(const std::string& filename);
	virtual bool LoadImageryData(const std::string& filename);
	virtual bool LoadModelData(const std::string& filename);
	virtual bool ExportData(const std::string& filename, const std::string& type);

private:
	virtual IGW3DCameraWPtr CreateCamera(const std::string& name);
	virtual bool SetCameraExtents();

private:
	IGW3DGeoweb3dSDKPtr _SdkContext;
	IGW3DWindowWPtr _Window;
	IGW3DCameraWPtr _MapCamera;
	IGW3DCameraWPtr _FollowCamera;
	IGW3DCameraWPtr _PrimaryCamera;
	IGW3DCameraWPtr _SecondaryCamera;
	GW3DEnvelope _Envelope;

	IGW3DFontWPtr _Font;

	FlightDataController _Data;
	ModelController _ModelController;
	double _Timer;

	bool _Pan;
	bool _Rotate;
	int _StartX;
	int _StartY;

	CameraMode _CameraMode;
	bool _FollowIsDetatched;
	bool _CockpitView;

	double _ChaseDistance;

	std::string _ModelPath;

	std::vector<FlightDataController> _FlightData;
	std::vector<IGW3DRasterRepresentationWPtr> _Imagery;
	std::vector<IGW3DRasterRepresentationWPtr> _Elevation;

private:
	// Inherited via IGW3DWindowCallback
	virtual void OnCreate() override;
	virtual void OnDrawBegin() override;
	virtual void OnDrawEnd() override;
	virtual void OnDraw2D(const IGW3DCameraWPtr & camera) override;
	virtual void OnDraw2D(const IGW3DCameraWPtr & camera, IGW3D2DDrawContext *drawcontext) override;
	virtual void ProcessEvent(const WindowEvent & event) override;

	bool CreatePIPCameraBackground(const IGW3DCameraWPtr& camera);
};

