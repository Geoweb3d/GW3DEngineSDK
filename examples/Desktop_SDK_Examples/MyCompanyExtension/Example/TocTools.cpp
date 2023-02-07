#include "stdafx.h"

#include <sstream>

#include "TocTools.h"

#include "gui/GW3DGUITableOfContents.h"
#include "gui/GW3DGUIVector.h"

TocTools::TocTools()
:	vector_total_envelope(0)
{
}

TocTools::~TocTools()
{
}

std::string TocTools::SummarizeTocVectorLayers(Geoweb3d::GUI::IGW3DGUITocItemCollection *toc_root)
{
	vector_total_envelope = 0;

	CollectSummaryData( toc_root );

	std::ostringstream vector_report;
	vector_report << "REPORT OF ALL VECTOR LAYERS IN TOC:\n";
	vector_report << "-----------------------------------\n";
	vector_report << "Number of vector layers: " << vector_layers.size() << "\n";
	vector_report << "Number of vector representations: " << vector_representations.size() << "\n";
	if (vector_total_envelope)
	{
		vector_report << "Total envelope of all vector layers: \n";
		vector_report << "   Max_X: " << vector_total_envelope->MaxX << "\n";
		vector_report << "   Min_X: " << vector_total_envelope->MinX << "\n";
		vector_report << "   Max_Y: " << vector_total_envelope->MaxY << "\n";
		vector_report << "   Min_Y: " << vector_total_envelope->MinY << "\n";
	}
	vector_report << "-----------------------------------\n";
	delete vector_total_envelope;
	vector_total_envelope = 0;
	return vector_report.str();

}

void TocTools::CollectSummaryData (Geoweb3d::GUI::IGW3DGUITocItemCollection* items)
{
	items->reset();

	Geoweb3d::GUI::IGW3DGUITocItemWPtr tocItem;
	while (items->next(&tocItem))
	{
		if (tocItem.lock()->get_Type() == Geoweb3d::GUI::IGW3DGUITocItem::VectorLayer)
		{
			Geoweb3d::GUI::IGW3DGUIVectorLayerWPtr vLayer = tocItem.lock()->get_VectorLayer();
			vector_layers.push_back(std::string(tocItem.lock()->get_Name()));

			if (!vector_total_envelope)
			{
				vector_total_envelope = new Geoweb3d::GW3DEnvelope();
			}
			vector_total_envelope->merge(vLayer.lock()->get_Envelope());
		}
		else if (tocItem.lock()->get_Type() == Geoweb3d::GUI::IGW3DGUITocItem::VectorRepresentation)
		{
			vector_representations.push_back(std::string(tocItem.lock()->get_Name()));
		}

		if (tocItem.lock()->get_ChildCollection()->count())
		{
			CollectSummaryData(tocItem.lock()->get_ChildCollection());
		}
	}
}
