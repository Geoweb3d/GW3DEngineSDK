#include "StdAfx.h"
#include "ExtrudedPathController.h"

#include <Geoweb3d\common\GW3DCommon.h>
#include <Geoweb3d\engine\GW3DVector.h>
#include <Geoweb3dCore\LayerParameters.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="rep">	The rep. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

ExtrudedPathController::ExtrudedPathController()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Destructor. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

ExtrudedPathController::~ExtrudedPathController()
{}

void ExtrudedPathController::SetRepresentation(Geoweb3d::IGW3DVectorRepresentationWPtr rep)
{
	rep_ = rep;
}

void ExtrudedPathController::SetLayer(Geoweb3d::IGW3DVectorLayerWPtr dynamic_layer)
{
	dynamic_layer_ = dynamic_layer;
}

void ExtrudedPathController::SetLocation(double lon, double lat)
{
	lon_ = lon;
	lat_ = lat;
}

void ExtrudedPathController::SetDragMode(bool drag_mode)
{
	drag_mode_ = drag_mode;
}
void ExtrudedPathController::SetWorkingFID(long working_fid)
{
	working_fid_ = working_fid;
}

void ExtrudedPathController::CaptureAScribPoint()
{
	printf("Adding point[%d] to the scribing list\n", (int)scribing_pts_.size());
	Geoweb3d::GW3DPoint new_pt;
	new_pt.put_X(lon_);
	new_pt.put_Y(lat_);
	scribing_pts_.push_back(new_pt);
}

void ExtrudedPathController::CreateScribedLine(Geoweb3d::IGW3DPropertyCollectionPtr defaults)
{
	printf("Attemping to create the feature that was being scribed.\n");
	if (!dynamic_layer_.expired())
	{
		auto ep_layer_locked = dynamic_layer_.lock();

		if (scribing_pts_.size() > 1)
		{
			//once delete feature is ironed out - we can show the line getting built
			/*if (working_fid_ >= 0)
			{
				ep_layer_locked->delete_Feature(working_fid_);
				working_fid_ = -1;
			}*/

			Geoweb3d::GW3DLineString line_string;
			for (int i = 0; i < scribing_pts_.size(); i++)
			{
				line_string.add_Point(&scribing_pts_[i]);
			}

			scribing_pts_.clear();

			Geoweb3d::IGW3DDefinitionCollectionPtr field_definition(Geoweb3d::IGW3DDefinitionCollection::create());
			Geoweb3d::IGW3DPropertyCollectionPtr field_values(field_definition->create_PropertyCollection());

			Geoweb3d::GW3DResult result;
			long new_fid = ep_layer_locked->create_Feature(field_values, &line_string, result);

			if (!Geoweb3d::Succeeded(result) || new_fid < 0)
			{
				printf("Failed to create and represent extruded path scribed line string\n");
			}
			else
			{
				working_fid_ = new_fid;
				printf("Created feature id: %d\n", new_fid);
			}
		}
	}
}

void ExtrudedPathController::StopScribingLine()
{
	scribing_pts_.clear();
}

void ExtrudedPathController::DeleteFeature()
{
	if (!dynamic_layer_.expired())
	{
		auto ep_layer_locked = dynamic_layer_.lock();

		if (working_fid_ >= 0)
		{
			ep_layer_locked->delete_Feature(working_fid_);
			working_fid_ = -1;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the stream action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
///
/// <param name="result">	[out] if non-null, the result. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ExtrudedPathController::OnStream(Geoweb3d::IGW3DVectorLayerStreamResult* result)
{
	const Geoweb3d::IGW3DAttributeCollection* attributesconst = result->get_AttributeCollection();
	//Geoweb3d::IGW3DAttributeCollection *attributes = result->get_ModifyableAttributeProperties() ;

	const Geoweb3d::IGW3DVectorLayer* layer = result->get_VectorLayer();

	Geoweb3d::GW3DGeometry* geometry = result->get_EditableGeometry();
	long object_id = result->get_ObjectID();

	if (object_id == working_fid_)// && drag_mode_)
	{
		if (!geometry) //must be we are not in edit mode????
			return false;

		if (geometry->get_GeometryType() == Geoweb3d::gtLINE_STRING_25D
			|| geometry->get_GeometryType() == Geoweb3d::gtLINE_STRING)
		{
			Geoweb3d::GW3DPoint pt;

			if (drag_mode_)
			{
				Geoweb3d::GW3DLineString* line = dynamic_cast<Geoweb3d::GW3DLineString*>(geometry);
				int num_pts = line->get_NumPoints();

				Geoweb3d::GW3DPoint closest_pt;
				Geoweb3d::GW3DPoint new_pt;
				new_pt.put_X(lon_);
				new_pt.put_Y(lat_);

				double closest_dist;
				int closest_pt_idx = 0;

				if (num_pts > 0)
				{
					for (int i = 0; i < num_pts; i++)
					{
						line->get_Point(i, &pt);
						double lon_delta = lon_ - pt.get_X();
						double lat_delta = lat_ - pt.get_Y();

						if (i == 0)
						{
							closest_pt = pt;
							closest_dist = std::sqrt(lon_delta * lon_delta + lat_delta * lat_delta);
						}
						else
						{
							double distance = std::sqrt(lon_delta * lon_delta + lat_delta * lat_delta);
							if (distance < closest_dist)
							{
								closest_pt = pt;
								closest_dist = distance;
								closest_pt_idx = i;
							}
						}
					}

					line->put_Point(closest_pt_idx, &new_pt);


				}
				
			}
			else
			{
				/*Geoweb3d::GW3DLineString* line = dynamic_cast<Geoweb3d::GW3DLineString*>(geometry);
				int num_pts = line->get_NumPoints();

				if (num_pts > 0)
				{
					line->get_Point(0, &pt);
					line->put_Point(0, &pt);

				}*/

				const Geoweb3d::IGW3DPropertyCollection* representation_properties = result->get_VectorRepresentationProperties(rep_);
				if (!representation_properties)
				{
					//unsafe_get saves some overhead vs a lock, but should only be used when you are sure the object can't get deleted while you are using it
					representation_properties = rep_.unsafe_get()->get_PropertyCollection();
				}

				if (representation_properties)
				{
					//two ways of defining the target property:
					//static int property_index = Vector::ModelProperties::HEADING; //by enum...
					static int width_property_index = representation_properties->get_DefinitionCollection()->get_IndexByName("LEFT_DISTANCE"); //...or, by string

					//this is the optimized way to set the representation property!
					double width = representation_properties->get_Property(Geoweb3d::Vector::ExtrudedPathProperties::IndividualProperties::LEFT_DISTANCE);
					result->put_VectorRepresentationProperty(rep_, width_property_index, width);

				}
			}
		}
		else
		{
			printf("Need to add code for this other geometry type\n");
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Executes the error action. </summary>
///
/// <remarks>	Geoweb3d, 11/2/2012. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ExtrudedPathController::OnError(/*todo*/)
{
	//something was done by the user in the streamer that messed things up so bad we have to tell you about it.
	return false;
}