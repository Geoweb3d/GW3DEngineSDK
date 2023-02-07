#define _USE_MATH_DEFINES
#include <math.h>
#include "stdafx.h"
#include "engine/IGW3DGeoweb3dSDK.h"
#include "engine/IGW3DInitializationConfiguration.h"
#include "engine/IGW3DPositionOrientation.h"
#include "engine/IGW3DWindowCallback.h"

#include <map>
#pragma comment(lib, "GW3DEngineSDK.lib")
#pragma comment(lib, "GW3DCommon.lib")

/**NavigationHelper class acts to be used a direct copy and paste example
*Creates a window that is navigable
*/
class NavigationHelper
{
	public:
		NavigationHelper() : left_mouse_button_down_(false), 
		right_mouse_button_down_(false),
		mouse_moving_(false),
		last_mouse_x_(0),
		last_mouse_y_(0)
		{}

		void add_Camera(Geoweb3d::IGW3DCameraWPtr &_camera);

		void put_HomePosition(Geoweb3d::IGW3DCameraWPtr &camPtr, float heading, float pitch, float roll, double lon, double lat, double elev, Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode);
		void put_HomePosition(Geoweb3d::IGW3DCameraWPtr &camPtr);

		void reset_ToHomePosition (Geoweb3d::IGW3DCameraWPtr &camPtr);
		void print_SelectedCameraValues();

		bool setCentralPixelLocation();
		bool setMousePixelLocation(int x, int y);
		bool getCentralPixelLocation(const Geoweb3d::GW3DPoint **pt);
		bool getMousePixelLocation(const Geoweb3d::GW3DPoint **pt);

		bool lockOrbitTarget(bool lock);

		Geoweb3d::IGW3DCameraWPtr ProcessEvent(const Geoweb3d::WindowEvent& win_event, Geoweb3d::IGW3DWindowWPtr &p);

		Geoweb3d::IGW3DCameraWPtr get_SelectedCamera();

		int get_MouseX();
		int get_MouseY();
		bool get_MouseMove();
		bool get_LeftMouseDown();
		bool get_RightMouseDown();

	private:

		bool AssignSelectedCamera_(int x, int y, Geoweb3d::IGW3DWindowWPtr &window);

		struct cameraHome
		{
			//set defaults for home coordinates
			float heading;
			float pitch;
			float roll;
			double lon;
			double lat;
			double elev;
			Geoweb3d::IGW3DPositionOrientation::AltitudeMode hMode;
		};

		struct cameraTarget
		{
			bool lock_loc;
			double lock_azimuth_angle;
			double lock_altitude_angle;
			double lock_range;
			double lock_lon;
			double lock_lat;
			double lock_elev;
			double lock_dirty;

		};

		typedef std::map<Geoweb3d::IGW3DCameraWPtr, NavigationHelper::cameraHome> CameraHomes;
		CameraHomes camera_homes_;
		typedef std::map<Geoweb3d::IGW3DCameraWPtr, NavigationHelper::cameraTarget> CameraTargets;
		CameraTargets camera_targets_;
		typedef std::map<Geoweb3d::IGW3DCameraWPtr, Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr> CameraCentralPixelQueries;
		CameraCentralPixelQueries camera_central_pixel_queries_;
		typedef std::map<Geoweb3d::IGW3DCameraWPtr, Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr> CameraMousePixelQueries;
		CameraMousePixelQueries camera_mouse_pixel_queries_;


		Geoweb3d::IGW3DCameraWPtr selected_camera_;
		Geoweb3d::IGW3DCameraController *camera_controller_;
		bool left_mouse_button_down_; 
		bool right_mouse_button_down_;
		bool mouse_moving_;
		int last_mouse_x_;
		int last_mouse_y_;

		bool mouse_rotate_;
		int start_mouse_for_gesture_x_;
		int start_mouse_for_gesture_y_;
		int last_mouse_for_gesture_x_;
		int last_mouse_for_gesture_y_;

		cameraTarget *target;
};