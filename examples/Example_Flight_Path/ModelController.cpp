#include "stdafx.h"
#include "ModelController.h"


ModelController::ModelController()
{
}

ModelController::~ModelController()
{
}

void ModelController::UpdateModel(const GW3DPoint & location, const double bearing, const IGW3DVectorLayerWPtr & model)
{
	// Update the model layer with a new location and heading
	if (!model.expired())
	{
		_ModelLocation = location;
		_ModelBearing = bearing;

		// Loop through all representations for this layer and stream an update to it.
		auto modelCount = model.lock()->get_RepresentationLayerCollection()->count();
		for (unsigned long i = 0; i < modelCount; i++) {
			_ModelRepresentation = model.lock()->get_RepresentationLayerCollection()->get_AtIndex(i);
			model.lock()->Stream(this);
		}
	}
}

void ModelController::GetCurrentLocation(GW3DPoint & location, double & bearing) const
{
	location = _ModelLocation;
	bearing = _ModelBearing;
}

unsigned long ModelController::count() const
{
	return 0;
}

bool ModelController::next(int64_t* ppVal)
{
	return false;
}

void ModelController::reset()
{
}

int64_t ModelController::operator[](unsigned long index)
{
	return 0;
}

int64_t ModelController::get_AtIndex(unsigned long index)
{
	return 0;
}

bool ModelController::OnStream(IGW3DVectorLayerStreamResult * result)
{
	// The model should have been tagged with editable geometry so that
	// we can update the location as we go.
	auto geometry = result->get_EditableGeometry();
	
	// Always test if the geometry pointer is valid.
	if (geometry != nullptr && geometry->get_GeometryType() == gtPOINT_25D) {
		auto location = dynamic_cast<GW3DPoint*>(geometry);
		*location = _ModelLocation;
	}

	// Get the individual properties for this feature, for the model representation
	auto props = result->get_VectorRepresentationProperties(_ModelRepresentation);

	// If the feature doesn't have individual properties (i.e. it's not unique)...
	if (!props) {
		props = _ModelRepresentation.lock()->get_PropertyCollection();
	}

	// Because we are potentially working with multiple different representations
	// we retrieve this representation's HEADING feature by name which will give
	// the appropriate property index, or -1 if it doesn't exist.
	auto propertyIndex = props->get_DefinitionCollection()->get_IndexByName("HEADING");

	// And set it
	if (propertyIndex != -1) {
		result->put_VectorRepresentationProperty(_ModelRepresentation, propertyIndex, _ModelBearing);
	}

	return true;
}

bool ModelController::OnError()
{
	return false;
}
