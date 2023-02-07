#pragma once

#include "core/GW3DGUIInterFace.h"
#include "gui/IGW3DGUIXMLReadContext.h"
#include "gui/IGW3DGUIXMLWriteContext.h"

class MyCompanyExtension;

#include <QtCore/QObject>

class XMLProcessor : public QObject
{
		Q_OBJECT

public:
    XMLProcessor(MyCompanyExtension* app);

	void XMLRead (Geoweb3d::GUI::Geoweb3dFileType fileType, const char *xmlBuffer);
	void XMLWrite (Geoweb3d::GUI::IGW3DGUIXMLWriteContext* ctx);
private:

	MyCompanyExtension* app_;
};
