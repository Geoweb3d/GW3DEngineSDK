#pragma once

#include "core/GW3DGUIInterFace.h"

#include <deque>

class TocTools
{
public:

    TocTools();
    ~TocTools();

	std::string SummarizeTocVectorLayers (Geoweb3d::GUI::IGW3DGUITocItemCollection *toc_root);

private:

	void CollectSummaryData (Geoweb3d::GUI::IGW3DGUITocItemCollection* items);

	std::deque<std::string> vector_layers;
	std::deque<std::string> vector_representations;
	Geoweb3d::GW3DEnvelope* vector_total_envelope;

};
