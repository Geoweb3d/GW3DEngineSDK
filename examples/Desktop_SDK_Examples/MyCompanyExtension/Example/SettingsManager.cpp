#include "stdafx.h"
#include "SettingsManager.h"
#include <QtCore/QSettings>

// Inherets from a QFrame
SettingsManager::SettingsManager ()
	: app_settings_ ( new QSettings("L3Com", "HDWorld"))
{

} 

SettingsManager::~SettingsManager () 
{
}

int SettingsManager::GetNumberOfFramesForCustomShelfExample()
{
	return app_settings_->value("Frames/FramesOnCustomShelf", QVariant(4)).toInt();
}