#include "stdafx.h"

#include "FlightDataController.h"

#include <iomanip>

FlightDataController::FlightDataController()
{
	_PostSpacing = 0.0;
	_Posts = 0;
	_PathWidth = 0.0;
	_SectionLength = 5.0;
	_InitialHeading = 0.0;
}

FlightDataController::FlightDataController(const IGW3DVectorDataSourceWPtr & dataSource)
{
	_PostSpacing = 0.0;
	_Posts = 0;
	_PathWidth = 0.0;
	_SectionLength = 5.0;
	_InitialHeading = 0.0;

	SetDataSource(dataSource);
}

FlightDataController::~FlightDataController()
{
}

void FlightDataController::SetPostSpacing(const double spacing)
{
	_PostSpacing = spacing;
}

void FlightDataController::SetPathWidth(const double width)
{
	_PathWidth = abs(width);
}

void FlightDataController::SetSectionLength(const double length)
{
	_SectionLength = length;
}

const double FlightDataController::GetMaxElevation() const
{
	return _MaxElevation;
}

const IGW3DVectorDataSourceWPtr & FlightDataController::GetDataSource() const
{
	return _PathDataSource;
}

const bool FlightDataController::SetDataSource(const IGW3DVectorDataSourceWPtr & dataSource)
{
	_PathDataSource = dataSource;
	return true;
}

const GW3DPoint FlightDataController::GetInitialLocation() const
{
	return _StartingPoint;
}

const double FlightDataController::GetInitialHeading() const
{
	return _InitialHeading;
}

const int FlightDataController::GetPostCount() const
{
	return _Posts;
}

const int FlightDataController::GetPathLength() const
{
	return _PathLength;
}

const VertexInformation FlightDataController::GetVertex(const size_t index)
{
	if (index >= _Path.size())
		return VertexInformation();

	return _Path.at(index);
}

unsigned long FlightDataController::count() const
{
	return 0;
}

bool FlightDataController::next(int64_t* ppVal)
{
	return false;
}

void FlightDataController::reset()
{
}

int64_t FlightDataController::operator[](unsigned long index)
{
	return 0;
}

int64_t FlightDataController::get_AtIndex(unsigned long index)
{
	return 0;
}

bool FlightDataController::OnStream(IGW3DVectorLayerStreamResult * result)
{
	int m = 0;
	double bearing = 0.0;
	double pitch = 0.0;
	double distance = 0.0;
	double lastBearing = 0.0;
	GW3DPoint lastPoint;
	GW3DPoint currentPoint;
	GW3DPoint nextPoint;
	GW3DPoint vertex;
	GW3DPoint lastPost;
	GW3DPoint lastRibbon;

	GW3DPolygon poly;
	GW3DMultiPolygon multi_poly;
	GW3DLinearRing section;

	GW3DResult retVal = GW3DResult::GW3D_sFalse;

	// Create a bearing helper that we're going to reuse
	IGW3DBearingHelperPtr bearingToNextPoint = IGW3DBearingHelper::create();

	// There was no datasource created to put all this stuff into.
	if (_PathDataSource.expired()) {
		std::cout << "No data source has been set." << std::endl;
		return false;
	}

	// Clone the existing attributes, though for this particular example I don't think we want to do that...
	auto definitionCollection = result->get_AttributeCollection()->create_Clone();

	// Get the actual geometry for the data set we're streaming so we can do something with it
	auto geometry = result->get_Geometry();

	// Make sure these are the droids we're looking for...
	if (geometry == nullptr || geometry->get_GeometryType() != gtLINE_STRING_25D)
		return false;

	// Cast our geometry to the proper type so we can access the right functions
	const GW3DLineString* lineString = dynamic_cast<const GW3DLineString*>(geometry);

	// Create our new layers

	// We don't need to but for the sake of making data slightly more uniform create a copy of the path
	// so that it's in the same datasource as all of the other layers
	if (_PathLayer.expired()) {
		_PathDefinitions = IGW3DDefinitionCollection::create();
		_PathLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Path", Geoweb3d::gtLINE_STRING_25D, _PathDefinitions);
	}

	auto pathAttributes = _PathLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	auto pathProperties = _PathDefinitions->create_PropertyCollection();

	// Flight path posts layer
	if (_PostLayer.expired()) {
		_PostDefinitions = IGW3DDefinitionCollection::create();
		_PostDefinitions->add_Property("Spacing", Geoweb3d::PROPERTY_DOUBLE, "Post spacing in meters");
		_PostLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Posts", Geoweb3d::gtPOINT_25D, _PostDefinitions);
	}

	auto postAttributes = _PostLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	auto postProperties = _PostDefinitions->create_PropertyCollection();
	postProperties->put_Property(0, _PostSpacing);

	// Flight path ribbon layer
	if (_RibbonLayer.expired()) {
		_RibbonDefinitions = IGW3DDefinitionCollection::create();
		_RibbonDefinitions->add_Property("sections", Geoweb3d::PROPERTY_DOUBLE, "Ribbon Sections");
#ifdef __USE_MULTIPOLY
		_RibbonLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Ribbon", Geoweb3d::gtMULTI_POLYGON_25D, _RibbonDefinitions);
#else
		_RibbonLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Ribbon", Geoweb3d::gtPOLYGON_25D, _RibbonDefinitions);
#endif // __USE_MULTIPOLY
	}

	auto ribbonAttributes = _RibbonLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	auto ribbonProperties = _RibbonDefinitions->create_PropertyCollection();

	// Model layer
	if (_ModelLayer.expired()) {
		_ModelDefinitions = IGW3DDefinitionCollection::create();
		_ModelDefinitions->add_Property("FlightID", Geoweb3d::PROPERTY_STR, "Dynamic Model Layer");
		_ModelLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Model", Geoweb3d::gtPOINT_25D, _ModelDefinitions);

		// Also set the model layer to have editable geometry
		_ModelLayer.lock()->put_GeometryEditableMode(true);
	}

	auto modelAttributes = _ModelLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	auto modelProperties = _ModelDefinitions->create_PropertyCollection();
	modelProperties->put_Property(0, "OU-81 2");

	// Create a new layer that contains data about each vertex
	if (_VertexLayer.expired()) {
		_VertexDefinitions = IGW3DDefinitionCollection::create();
		_VertexDefinitions->add_Property("Not Used", Geoweb3d::PROPERTY_BOOL, "Point-to-Point Vertex Data");
		_HeadingPropertyIndex = _VertexDefinitions->add_Property("Heading", Geoweb3d::PROPERTY_BOOL, "Calculated heading to next vertex");
		_VertexLayer = _PathDataSource.lock()->get_VectorLayerCollection()->create("Heading", Geoweb3d::gtPOINT_25D, _VertexDefinitions);
	}

	auto pointPathAttributes = _VertexLayer.lock()->get_AttributeDefinitionCollection()->create_PropertyCollection();
	auto pointPathProperties = _VertexDefinitions->create_PropertyCollection();
	pathProperties->put_Property(0, "VectorData");


	// ====================================================================================================================
	// Now that we have the destination layers defined we can now go ahead and start populating them
	// ====================================================================================================================

	// Copy over the original source data for us to use later.  This isn't mandatory as the original
	// datasource can be used directly as well.  This simply shows how to copy over the feature in its
	// entirety.
	_PathLayer.lock()->create_Feature(pathProperties, geometry, retVal);

	// From here on out we're dynamically creating data...

	// First point is our starting point
	lineString->get_StartPoint(&currentPoint);
	_StartingPoint = lastRibbon = lastPoint = currentPoint;

	// It's also the starting location for the visualization model
	_ModelLayer.lock()->create_Feature(modelProperties, &currentPoint, retVal);

	// Initialize a few things
	_MaxElevation = 0.0;
	_PathLength = lineString->get_NumPoints();

	// Iterate through all points along this geometry path
	for (int i = 0; i < _PathLength; i++) {
		lastPoint = currentPoint;
		lineString->get_Point(i, &currentPoint);

		// Handle the last point gracefully
		if (i < lineString->get_NumPoints() - 1) {
			lineString->get_Point(i + 1, &nextPoint);
		}
		else {
			nextPoint = currentPoint;
		}

		// Calculate the bearing to the next point
		bearingToNextPoint->put_SourceLocation(currentPoint.get_X(), currentPoint.get_Y(), currentPoint.get_Z());
		bearingToNextPoint->put_TargetLocation(nextPoint.get_X(), nextPoint.get_Y(), nextPoint.get_Z());
		bearingToNextPoint->calculate_result(bearing, pitch, distance);

		// Store the vertex information locally for later use
		VertexInformation v;
		v.Longitude = currentPoint.get_X();
		v.Latitude = currentPoint.get_Y();
		v.Elevation = currentPoint.get_Z();
		v.DistanceToNext = distance;
		v.BearingToNext = bearing;
		v.PitchToNext = pitch;
		_Path.push_back(v);

		// Determine our maximal post elevation
		// This will be used for the height of each post to the ground.
		if (currentPoint.get_Z() > _MaxElevation)
			_MaxElevation = currentPoint.get_Z();

		// Determine where to place the next post
		// TODO : Interpolate a point to the exact spacing the current point is not exact
		// TODO : Some more notes - this is completely inaccurate as it's the straight-line
		//      : distance between the last post placed and the current point.  It should
		//      : be calculated and accumulated from point to point.
		currentPoint.get_DistanceInMeters(&lastPost, false, distance);

		if (distance >= _PostSpacing) {
			_PostLayer.lock()->create_Feature(postProperties, &currentPoint, retVal);
			lastPost = currentPoint;
			_Posts++;
		}
	}

	// Check to see if we need to add the last point
	lineString->get_EndPoint(&currentPoint);
	if (!currentPoint.equals(&lastPost)) {
		_PostLayer.lock()->create_Feature(postProperties, &currentPoint, retVal);
	}

	// Vertex post processing to allow for some smoother transitions between vertices
	for (size_t i = 0; i < _Path.size(); i++) {
		double effectiveBearing = 0.0;
		double effectivePitch = 0.0;
		int count = 0;

		double bearingX = 0.0;
		double bearingY = 0.0;
		double pitchX = 0.0;
		double pitchY = 0.0;

		// Normalize out several subsequent bearings. This isn't exactly accurate, but it will do.
		for (auto it = _Path.begin() + i; it != _Path.end() && count < 10; it++) {
			bearingX += cos(it->BearingToNext * 3.14159265358979323846 / 180.0);
			bearingY += sin(it->BearingToNext * 3.14159265358979323846 / 180.0);
			pitchX += cos(it->PitchToNext * 3.14159265358979323846 / 180.0);
			pitchY += sin(it->PitchToNext * 3.14159265358979323846 / 180.0);
			count++;
		}

		if (i != _Path.size() - 1) {
			_Path.at(i).EffectiveHeading = atan2(bearingY, bearingX) * 180.0 / 3.14159265358979323846;
			_Path.at(i).EffectivePitch = atan2(pitchY, pitchX) * 180.0 / 3.14159265358979323846;
		}
		else {
			_Path.at(i).EffectiveHeading = _Path.at(i - 1).EffectiveHeading;
			_Path.at(i).EffectivePitch = _Path.at(i - 1).EffectivePitch;
		}
	}

	// Flight path ribbon polygon generation
	double minLen = 5.0;	// EXPERIMENTAL - Set minimum segment length to 5 meters
	lastPoint.empty();

	for (auto it = _Path.begin(); it != _Path.end(); it++) {
		currentPoint.put_X(it->Longitude);
		currentPoint.put_Y(it->Latitude);
		currentPoint.put_Z(it->Elevation);

		// lastPoint should be empty for the first point, that way we can
		// ensure that we're not closing a polygon that hasn't been started yet
		if (!lastPoint.get_IsEmpty()) {
			lastPoint.get_DistanceInMeters(&currentPoint, true, distance);

			if (distance >= _SectionLength || it == _Path.end() - 1) {

				// Generate the two tail points of the currently open polygon taking into account
				// that the first point won't also be the last point.
				if (it != _Path.begin()) {
					CalcPointFromDistanceBearing(currentPoint, _PathWidth / 2.0, it->EffectiveHeading + 90.0, vertex);
					section.add_Point(&vertex);

					CalcPointFromDistanceBearing(currentPoint, _PathWidth / 2.0, it->EffectiveHeading + 270.0, vertex);
					section.add_Point(&vertex);

					// close_Rings() will stitch the last added point back to the first point to close
					// the shape
					section.close_Rings();

					// Add the new ring to the polygon
					poly.add_Ring(&section);
					poly.close_Rings();

#ifdef __USE_MULTIPOLY
					// Add the polygon section to the multipolygon
					// multi_poly.add_Geometry(&poly);
#else 
					// Finally create the polygon feature that has been generated
					_RibbonLayer.lock()->create_Feature(ribbonProperties, &poly, retVal);
#endif // __USE_MULTIPOLY

					// Empty out the section and polygon objects to reuse them
					section.empty();
					poly.empty();
				}
			}
			else {
				continue;
			}
		}

		// Start a new polygon by generating the leading vertices.
		// Also ensure that we don't start a new polygon on the last vertex in the path.
		if (it != _Path.end() - 1) {
			CalcPointFromDistanceBearing(currentPoint, _PathWidth / 2.0, it->EffectiveHeading + 270.0, vertex);
			section.add_Point(&vertex);

			CalcPointFromDistanceBearing(currentPoint, _PathWidth / 2.0, it->EffectiveHeading + 90.0, vertex);
			section.add_Point(&vertex);

#ifndef __USE_MULTIPOLY
			ribbonProperties->put_Property(0, it->EffectiveHeading);
#endif // __USE_MULTIPOLY

			lastPoint.put_X(it->Longitude);
			lastPoint.put_Y(it->Latitude);
			lastPoint.put_Z(it->Elevation);
		}
	}

		// And finally add the new geometry as a feature to this layer
#ifdef __USE_MULTIPOLY
	ribbonProperties->put_Property(0, bearing);
	multi_poly.close_Rings();
	_RibbonLayer.lock()->create_Feature(ribbonProperties, &multi_poly, retVal);
#endif // __USE_MULTIPOLY

	// If everything has proceeded properly we can return success which will
	// indicate to the streamer to proceed to the next source feature.
	if (Geoweb3d::Succeeded(retVal)) {
		return true;
	}

	// Otherwise don't continue
	return false;
}

bool FlightDataController::OnError()
{
	return false;
}

// Calculate a new geographic point based on a source point, bearing and distance
void FlightDataController::CalcPointFromDistanceBearing(const GW3DPoint & point, const double distance, double bearing, GW3DPoint& output)
{
	double rLat = point.get_Y() * 3.14159265358979323846 / 180.0;
	double rLon = point.get_X() * 3.14159265358979323846 / 180.0;
	double rAng = bearing * 3.14159265358979323846 / 180.0;
	double rDist = (distance) / (6371000.0);

	double lat = asin((sin(rLat) * cos(rDist)) + (cos(rLat) * sin(rDist) * cos(rAng)));
	double lon = rLon + atan2((sin(rAng) * sin(rDist) * cos(rLat)), cos(rDist) - (sin(rLat) * sin(lat)));

	lat = lat * 180.0 / 3.14159265358979323846;
	lon = lon * 180.0 / 3.14159265358979323846;

	output.put_X(lon);
	output.put_Y(lat);
	output.put_Z(point.get_Z());
}