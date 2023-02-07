#include "stdafx.h"

#include "FlightPath.h"

#include <iomanip>

FlightPath::FlightPath()
{
	_Spacing = 0.0;
}

FlightPath::FlightPath(const IGW3DVectorDataSourceWPtr & dataSource)
{
	_Spacing = 0.0;

	SetDataSource(dataSource);
}

FlightPath::~FlightPath()
{
}

void FlightPath::SetSpacing(const double spacing)
{
	_Spacing = spacing;
}

double FlightPath::GetMaxElevation()
{
	return _MaxElevation;
}

const IGW3DVectorDataSourceWPtr & FlightPath::GetDataSource()
{
	return _PathDataSource;
}

bool FlightPath::SetDataSource(const IGW3DVectorDataSourceWPtr & dataSource)
{
	_PathDataSource = dataSource;
	return true;
}

unsigned long FlightPath::count() const
{
	return 0;
}

bool FlightPath::next(long * ppVal)
{
	return false;
}

void FlightPath::reset()
{
}

long FlightPath::operator[](unsigned long index)
{
	return 0;
}

long FlightPath::get_AtIndex(unsigned long index)
{
	return 0;
}

bool FlightPath::OnStream(IGW3DVectorLayerStreamResult * result)
{
	// Create a new layer "copy" based on the original layer
	GW3DResult retVal = GW3DResult::GW3D_sFalse;

	// Clone the existing attributes, though for this particular example I don't think we want to do that...
	auto definitionCollection = result->get_AttributeCollection()->create_Clone();
	
	// Get the actual geometry for the data set we're streaming so we can do something with it
	auto geometry = result->get_Geometry();
	
	// Create our new layers
	// Create a new layer for the path itself
	auto pathFieldDefinitions = IGW3DDefinitionCollection::create();
	auto pathLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Path", Geoweb3d::gtLINE_STRING_25D, pathFieldDefinitions);
	auto pathAttributes = pathLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	pathLayer.lock()->put_GeometryEditableMode(true);

	// Create a new layer for the path spires
	auto spireFieldDefinitions = IGW3DDefinitionCollection::create();
	auto spireLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("PathSpires", Geoweb3d::gtPOINT_25D, spireFieldDefinitions);
	auto spireAttributes = spireLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	// spireLayer.lock()->put_GeometryEditableMode(true);

	// Create a new layer for the camera point
	auto cameraFieldDefinitions = IGW3DDefinitionCollection::create();
	auto cameraLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Camera", Geoweb3d::gtPOINT_25D, cameraFieldDefinitions);
	auto cameraAttributes = cameraLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	cameraLayer.lock()->put_GeometryEditableMode(true);

	// Create a new layer for the model location
	auto modelFieldDefinitions = IGW3DDefinitionCollection::create();
	auto modelLocationLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Model", Geoweb3d::gtPOINT_25D, modelFieldDefinitions);
	auto modelAttributes = modelLocationLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	modelLocationLayer.lock()->put_GeometryEditableMode(true);

	// Create a new layer for the flight path ribbon
	auto ribbonFieldDefinitions = IGW3DDefinitionCollection::create();
	auto ribbonLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Ribbon", Geoweb3d::gtPOLYGON_25D, ribbonFieldDefinitions);
	auto ribbonAttributes = ribbonLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	// ribbonLayer.lock()->put_GeometryEditableMode(true);

	// Make a copy of the path for us to use
	pathLayer.lock()->create_Feature(pathAttributes, geometry, retVal);

	IGW3DBearingHelperPtr bearingToNextPoint = IGW3DBearingHelper::create();
	double bearing = 0.0;
	double pitch = 0.0;
	double distance = 0.0;
	GW3DPoint lastPoint;
	GW3DPoint currentPoint;
	GW3DPoint nextPoint;
	GW3DPoint vertex;

	// Create the post guides for the path
	{
		auto lineString = dynamic_cast<const GW3DLineString*>(geometry);

		// First point is our starting point
		lineString->get_StartPoint(&currentPoint);
		lineString->get_Point(1, &nextPoint);
		lastPoint = currentPoint;

		spireLayer.lock()->create_Feature(spireAttributes, &currentPoint, retVal);

		// It's also the starting point for the camera
		cameraLayer.lock()->create_Feature(cameraAttributes, &currentPoint, retVal);

		// It's also the starting location for the visualization model
		modelLocationLayer.lock()->create_Feature(modelAttributes, &currentPoint, retVal);

		//// Start generating the ribbon polys
		//{
		//	GW3DPolygon poly;
		//	GW3DLinearRing segment;

		//	// POC
		//	bearingToNextPoint->put_SourceLocation(currentPoint.get_X(), currentPoint.get_Y(), currentPoint.get_Z());
		//	bearingToNextPoint->put_TargetLocation(nextPoint.get_X(), nextPoint.get_Y(), nextPoint.get_Z());
		//	bearingToNextPoint->calculate_result(bearing, pitch, distance);

		//	//if (bearing + 90.0 >= 360.0)
		//	//	bearing -= 360.0;

		//	vertex = currentPoint;
		//	segment.add_Point(&vertex);
		//	//std::cout << std::setprecision(10) << "Poly : (" << currentPoint.get_X() << "," << currentPoint.get_Y() << "," << currentPoint.get_Z() << ")";

		//	vertex = nextPoint;
		//	segment.add_Point(&vertex);
		//	//std::cout << "(" << nextPoint.get_X() << "," << nextPoint.get_Y() << "," << nextPoint.get_Z() << ")";

		//	vertex = CalcPointFromDistanceBearing(nextPoint, 100.0, bearing);
		//	vertex.put_Z(currentPoint.get_Z());
		//	segment.add_Point(&vertex);
		//	//std::cout << "(" << vertex.get_X() << "," << vertex.get_Y() << "," << vertex.get_Z() << ")";

		//	vertex = CalcPointFromDistanceBearing(currentPoint, 100.0, bearing);
		//	vertex.put_Z(currentPoint.get_Z());
		//	segment.add_Point(&vertex);
		//	//std::cout << "(" << vertex.get_X() << "," << vertex.get_Y() << "," << vertex.get_Z() << ")" << std::endl;
		//}

		_MaxElevation = currentPoint.get_Z();
		pointsAdded = 1;

		GW3DPolygon poly;
		GW3DLinearRing segment;

		for (int i = 0; i < lineString->get_NumPoints(); i++) {
			lastPoint = currentPoint;
			lineString->get_Point(i, &currentPoint);

			// Determine our maximal post elevation
			if (currentPoint.get_Z() > _MaxElevation)
				_MaxElevation = currentPoint.get_Z();

			// Roughly place posts every _Spacing meters apart
			// TODO : Interpolate a point to the exact spacing the current point is not exact
			currentPoint.get_DistanceInMeters(&lastPoint, false, distance);
			
			if (distance > _Spacing) {
				pointsAdded++;
				spireLayer.lock()->create_Feature(spireAttributes, &currentPoint, retVal);
				lastPoint = currentPoint;
				distance = 0.0;
			}

			// Continue with generating the path ribbon
			//if(i < lineString->get_NumPoints() - 1) {
				//GW3DPolygon poly;
				//GW3DLinearRing segment;

				lineString->get_Point(i + 1, &nextPoint);

				// POC
				bearingToNextPoint->put_SourceLocation(currentPoint.get_X(), currentPoint.get_Y(), currentPoint.get_Z());
				bearingToNextPoint->put_TargetLocation(nextPoint.get_X(), nextPoint.get_Y(), nextPoint.get_Z());
				bearingToNextPoint->calculate_result(bearing, pitch, distance);

				if (bearing + 90.0 >= 360.0)
					bearing -= 360.0;

				vertex = currentPoint;
				segment.add_Point(&vertex);
				////std::cout << std::setprecision(10) << "Poly : (" << currentPoint.get_X() << "," << currentPoint.get_Y() << "," << currentPoint.get_Z() << ")";

				//vertex = nextPoint;
				//segment.add_Point(&vertex);
				////std::cout << "(" << nextPoint.get_X() << "," << nextPoint.get_Y() << "," << nextPoint.get_Z() << ")";

				//vertex = CalcPointFromDistanceBearing(nextPoint, 100.0, bearing);
				//vertex.put_Z(currentPoint.get_Z());
				//segment.add_Point(&vertex);
				////std::cout << "(" << vertex.get_X() << "," << vertex.get_Y() << "," << vertex.get_Z() << ")";

				//vertex = CalcPointFromDistanceBearing(currentPoint, 100.0, bearing);
				//vertex.put_Z(currentPoint.get_Z());
				//segment.add_Point(&vertex);
				////std::cout << "(" << vertex.get_X() << "," << vertex.get_Y() << "," << vertex.get_Z() << ")" << std::endl;

				//segment.close_Rings();

				//poly.add_Ring(&segment);
				//poly.close_Rings();

				//ribbonLayer.lock()->create_Feature(ribbonAttributes, &poly, retVal);
			//}		

		}

		double lastBearing = 0.0;

		for (int i = lineString->get_NumPoints() - 1; i != 0; i--) {
			lineString->get_Point(i, &currentPoint);
			lineString->get_Point(i + 1, &nextPoint);

			// POC
			bearingToNextPoint->put_SourceLocation(currentPoint.get_X(), currentPoint.get_Y(), currentPoint.get_Z());
			bearingToNextPoint->put_TargetLocation(nextPoint.get_X(), nextPoint.get_Y(), nextPoint.get_Z());
			bearingToNextPoint->calculate_result(bearing, pitch, distance);

			if (bearing < 0.0)
				bearing += 360.0;

			if (bearing + 90.0 >= 360.0)
				bearing -= 360.0;

			if ((bearing > lastBearing + 10.0) || (bearing < lastBearing - 10.0)) {
				bearing = lastBearing;
			}

			vertex = CalcPointFromDistanceBearing(nextPoint, 100.0, bearing);;
			segment.add_Point(&vertex);

			lastBearing = bearing;
		}

		segment.close_Rings();

		poly.add_Ring(&segment);
		poly.close_Rings();

		ribbonLayer.lock()->create_Feature(ribbonAttributes, &poly, retVal);


		// Check to see if we need to add the last point
		lineString->get_EndPoint(&currentPoint);
		if (!currentPoint.equals(&lastPoint)) {
			spireLayer.lock()->create_Feature(spireAttributes, &currentPoint, retVal);
		}
	}

	if (Geoweb3d::Succeeded(retVal)) {
		return true;
	}

	return false;
}

bool FlightPath::OnError()
{
	return false;
}

GW3DPoint FlightPath::CalcPointFromDistanceBearing(const GW3DPoint & point, const double distance, const double bearing)
{
	double rLat = point.get_Y() * 3.14159265358979323846 / 180.0;
	double rLon = point.get_X() * 3.14159265358979323846 / 180.0;
	double rAng = bearing * 180.0 / 3.14159265358979323846;
	double r = (distance) / (6371.0 * 2.0 * 1000.0);

	double lat = asin((sin(rLat) * cos(r)) + (cos(rLat) * sin(r) * cos(rAng)));
	double lon = rLon + atan2((sin(rAng) * sin(r) * cos(rLat)), cos(r) - (sin(rLat) * sin(lat)));

	lat = lat * 180.0 / 3.14159265358979323846;
	lon = lon * 180.0 / 3.14159265358979323846;

	return GW3DPoint(lon, lat, point.get_Z());
}
