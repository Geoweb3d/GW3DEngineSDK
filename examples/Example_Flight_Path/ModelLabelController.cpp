#include "stdafx.h"

#include "ModelLabelController.h"

ModelLabelController::ModelLabelController()
{
}

ModelLabelController::~ModelLabelController()
{
}

bool ModelLabelController::OnStream(IGW3DContentsResult * contents, IGW3DFeatureFeedbackResultCollection * featurefeedback)
{	
	if (contents->get_VectorRepresentationIsVisible()) {
		std::string layerName = contents->get_VectorRepresentation().lock()->get_VectorLayer().lock()->get_Name();
		featurefeedback->reset();

		// Are we working with the model layer?  If not we don't care.
		if (layerName == "Model" && featurefeedback->count()) {
			// There should be only 1 feature - the model.
			auto feature = featurefeedback->get_AtIndex(0);
			feature->get_FeatureID2DPixelLocationResult()->get_WindowCoordinate(_X, _Y);
			return true;
		}
	}

	return true;
}

bool ModelLabelController::OnError()
{
	return false;
}

void ModelLabelController::GetWindowPosition(int & x, int & y)
{
	x = _X;
	y = _Y;
}

std::string ModelLabelController::GetName()
{
	return _Name;
}
