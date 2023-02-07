//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Geoweb3d/core/GW3DInterFace.h"
#include "Geoweb3d/core/GW3DGUIInterFace.h"
#include <Geoweb3dCore/LayerParameters.h>
#include <sstream>
#include <algorithm>

namespace Geoweb3d
{
namespace Helpers
{

	//here set the number of additional properties per additional ring, we have 5 of them (interval and color and alpha)
	const int extra_properties_per_ring = 5;

	const std::string NUMBER_OF_ADDITIONAL_RINGS("NUMBER_OF_ADDITIONAL_RINGS");
	const std::string EXCLUDE("EXCLUDE");

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Resize a property group of range ring properties to accommodate additional rings. 
	/// 			</summary>
	///
	/// <param name="additionalrings">   	The number of additional rings. </param>
	/// <param name="input">			 	The input property group. </param>
	/// <param name="fill_new_values">		If true, copies the values of the outermost ring to any new 
	///										rings.  If false, leaves them as null. </param>
	///
	/// <returns>	The resized property collection. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static IGW3DPropertyCollectionPtr resize_ToAdditionalRings ( int additionalrings, const IGW3DPropertyCollection* input, bool fill_new_values = true )
	{

		if (!input)
		{
			return IGW3DPropertyCollectionPtr();
		}

		int number_of_additional_rings_index = input->get_DefinitionCollection()->get_IndexByName(NUMBER_OF_ADDITIONAL_RINGS.c_str());

		if (number_of_additional_rings_index < 0)
		{
			return IGW3DPropertyCollectionPtr();
		}

		int exclude_index = input->get_DefinitionCollection()->get_IndexByName(EXCLUDE.c_str());
		bool isFromAttribMap = (exclude_index > -1 );  //Attribute Maps (GUI SDK) have an additional property 'EXCLUDE'

		int max_base_property_index = isFromAttribMap ? exclude_index : number_of_additional_rings_index;

		int number_of_fields_base = max_base_property_index + 1;
		int number_of_fields_needed = number_of_fields_base + ( extra_properties_per_ring * additionalrings ); // 5 additional properties per ring (interval,r,g,b,a)

		if (number_of_fields_needed == input->count())
		{
			//no resize needed
			return IGW3DPropertyCollectionPtr();
		}

		IGW3DDefinitionCollectionPtr def_copy = IGW3DDefinitionCollection::create();

		//rebuild the base property collection
		for(int i = 0; i <= max_base_property_index; i++)
		{
			const char* name = input->get_DefinitionCollection()->get_AtIndex(i)->property_name;
			Geoweb3d::PropertyType type = input->get_DefinitionCollection()->get_AtIndex(i)->property_type;
			const char* description = input->get_DefinitionCollection()->get_AtIndex(i)->property_description;
			int length = input->get_DefinitionCollection()->get_AtIndex(i)->property_length;
			def_copy->add_Property(name, type, description, length);
		}

		static std::string prefix ( "RING_" );
		static std::string interval_suffix ( "_INTERVAL" );
		static std::string red_suffix ( "_RED" );
		static std::string green_suffix ( "_GREEN" );
		static std::string blue_suffix ( "_BLUE" );
		static std::string alpha_suffix ( "_ALPHA" );

		std::ostringstream final_name;

		//add the properties for the given number of rings
		for(int i = 0; i < additionalrings; i++)
		{
			int ring_number = i+1; //first additional ring is ring 1
			final_name.str("");
			final_name.clear();
			final_name << prefix.c_str() << ring_number << interval_suffix.c_str();
			def_copy->add_Property( final_name.str().c_str(), PROPERTY_DOUBLE, "meters" );

			final_name.str("");
			final_name.clear();
			final_name << prefix.c_str() << ring_number << red_suffix.c_str();
			def_copy->add_Property( final_name.str().c_str(), PROPERTY_DOUBLE, "0 to 1" );

			final_name.str("");
			final_name.clear();
			final_name << prefix.c_str() << ring_number << green_suffix.c_str();
			def_copy->add_Property( final_name.str().c_str(), PROPERTY_DOUBLE, "0 to 1" );

			final_name.str("");
			final_name.clear();
			final_name << prefix.c_str() << ring_number << blue_suffix.c_str();
			def_copy->add_Property( final_name.str().c_str(), PROPERTY_DOUBLE, "0 to 1" );

			final_name.str("");
			final_name.clear();
			final_name << prefix.c_str() << ring_number << alpha_suffix.c_str();
			def_copy->add_Property( final_name.str().c_str(), PROPERTY_DOUBLE, "0 to 1" );
		}
		
		IGW3DPropertyCollectionPtr output = def_copy->create_PropertyCollection( true );

		unsigned max_existing_properties = std::min<unsigned>(input->count(),output->count()); //If the size shrank, don't overrun

		//fill in the properties that already existed
		int property_index = 0;
		for ( ; property_index < max_existing_properties; ++property_index)
		{
			output->put_Property(property_index, input->get_Property(property_index));
		}

		//override with the new number of additional rings
		output->put_Property( number_of_additional_rings_index, additionalrings );

		int ring_1_interval_index = max_base_property_index + 1;
		int ring_1_red_index = max_base_property_index + 2;
		int ring_1_green_index = max_base_property_index + 3;
		int ring_1_blue_index = max_base_property_index + 4;
		int ring_1_alpha_index = max_base_property_index + 4;

		if (fill_new_values)
		{
			//fill in any additional ring properties that didn't exist in the input (use ring 0 values as defaults)
			for(int i = 0; i < additionalrings; i++)
			{		
				int property_to_set = ring_1_interval_index + (extra_properties_per_ring  * i);
				if (property_to_set >= input->count())
				{
					output->put_Property( property_to_set, input->get_Property(Vector::RingProperties::RING_0_INTERVAL) ); 
				}
				
				property_to_set = ring_1_red_index + (extra_properties_per_ring  * i);
				if (property_to_set >= input->count())
				{
					output->put_Property( property_to_set, input->get_Property(Vector::RingProperties::RED) ); 
				}

				property_to_set = ring_1_green_index + (extra_properties_per_ring  * i);
				if (property_to_set >= input->count())
				{
					output->put_Property( property_to_set, input->get_Property(Vector::RingProperties::GREEN) ); 
				}

				property_to_set = ring_1_blue_index + (extra_properties_per_ring  * i);
				if (property_to_set >= input->count())
				{
					output->put_Property( property_to_set, input->get_Property(Vector::RingProperties::BLUE) ); 
				}

				property_to_set = ring_1_alpha_index + (extra_properties_per_ring  * i);
				if (property_to_set >= input->count())
				{
					output->put_Property( property_to_set, input->get_Property(Vector::RingProperties::ALPHA) ); 
				}
			}
		}
		return output;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Resize a property group of range ring properties to accommodate additional rings. 
	/// 			</summary>
	///
	/// <param name="additionalrings">   	The number of additional rings. </param>
	/// <param name="input">			 	The input property group. </param>
	/// <param name="fill_new_values">		If true, copies the values of the outermost ring to any new 
	///										rings.  If false, leaves them as null. </param>
	///
	/// <returns>	The resized property collection. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static IGW3DPropertyCollectionPtr resize_ToAdditionalRings ( int additionalrings, const IGW3DPropertyCollectionPtr input, bool fill_new_values = true )
	{
		return resize_ToAdditionalRings ( additionalrings, input.get(), fill_new_values );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a ring interval. </summary>
	///
	/// <param name="ring">	   	The ring. </param>
	/// <param name="input">   	The input. </param>
	/// <param name="interval">	The interval. </param>
	///
	/// <returns> TODO </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static GW3DResult put_RingInterval ( int ring, IGW3DPropertyCollectionPtr input, const double& interval )
	{
		
		if (!input || ring < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		if (ring == 0)
		{
			static std::string RING_0_INTERVAL("RING_0_INTERVAL");
			int ring_0_interval_index = input->get_DefinitionCollection()->get_IndexByName(RING_0_INTERVAL.c_str());

			if (ring_0_interval_index < 0)
			{
				return Geoweb3d::GW3D_eInvalidArgument;
			}
			return input->put_Property(ring_0_interval_index, interval);		
		}

		int number_of_additional_rings_index = input->get_DefinitionCollection()->get_IndexByName(NUMBER_OF_ADDITIONAL_RINGS.c_str());

		if (number_of_additional_rings_index < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		int exclude_index = input->get_DefinitionCollection()->get_IndexByName(EXCLUDE.c_str());
		bool isFromAttribMap = (exclude_index > -1 );  //Attribute Maps (GUI SDK) have an additional property 'EXCLUDE'

		int max_base_property_index = isFromAttribMap ? exclude_index : number_of_additional_rings_index;

		int additional_ring = ring - 1;  //ring 0 isn't additional

		const int internal_offset = 1;

		int interval_property_index = max_base_property_index + internal_offset + (extra_properties_per_ring * additional_ring);
		if (interval_property_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}
		return input->put_Property(interval_property_index, interval);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets ring interval. </summary>
	///
	/// <param name="ring">	   	The ring. </param>
	/// <param name="input">   	The input. </param>
	/// <param name="interval">	[in,out] The interval. </param>
	///
	/// <returns>	The ring interval. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static GW3DResult get_RingInterval (int ring, const IGW3DPropertyCollection* input, double& interval)
	{
		if (!input || ring < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		if (ring == 0)
		{
			static std::string RING_0_INTERVAL("RING_0_INTERVAL");
			int ring_0_interval_index = input->get_DefinitionCollection()->get_IndexByName(RING_0_INTERVAL.c_str());

			if (ring_0_interval_index < 0)
			{
				return Geoweb3d::GW3D_eInvalidArgument;
			}
		
			interval = input->get_Property(ring_0_interval_index);		
			return Geoweb3d::GW3D_sOk;

		}

		int number_of_additional_rings_index = input->get_DefinitionCollection()->get_IndexByName(NUMBER_OF_ADDITIONAL_RINGS.c_str());

		if (number_of_additional_rings_index < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		int exclude_index = input->get_DefinitionCollection()->get_IndexByName(EXCLUDE.c_str());
		bool isFromAttribMap = (exclude_index > -1 );  //Attribute Maps (GUI SDK) have an additional property 'EXCLUDE'

		int max_base_property_index = isFromAttribMap ? exclude_index : number_of_additional_rings_index;

		int additional_ring = ring - 1;  //ring 0 isn't additional

		const int internal_offset = 1;

		int interval_property_index = max_base_property_index + internal_offset + (extra_properties_per_ring * additional_ring);
		if (interval_property_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}
		interval = input->get_Property(interval_property_index);
		return Geoweb3d::GW3D_sOk;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets ring color. </summary>
	///
	/// <param name="ring"> 	The ring. </param>
	/// <param name="input">	The input. </param>
	/// <param name="r">		Red. </param>
	/// <param name="g">		Green. </param>
	/// <param name="b">		Blue. </param>
	/// <param name="a">		Alpha. </param>
	///
	/// <returns> TODO </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static GW3DResult put_RingColor ( int ring, IGW3DPropertyCollectionPtr input, const double& r, const double& g, const double& b, const double& a)
	{
		if (!input || ring < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		if (ring == 0)
		{
			static std::string RED("RED");
			static std::string GREEN("GREEN");
			static std::string BLUE("BLUE");
			static std::string ALPHA("ALPHA");
			int ring_0_red_index = input->get_DefinitionCollection()->get_IndexByName(RED.c_str());
			int ring_0_green_index = input->get_DefinitionCollection()->get_IndexByName(GREEN.c_str());
			int ring_0_blue_index = input->get_DefinitionCollection()->get_IndexByName(BLUE.c_str());
			int ring_0_alpha_index = input->get_DefinitionCollection()->get_IndexByName(ALPHA.c_str());

			if (ring_0_red_index < 0 || ring_0_green_index < 0 || ring_0_blue_index < 0 || ring_0_alpha_index < 0)
			{
				return Geoweb3d::GW3D_eInvalidArgument;
			}
		
			input->put_Property(ring_0_red_index, r);		
			input->put_Property(ring_0_green_index, g);		
			input->put_Property(ring_0_blue_index, b);		
			input->put_Property(ring_0_alpha_index, a);		
			return Geoweb3d::GW3D_sOk;

		}

		int number_of_additional_rings_index = input->get_DefinitionCollection()->get_IndexByName(NUMBER_OF_ADDITIONAL_RINGS.c_str());

		if (number_of_additional_rings_index < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		int exclude_index = input->get_DefinitionCollection()->get_IndexByName(EXCLUDE.c_str());
		bool isFromAttribMap = (exclude_index > -1 );  //Attribute Maps (GUI SDK) have an additional property 'EXCLUDE'

		int max_base_property_index = isFromAttribMap ? exclude_index : number_of_additional_rings_index;

		int additional_ring = ring - 1;  //ring 0 isn't additional

		const int red_internal_offset = 2;
		const int green_internal_offset = 3;
		const int blue_internal_offset = 4;
		const int alpha_internal_offset = 5;

		int red_index = max_base_property_index + red_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (red_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		input->put_Property( red_index, r ); 

		int green_index = max_base_property_index + green_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (green_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		input->put_Property( green_index, g );  

		int blue_index = max_base_property_index + blue_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (blue_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		input->put_Property( blue_index, b ); 

		int alpha_index = max_base_property_index + alpha_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (alpha_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		input->put_Property( alpha_index, a ); 
		
		return Geoweb3d::GW3D_sOk;

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets ring color. </summary>
	///
	/// <param name="ring"> 	The ring. </param>
	/// <param name="input">	The input. </param>
	/// <param name="r">		Red. </param>
	/// <param name="g">		Green. </param>
	/// <param name="b">		Blue. </param>
	/// <param name="a">		Alpha. </param>
	///
	/// <returns>	The ring color. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static GW3DResult get_RingColor ( int ring, const IGW3DPropertyCollection* input,  double& r, double& g, double& b, double& a ) 
	{
		if (!input || ring < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		if (ring == 0)
		{
			static std::string RED("RED");
			static std::string GREEN("GREEN");
			static std::string BLUE("BLUE");
			static std::string ALPHA("ALPHA");
			int ring_0_red_index = input->get_DefinitionCollection()->get_IndexByName(RED.c_str());
			int ring_0_green_index = input->get_DefinitionCollection()->get_IndexByName(GREEN.c_str());
			int ring_0_blue_index = input->get_DefinitionCollection()->get_IndexByName(BLUE.c_str());
			int ring_0_alpha_index = input->get_DefinitionCollection()->get_IndexByName(ALPHA.c_str());

			if (ring_0_red_index < 0 || ring_0_green_index < 0 || ring_0_blue_index < 0 || ring_0_alpha_index < 0)
			{
				return Geoweb3d::GW3D_eInvalidArgument;
			}
		
			r = input->get_Property(ring_0_red_index);		
			g = input->get_Property(ring_0_green_index);		
			b = input->get_Property(ring_0_blue_index);		
			a = input->get_Property(ring_0_alpha_index);		
			return Geoweb3d::GW3D_sOk;

		}

		int number_of_additional_rings_index = input->get_DefinitionCollection()->get_IndexByName(NUMBER_OF_ADDITIONAL_RINGS.c_str());

		if (number_of_additional_rings_index < 0)
		{
			return Geoweb3d::GW3D_eInvalidArgument;
		}

		int exclude_index = input->get_DefinitionCollection()->get_IndexByName(EXCLUDE.c_str());
		bool isFromAttribMap = (exclude_index > -1 );  //Attribute Maps (GUI SDK) have an additional property 'EXCLUDE'

		int max_base_property_index = isFromAttribMap ? exclude_index : number_of_additional_rings_index;

		int additional_ring = ring - 1;  //ring 0 isn't additional

		const int red_internal_offset = 2;
		const int green_internal_offset = 3;
		const int blue_internal_offset = 4;
		const int alpha_internal_offset = 5;

		int red_index = max_base_property_index + red_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (red_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		r = input->get_Property( red_index ); 

		int green_index = max_base_property_index + green_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (green_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		g = input->get_Property( green_index );  

		int blue_index = max_base_property_index + blue_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (blue_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		b = input->get_Property( blue_index ); 

		int alpha_index = max_base_property_index + alpha_internal_offset + (extra_properties_per_ring  * additional_ring);
		if (alpha_index >= input->count())
		{
			return Geoweb3d::GW3D_eArgumentOutOfRange;
		}

		a = input->get_Property( alpha_index ); 

		return Geoweb3d::GW3D_sOk;

	}
}
}
