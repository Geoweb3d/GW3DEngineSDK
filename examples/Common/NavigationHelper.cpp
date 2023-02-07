#include "stdafx.h"
#include "NavigationHelper.h"

#include "common/GW3DCommon.h"
#include "engine/GW3DVector.h"
#include "engine/GW3DWindow.h"
#include "engine/GW3DCamera.h"
#include "engine/IGW3DEventStream.h"
#include "GeoWeb3dCore/SystemExports.h"
#include "Geoweb3dCore/CoordinateExports.h"
#include "engine/IGW3DCameraThrowSettings.h"
#include <math.h>

namespace Math
{
	const float PI = 3.141592654f;
}

namespace Conversion
{
	double RadiansToDegrees( double radians )
	{
		double degrees = ( 180.0 / Math::PI ) * radians;
		return degrees;
	}

	double DegreesToRadians( double degrees )
	{
		double radians = ( Math::PI / 180.0 ) * degrees;
		return radians;
	}
}

struct Vector2
{
	double x;
	double y;

	Vector2()
		: x( 0.0 )
		, y( 0.0 )
	{}

	Vector2( double _x, double _y )
		: x( _x )
		, y( _y )
	{}

	double Magnitude() const { return std::sqrt( Dot( *this, *this ) ); }

	Vector2 operator+( const Vector2& B ) const
	{
		return Vector2( x + B.x, y + B.y );
	}

	Vector2 operator-( const Vector2& B )
	{
		return Vector2( x - B.x, y - B.y );
	}

	static double Dot( const Vector2& A, const Vector2& B )
	{
		return A.x * B.x + A.y * B.y;
	}

	static double Cross( const Vector2& A, const Vector2& B )
	{
		return A.x * B.y - A.y * B.x;
	}

	// the angle (in degrees) between 2 vectors
	static double Angle( const Vector2& A, const Vector2& B )
	{
		double angle_radians = std::atan2( Cross( A, B ), Dot( A, B ) ); // Radians: [-PI,PI]
		return Conversion::RadiansToDegrees( angle_radians ); // Degrees: [-180, 180]
	}
};

/* Used to record a central pixel */

Geoweb3d::IGW3DCameraWPtr NavigationHelper::get_SelectedCamera()
{
	return selected_camera_;
}

int NavigationHelper::get_MouseX()
{
	return last_mouse_x_;
}

int NavigationHelper::get_MouseY()
{
	return last_mouse_y_;
}

bool NavigationHelper::get_MouseMove()
{
	return mouse_moving_;
}

bool NavigationHelper::get_LeftMouseDown()
{
	return left_mouse_button_down_;
}

bool NavigationHelper::get_RightMouseDown()
{
	return right_mouse_button_down_;
}

/**
*add_Camera adds a camera and its stats to a map that will be used to save as a home point later
*/
void NavigationHelper::add_Camera(Geoweb3d::IGW3DCameraWPtr &camPtr)
{
	selected_camera_ = camPtr;
	put_HomePosition(camPtr);
	selected_camera_.lock()->get_CameraController()->get_CameraThrowSettings()->put_ThrowingEnabled( true );

	NavigationHelper::cameraTarget camTarget = {false, 0, 0, 0, 0, 0, 0, true};
	camera_targets_[camPtr] =  camTarget;

	/*constructing central pixel queries*/
	Geoweb3d::IGW3DWindowCoordinateToGeodeticQueryWPtr pixel_query = selected_camera_.lock()->get_WindowCoordinateToGeodeticQueryCollection()->create();
	camera_central_pixel_queries_[selected_camera_] = pixel_query;

	int cam_window_x, cam_window_y;
	unsigned cam_window_width, cam_window_height;
	selected_camera_.lock()->get_Window().lock()->get_CameraWindowCoordinates(selected_camera_, cam_window_x, cam_window_y, cam_window_width, cam_window_height);

	pixel_query.lock()->put_WindowCoordinate(cam_window_x + cam_window_width / 2, cam_window_y + cam_window_height / 2);
	pixel_query.lock()->put_Enabled(true);

	/*constructing mouse pixel queries*/
	pixel_query = selected_camera_.lock()->get_WindowCoordinateToGeodeticQueryCollection()->create();
	camera_mouse_pixel_queries_[selected_camera_] = pixel_query;

	selected_camera_.lock()->get_Window().lock()->get_CameraWindowCoordinates(selected_camera_, cam_window_x, cam_window_y, cam_window_width, cam_window_height);

	pixel_query.lock()->put_WindowCoordinate(cam_window_x + cam_window_width / 2, cam_window_y + cam_window_height / 2);
	pixel_query.lock()->put_Enabled(true);


	target = &camera_targets_[selected_camera_];


}

void NavigationHelper::put_HomePosition(Geoweb3d::IGW3DCameraWPtr &camPtr)
{
	float heading, pitch, roll;
	double lon, lat, elev;
	Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
	selected_camera_.lock()->get_CameraController()->put_EnableOverride( true );
	selected_camera_.lock()->get_CameraController()->get_Rotation(heading,pitch,roll);
	selected_camera_.lock()->get_CameraController()->get_Location(lon, lat);
	selected_camera_.lock()->get_CameraController()->get_Elevation(elev, mode);
	put_HomePosition(camPtr, heading, pitch, roll, lon, lat, elev, mode);
	printf("New camera home: heading: %f, pitch: %f, roll: %f, lon: %f, lat: %f, elev: %f\n", heading, pitch, roll, lon, lat, elev );
}

void NavigationHelper::put_HomePosition(Geoweb3d::IGW3DCameraWPtr &camPtr, float heading, float pitch, float roll, double lon, double lat, double elev, Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode)
{
	NavigationHelper::cameraHome homeState = {heading, pitch, roll, lon, lat, elev, mode};
	camera_homes_[camPtr] =  homeState;
}

void NavigationHelper::reset_ToHomePosition (Geoweb3d::IGW3DCameraWPtr &camPtr)
{
	CameraHomes::iterator it;
	it = camera_homes_.find(selected_camera_);
	if (it != camera_homes_.end())
	{
		selected_camera_.lock()->get_CameraController()->put_Location(it->second.lon, it->second.lat);
		selected_camera_.lock()->get_CameraController()->put_Elevation(it->second.elev);
		selected_camera_.lock()->get_CameraController()->put_Rotation(it->second.heading, it->second.pitch, it->second.roll);
		printf("Reset to camera home: heading: %f, pitch: %f, roll: %f, lon: %f, lat: %f, elev: %f\n", it->second.heading, it->second.pitch, it->second.roll, it->second.lon, it->second.lat, it->second.elev );
	}
	else
	{
		//printf("Bug detected - selected camera has no home position!\n");
	}
}

void NavigationHelper::print_SelectedCameraValues()
{
	float heading, pitch, roll;
	double lon, lat, elev;
	Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
	selected_camera_.lock()->get_CameraController()->get_Rotation(heading, pitch, roll);
	selected_camera_.lock()->get_CameraController()->get_Location(lon, lat);
	selected_camera_.lock()->get_CameraController()->get_Elevation(elev, mode);

	printf("Cam values: heading: %f, pitch: %f, roll: %f, lon: %f, lat: %f, elev: %f\n", heading, pitch, roll, lon, lat, elev);
}

bool NavigationHelper::AssignSelectedCamera_(int x, int y, Geoweb3d::IGW3DWindowWPtr &window)
{
	Geoweb3d::IGW3DCameraWPtr hit_camera = window.unsafe_get()->get_TopCamera(x, y);	

	if(selected_camera_ != hit_camera)
	{
		//reset
		left_mouse_button_down_ = false;
		right_mouse_button_down_ = false;
		mouse_moving_ = false;
	} 
	
	selected_camera_ = hit_camera;

	if (selected_camera_.expired())
	{
		return false;
	}

	target = &camera_targets_[selected_camera_];
		
	return true;
}

bool NavigationHelper::setCentralPixelLocation()
{
	CameraCentralPixelQueries::iterator it;
	it = camera_central_pixel_queries_.find(selected_camera_);
	if (it != camera_central_pixel_queries_.end())
	{
		if (!it->second.expired())
		{		
			it->second.lock()->put_Enabled(true);
			int window_x, window_y;
			unsigned window_width, window_height;
			selected_camera_.lock()->get_Window().lock()->get_CameraWindowCoordinates(selected_camera_, window_x, window_y, window_width, window_height);

			it->second.lock()->put_WindowCoordinate( window_x + window_width/2,  window_y + window_height/2);
		}	
	}
	else
	{
		//printf("Bug detected - selected camera has no query structure!\n");

	}
	return false;
}

bool NavigationHelper::setMousePixelLocation(int x, int y)
{
	CameraMousePixelQueries::iterator it;
	it = camera_mouse_pixel_queries_.find(selected_camera_);

	if (it != camera_mouse_pixel_queries_.end())
	{
		if (!it->second.expired())
		{		
			it->second.lock()->put_Enabled(true);
			it->second.lock()->put_WindowCoordinate(x, y);
		}	
	}
	else
	{
		//printf("Bug detected - selected camera has no query structure!\n");

	}
	return false;
}

bool NavigationHelper::getCentralPixelLocation(const Geoweb3d::GW3DPoint **pt)
{

	CameraCentralPixelQueries::iterator it;
	it = camera_central_pixel_queries_.find(selected_camera_);
	if (it != camera_central_pixel_queries_.end())
	{
		if (!it->second.expired())
		{		
			if (it->second.lock()->get_IsValid())
			{
				*pt = it->second.lock()->get_GeodeticResult();
				return true;
			}
		}	
	}
	else
	{
		//printf("Bug detected - selected camera has no query structure!\n");

	}
	printf("Could not obtain center pixel location\n");
	return false;
}

bool NavigationHelper::getMousePixelLocation(const Geoweb3d::GW3DPoint **pt)
{
	CameraMousePixelQueries::iterator it;
	it = camera_mouse_pixel_queries_.find(selected_camera_);

	if (it != camera_mouse_pixel_queries_.end())
	{
		if (!it->second.expired())
		{		
			if (it->second.lock()->get_IsValid())
			{
				*pt = it->second.lock()->get_GeodeticResult();
				return true;
			}
		}	
	}
	else
	{
		//printf("Bug detected - selected camera has no query structure!\n");

	}
	printf("Could not obtain pixel location\n");
	return false;
}

#undef PRINT_ORBIT_STATUS

bool NavigationHelper::lockOrbitTarget(bool lock)
{
	if (target->lock_loc != lock || lock && target->lock_dirty)
	{
		if (lock)
		{
			Geoweb3d::IGW3DCameraController* controller = selected_camera_.lock()->get_CameraController();
			const Geoweb3d::GW3DPoint* center_pt = 0;
			if (getCentralPixelLocation(&center_pt))
			{
				//Using this central_pixel query to give me an intersection with the globe/terrain 
				controller->get_OrbitAngles(target->lock_azimuth_angle, target->lock_altitude_angle, target->lock_range, center_pt);

#ifdef PRINT_ORBIT_STATUS
				printf("Present orbit angles - azimuth ang: %f, altitude ang: %f, range: %f\n", target->lock_azimuth_angle, target->lock_altitude_angle, target->lock_range);
#endif
				target->lock_lon = center_pt->get_X();
				target->lock_lat = center_pt->get_Y();
				target->lock_elev = center_pt->get_Z();
			}
			else
			{
				double lon, lat;
				controller->get_ClosestHorizonLocation(lon, lat);
				Geoweb3d::GW3DPoint orbit_pt(lon, lat, 0);
				controller->get_OrbitAngles(target->lock_azimuth_angle, target->lock_altitude_angle, target->lock_range, &orbit_pt);

#ifdef PRINT_ORBIT_STATUS
				printf("Closest Present orbit angles - azimuth ang: %f, altitude ang: %f, range: %f\n", target->lock_azimuth_angle, target->lock_altitude_angle, target->lock_range);
#endif
				target->lock_lon = lon;
				target->lock_lat = lat;
				target->lock_elev = 0;
			}

			target->lock_dirty = false;
		}

		target->lock_loc = lock;

#ifdef PRINT_ORBIT_STATUS
		if (lock)
			printf("Locked central pixel location - lon: %f, lat: %f, elev: %f\n", target->lock_lon, target->lock_lat, target->lock_elev);
		else
			printf("Unlocked central pixel location\n");
#endif

		return true;
	}
	else
	{
		return true;
	}
}

/**
*ProcessEvent takes in a window event and a camera pointer in order to do a specified action with the mouse or keyboard
**/
Geoweb3d::IGW3DCameraWPtr NavigationHelper::ProcessEvent(const Geoweb3d::WindowEvent& win_event, Geoweb3d::IGW3DWindowWPtr &window)
{
	Geoweb3d::WindowEvent::KeyEvent special_keys;
	special_keys.code = Geoweb3d::Key::Code( 0 );
	special_keys.alt = HIWORD( GetAsyncKeyState( VK_MENU ) ) != 0;
	special_keys.control = HIWORD( GetAsyncKeyState( VK_CONTROL ) ) != 0;
	special_keys.shift = HIWORD( GetAsyncKeyState( VK_SHIFT ) ) != 0;

	int x = 0;
	int y = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	window.unsafe_get()->get_WindowSize( x, y, width, height );

	int window_center_x = x + static_cast<int>( std::floor( width * 0.5f ) );
	int window_center_y = y + static_cast<int>( std::floor( height * 0.5f ) );

	switch( win_event.Type)
	{
		case win_event.MouseButtonPressed:
		{
			if (!AssignSelectedCamera_(win_event.MouseButton.x, win_event.MouseButton.y, window))
				return selected_camera_;

			//check the button that was pressed
			switch( win_event.MouseButton.button )
			{
				case Geoweb3d::Mouse::Left:
				{								
					//printf("Left mouse button down.\n");
					left_mouse_button_down_ = true;
					right_mouse_button_down_ = false;
					mouse_moving_ = false;
					selected_camera_.lock()->get_CameraController()->pan_Begin( win_event.MouseButton.x, win_event.MouseButton.y );
				}
				break;
				case Geoweb3d::Mouse::Right:
				{
					//printf("Right mouse button down.\n");
					right_mouse_button_down_ = true;
					left_mouse_button_down_ = false;
					mouse_moving_ = false;
					mouse_rotate_ = !special_keys.shift;

					if ( !mouse_rotate_ )
					{
						start_mouse_for_gesture_x_ = win_event.MouseButton.x;
						start_mouse_for_gesture_y_ = win_event.MouseButton.y;
						last_mouse_for_gesture_x_ = start_mouse_for_gesture_x_;
						last_mouse_for_gesture_y_ = start_mouse_for_gesture_y_;
						selected_camera_.lock()->get_CameraController()->panZoomAndOrbit_Begin( window_center_x, window_center_y );
					}
				}
				break;
				default:
				break;
			};

			last_mouse_x_ = win_event.MouseButton.x;
			last_mouse_y_ = win_event.MouseButton.y;
		}
		break;
		case win_event.MouseButtonReleased: 
		{	
			if (!AssignSelectedCamera_(win_event.MouseButton.x, win_event.MouseButton.y, window))
				return selected_camera_;

			switch( win_event.MouseButton.button )
			{
				case Geoweb3d::Mouse::Left:
				{
					//printf("Left mouse button up.\n");
					if ( left_mouse_button_down_ )
					{
						selected_camera_.lock()->get_CameraController()->throw_Camera();
					}
					left_mouse_button_down_ = false;
				}
				break;
				case Geoweb3d::Mouse::Right:
				{
					//printf("Right mouse button up.\n");
					right_mouse_button_down_ = false;
				}
				break;
				default:
				break;
			};

			last_mouse_x_ = win_event.MouseButton.x;
			last_mouse_y_ = win_event.MouseButton.y;
		}
		break;
		case win_event.MouseMoved: 
		{
			if (!AssignSelectedCamera_(win_event.MouseMove.x, win_event.MouseMove.y, window))
				return selected_camera_;

			setMousePixelLocation(win_event.MouseMove.x, win_event.MouseMove.y);
			setCentralPixelLocation();

			/**
			**Moves the map according to where the mouse is.
			*/
			if( left_mouse_button_down_ )
			{
				selected_camera_.lock()->get_CameraController()->pan_ToWindowCoordinate( win_event.MouseMove.x, win_event.MouseMove.y );
				target->lock_dirty = true;
			}

			if (right_mouse_button_down_)
			{
				if ( mouse_rotate_ )
				{
					/**
					**Rotates the map in the direction of the mouse.
					*/
					selected_camera_.lock()->get_CameraController()->rotate_ByWindowCoordinateDelta( last_mouse_x_, last_mouse_y_, win_event.MouseMove.x, win_event.MouseMove.y, 0.8, 0.8 );
					target->lock_dirty = true;
				}
				else
				{
					// pan, zoom, rotate, based on the offset from window center. 
					// this simulates a 2-finger gesture where the first finger is at the widow center and the mouse 
					// position is the second finger. From which and angle and a scale can be calculated.

					// point 0 doesn't move for a mouse based test
					Vector2 pt_0_start( window_center_x, window_center_y );
					Vector2 pt_0_current( window_center_x, window_center_y );
					Vector2 pt_0_last( window_center_x, window_center_y );

					Vector2 pt_1_start( start_mouse_for_gesture_x_, start_mouse_for_gesture_y_ );
					Vector2 pt_1_current( win_event.MouseMove.x, win_event.MouseMove.y );
					Vector2 pt_1_last( last_mouse_for_gesture_x_, last_mouse_for_gesture_y_ );

					Vector2 startVector = pt_0_start - pt_1_start;
                    Vector2 currentVector = pt_0_current - pt_1_current;
                    Vector2 lastVector = pt_0_last - pt_1_last;

					// angle relative to gesture start - needed in range [0, 360]
 					double relative_angle_degrees = Vector2::Angle( currentVector, startVector );
					relative_angle_degrees =  std::fmod( relative_angle_degrees + 360.0, 360.0 ); 

					// angle relative to last position - needed in range [-180, 180]
					double incremental_angle_degreees =  Vector2::Angle( currentVector, lastVector );

					// a ratio of the distance between the two fingers at the start and now.
					double relative_scale = currentVector.Magnitude() / startVector.Magnitude();

					// a ratio of the distance between the two fingers at the last move and now.
					double incremental_scale = lastVector.Magnitude() / currentVector.Magnitude();

					//printf( "2 Finger Rotate (%f vs %f)\n", relative_angle_degrees, incremental_angle_degreees );
					//printf( "2 Finger Scale (%f vs %f)\n", relative_scale, incremental_scale );

					float angle = static_cast<float>( -incremental_angle_degreees );
					float scale = static_cast<float>( incremental_scale );

					selected_camera_.lock()->get_CameraController()->pan_ToWindowCoordinate_ZoomAndOrbit_ByAngleScale
						( static_cast<int>(pt_0_current.x), static_cast<int>(pt_0_current.y), angle, scale );

					// set for the next frame
					last_mouse_for_gesture_x_ = static_cast<int>( pt_1_current.x );
					last_mouse_for_gesture_y_ = static_cast<int>( pt_1_current.y );
				}
			}

			last_mouse_x_ = win_event.MouseMove.x;
			last_mouse_y_ = win_event.MouseMove.y;
			mouse_moving_ = true;
		}
		break;
		/**
		**Basic wheel scroll that changes elevation depending on wheel delta. Uses pitch so it zooms in depending on your angle.
		*/
		case win_event.MouseWheelMoved:
		{
			if (!AssignSelectedCamera_(win_event.MouseButton.x, win_event.MouseButton.y, window))
				return selected_camera_;

			Geoweb3d::IGW3DCameraController* controller = selected_camera_.lock()->get_CameraController();
			Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
			double elev;
			controller->get_Elevation(elev, mode);
			selected_camera_.lock()->get_CameraController()->move(elev * 0.05 * win_event.MouseWheel.delta, 0.0, true);
			target->lock_dirty = true;
		}
		break;
		/**
		*Double click zooms in 10x more. 
		*/
		case win_event.MouseButtonDBLClick:
		{	
			if (!AssignSelectedCamera_(win_event.MouseButton.x, win_event.MouseButton.y, window))
				return selected_camera_;

			const Geoweb3d::GW3DPoint *mouse_pt = 0;
			if(getMousePixelLocation(&mouse_pt ))
			{
				//const Geoweb3d::GW3DPoint* mouse_pt = mouse_pixel_query.lock()->get_GeodeticResult();
				const Geoweb3d::GW3DPoint* camera_loc = selected_camera_.lock()->get_CameraController()->get_Location();
					
				Geoweb3d::IGW3DCameraController* controller = selected_camera_.lock()->get_CameraController();
				Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
				double elev;
				controller->get_Elevation(elev, mode);
				if(elev > 5000)
				{
					selected_camera_.lock()->get_CameraController()->move_Toward(camera_loc->get_Z() * 0.66, mouse_pt);
				}
				else
				{
					selected_camera_.lock()->get_CameraController()->move_Toward(camera_loc->get_Z() * 0.5, mouse_pt);
				}
				target->lock_dirty = true;
			}
		}
		break;
		/**
		*Keyboard events. 
		*/
		case win_event.KeyPressed:
		{	
			if (selected_camera_.expired() || !target)
				break;

			//printf("PRE ");
			//print_SelectedCameraValues();

			float cHeading, cPitch, cRoll;
			switch(	win_event.Key.code )
			{
			case Geoweb3d::Key::Space:
			{
				if (win_event.Key.shift)
				{				
					put_HomePosition(selected_camera_);
				}
				else
				{							
					reset_ToHomePosition(selected_camera_);
				}
				target->lock_dirty = true;
				break;
			}
			//Move in corresponding direction
			case Geoweb3d::Key::Up:
				{
					target->lock_dirty = true;
					selected_camera_.lock()->get_CameraController()->move(10.0,0.0, false);
					break;
				}
			case Geoweb3d::Key::Down:
				{
					target->lock_dirty = true;
					selected_camera_.lock()->get_CameraController()->move(10.0,180.0, false);
					break;
				}
			case Geoweb3d::Key::Right:
				{
					target->lock_dirty = true;
					//printf("Right pressed.\n");
					selected_camera_.lock()->get_CameraController()->move(10.0,90.0, false);
					break;
				}
			case Geoweb3d::Key::Left:
				{
					target->lock_dirty = true;
					//printf("Left pressed.\n");
					selected_camera_.lock()->get_CameraController()->move(10.0,270.0, false);
					break;
				}
			//R will go to the default starting location.
			case Geoweb3d::Key::R:
				{
					target->lock_dirty = true;
					reset_ToHomePosition(selected_camera_);
					break;
				}
			//Heading
			case Geoweb3d::Key::D:
				{
					target->lock_dirty = true;
					cHeading = selected_camera_.lock()->get_CameraController()->get_Heading( );
					selected_camera_.lock()->get_CameraController()->put_Heading(++cHeading);
					break;
				}
			case Geoweb3d::Key::A:
				{
					target->lock_dirty = true;
					cHeading = selected_camera_.lock()->get_CameraController()->get_Heading( );
					selected_camera_.lock()->get_CameraController()->put_Heading(--cHeading);
					break;
				}
			//Pitch
			case Geoweb3d::Key::W:
				{
					target->lock_dirty = true;
					cPitch = selected_camera_.lock()->get_CameraController()->get_Pitch( );
					selected_camera_.lock()->get_CameraController()->put_Pitch(++cPitch);
					break;
				}
			case Geoweb3d::Key::S:
				{
					target->lock_dirty = true;
					cPitch = selected_camera_.lock()->get_CameraController()->get_Pitch( );
					selected_camera_.lock()->get_CameraController()->put_Pitch(--cPitch);
					break;
				}
			//Roll
			case Geoweb3d::Key::Z:
				{
					target->lock_dirty = true;
					cRoll = selected_camera_.lock()->get_CameraController()->get_Roll( );
					selected_camera_.lock()->get_CameraController()->put_Roll(++cRoll);
					break;						
				}
			case Geoweb3d::Key::X:
				{
					target->lock_dirty = true;
					cRoll = selected_camera_.lock()->get_CameraController()->get_Roll( );
					selected_camera_.lock()->get_CameraController()->put_Roll(--cRoll);
					break;
				}
			/* Orbit Related Navigation -------------------------------------------------------------------------*/
			case Geoweb3d::Key::L: 
			//Record central pixel - it will be used in consecutive orbiting calls (using a new central pixel directly for subsequent orbits will cause drift due to limited central pixel precision)
				{
					lockOrbitTarget(!target->lock_loc);			
					break;
				}
			case Geoweb3d::Key::U: 		
				{
					if(lockOrbitTarget(true))
					{
						Geoweb3d::IGW3DCameraController* controller = selected_camera_.lock()->get_CameraController();

						if (win_event.Key.shift)
							target->lock_altitude_angle -= 1;
						else
							target->lock_altitude_angle += 1;

						Geoweb3d::GW3DPoint target_pt(target->lock_lon, target->lock_lat, target->lock_elev);
						controller->orbit(target->lock_azimuth_angle, target->lock_altitude_angle, target->lock_range, &target_pt);
					}
					break;
				}
			case Geoweb3d::Key::I:
			//Orbit based on keeping target centered, while keeping lon lat constant and only varying elevation
				{
					if (lockOrbitTarget(true))
					{
						Geoweb3d::IGW3DCameraController* controller = selected_camera_.lock()->get_CameraController();
						Geoweb3d::IGW3DPositionOrientation::AltitudeMode mode;
						double elev;
						controller->get_Elevation(elev, mode);
						//Changing elevation by 5 percent
						double elev_diff = elev * 0.05;

						if (win_event.Key.shift)
							elev_diff = -elev_diff;

						Geoweb3d::GW3DPoint target_pt(target->lock_lon, target->lock_lat, target->lock_elev);
						controller->orbit_ByElevation (elev_diff, &target_pt);

						controller->get_OrbitAngles(target->lock_azimuth_angle, target->lock_altitude_angle, target->lock_range, &target_pt);
					}
					break;
				}
			case Geoweb3d::Key::O: 
			//Orbit based on angles relative from target - consider locking central pixel before calling this
				{

					if (lockOrbitTarget(true))
					{
						Geoweb3d::IGW3DCameraController* controller = selected_camera_.lock()->get_CameraController();
						//Changing azimuth by degrees
						double azimuth_diff = 1;

						if (win_event.Key.shift)
							azimuth_diff = -azimuth_diff;

						Geoweb3d::GW3DPoint target_pt(target->lock_lon, target->lock_lat, target->lock_elev);
						controller->orbit_ByAzimuth (azimuth_diff, &target_pt);

						controller->get_OrbitAngles(target->lock_azimuth_angle, target->lock_altitude_angle, target->lock_range, &target_pt);
					}
					break;
				}
			break;

			default:
			break;
			}

			//printf("POST ");
			//print_SelectedCameraValues();
		}
		break;
		default:	
		break;
    };

	return selected_camera_;
}

