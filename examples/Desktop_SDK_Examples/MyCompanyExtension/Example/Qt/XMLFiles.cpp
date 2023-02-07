#include "../stdafx.h"

#include <sstream>
#include <string>

#include "XMLFiles.h"

#include "../MyCompanyExtension.h"

#include "gui/GW3DGUITableOfContents.h"
#include "gui/GW3DGUIVector.h"

#include <QtXml/QDomDocument>
#include <QtCore/QXmlStreamWriter>

XMLProcessor::XMLProcessor(MyCompanyExtension* app)
	:	app_(app)
{}

void XMLProcessor::XMLRead (Geoweb3d::GUI::Geoweb3dFileType fileType, const char *xmlBuffer)
{
	if (!xmlBuffer)
	{
		return;
	}

	switch (fileType) {

		case Geoweb3d::GUI::Geoweb3dFileTypeProject:
			if (xmlBuffer != NULL) {
				app_->LogMessage("read custom project xml :: \n %s\n", xmlBuffer);
			} else {
				app_->LogMessage("project file loaded \n");
			}
			break;

		case Geoweb3d::GUI::Geoweb3dFileTypeLayer:
			if (xmlBuffer != NULL) {
				app_->LogMessage("read custom layer xml :: \n %s\n", xmlBuffer);
			} else {
				app_->LogMessage("layer file loaded \n");
			}
			break;

		case Geoweb3d::GUI::Geoweb3dFileTypeRepresentation:
			if (xmlBuffer != NULL) {
				app_->LogMessage("read custom representation xml :: \n %s\n", xmlBuffer);
			} else {
				app_->LogMessage("representation file loaded \n");
			}
			break;

		default:
			break;
	}
}


void XMLProcessor::XMLWrite (Geoweb3d::GUI::IGW3DGUIXMLWriteContext* ctx) 
{
	QString result;

	// Users can leverage Qt XML
	QXmlStreamWriter stream (&result);
	stream.setAutoFormatting (true);

	stream.writeStartElement ("Overlay");
	stream.writeAttribute ("href", "https://maps.google.com/");
	stream.writeTextElement ("title", "Overlay title");

	stream.writeStartElement ("Overlay_Item");
	stream.writeAttribute ("href", "https://maps.google.com/");
	stream.writeTextElement ("title", "Overlay item title");
	stream.writeEndElement (); 

	//....

	stream.writeEndDocument (); 

	QByteArray data(result.toLocal8Bit()); 

	// Validate XML
	QDomDocument domDoc;
	QString error;
	int line, col;
	bool validXML = true;

	if(!domDoc.setContent (data, &error, &line, &col)) {
		validXML = false;
		app_->LogMessage(" ** error at line: %d line %d column\n", line, col);
		app_->LogMessage(data.constData());
	} 

	if (validXML) {

		switch (ctx->get_FileType()) {

			case Geoweb3d::GUI::Geoweb3dFileTypeProject:
				ctx->put_XML (data.constData()); 
				break;

			case Geoweb3d::GUI::Geoweb3dFileTypeLayer:
				ctx->put_XML (data.constData());
				break;

			case Geoweb3d::GUI::Geoweb3dFileTypeRepresentation:
				ctx->put_XML (data.constData());
				break;

			default:
				break;
		}

		if ( ctx->get_XML())
		{
			app_->LogMessage("Wrote custom xml to file\n%s\n", ctx->get_XML());
		}
		else
		{
			app_->LogMessage("No custom data written to the file!");
		}

	}
}