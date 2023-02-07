#pragma once

using namespace Geoweb3d;

class ModelLabelController : public IGW3DFrustumAnalysisStream
{
public:
	ModelLabelController();
	~ModelLabelController();

	// Inherited via IGW3DFrustumAnalysisStream
	virtual bool OnStream(IGW3DContentsResult * contents, IGW3DFeatureFeedbackResultCollection * featurefeedback) override;
	virtual bool OnError() override;

	void GetWindowPosition(int& x, int& y);
	std::string GetName();

private:
	int _X;
	int _Y;
	std::string _Name;
};
