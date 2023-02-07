#pragma once
#include "StdAfx.h"

class QSettings;

class SettingsManager
{

public:
	SettingsManager ();
	~SettingsManager ();

	void Initialize();

	int GetNumberOfFramesForCustomShelfExample();

private:
	QSettings* app_settings_;

};

