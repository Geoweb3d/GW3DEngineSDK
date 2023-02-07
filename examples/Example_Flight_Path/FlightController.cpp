#include "stdafx.h"
#include "FlightController.h"


FlightController::FlightController()
{
}

FlightController::~FlightController()
{
}

GW3DPoint FlightController::GetPointAtTime(const double time, const IGW3DVectorLayerWPtr& path)
{
	_Time = time;
	path.lock()->Stream(this);
	return _PointOfInterest;
}

GW3DPoint FlightController::MoveModelToTimePoint(const double time, const IGW3DVectorLayerWPtr & path, const IGW3DVectorLayerWPtr & model)
{
	_Time = time;
	path.lock()->Stream(this);
	model.lock()->Stream(this);
	return _PointOfInterest;
}

unsigned long FlightController::count() const
{
	return 0;
}

bool FlightController::next(long * ppVal)
{
	return false;
}

void FlightController::reset()
{
}

long FlightController::operator[](unsigned long index)
{
	return 0;
}

long FlightController::get_AtIndex(unsigned long index)
{
	return 0;
}

bool FlightController::OnStream(IGW3DVectorLayerStreamResult * result)
{
	auto geometry = result->get_EditableGeometry();

	if (geometry->get_GeometryType() == Geoweb3d::gtLINE_STRING_25D) {

		// Figure out where in the path we are for a given time
		auto path = dynamic_cast<GW3DLineString*>(geometry);

		auto nPoints = path->get_NumPoints();

		// If we get a fractional point we have to interpolate
		double currentTimePoint = _Time / 1.0 * (double)nPoints;

		// But for the sake of a demo we'll just approximate for now
		path->get_Point((int)currentTimePoint, &_PointOfInterest);

		GW3DPoint nextPoint;
		path->get_Point((int)currentTimePoint + 1, &nextPoint);

		IGW3DBearingHelperPtr bearingToNextPoint = IGW3DBearingHelper::create();
		bearingToNextPoint->put_SourceLocation(_PointOfInterest.get_X(), _PointOfInterest.get_Y(), _PointOfInterest.get_Z());
		bearingToNextPoint->put_TargetLocation(nextPoint.get_X(), nextPoint.get_Y(), nextPoint.get_Z());
		bearingToNextPoint->calculate_result(_Bearing, _Pitch, _Distance);

		return true;
	}

	if (geometry->get_GeometryType() == Geoweb3d::gtPOINT_25D) {
		auto location = dynamic_cast<GW3DPoint*>(geometry);
		*location = _PointOfInterest;
	}

	return false;
}

bool FlightController::OnError()
{
	return false;
}
