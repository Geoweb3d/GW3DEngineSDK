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
#include "../core/GW3DInterFace.h"

namespace Geoweb3d
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A color lookup table. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct GW3D_DLL IGW3DColorLookupTable
	{
		virtual ~IGW3DColorLookupTable() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the minimum and maximum scalar values for scalar mapping. Using put_Range will
		///				cause the set range to be expanded </summary>
		///
		/// <remarks>	put_TableRange only needs to be set in cases where values configured via put_Range do
		/// 			not fully represent the entire range of the table. Missing values will get filled with the color
		///				configured using put_NoDataColor. </remarks>
		///
		/// <param name="min_value">	The upper bound of the table. </param>
		/// <param name="max_value">	The lower bound of the table. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_TableRange(int min_value, int max_value) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the color for the specified range. Note: Any values that overlap a previous range
		///				will be updated with this new color. </summary>
		///
		/// <param name="min_value">	The upper bound of the range. </param>
		/// <param name="max_value">	The lower bound of the range. </param>
		/// <param name="red">	The red value. </param>
		/// <param name="green">	The green value. </param>
		/// <param name="blue">	The blue value. </param>
		/// <param name="alpha">	The alpha value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_Range(int min_value, int max_value, float red, float green, float blue, float alpha) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the color that will represent no data in the lookup table. This applies to any 
		///				values in the table that have not been configured with a color. </summary>
		///
		/// <param name="red">	The red value. </param>
		/// <param name="green">	The green value. </param>
		/// <param name="blue">	The blue value. </param>
		/// <param name="alpha">	The alpha value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_NoDataColor(float red, float green, float blue, float alpha) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Enables/Disables using a custom color for values below the configured minimum value. </summary>
		/// 
		/// <remarks>	When disabled, values below the configured minimum will be clamped to the minimum
		///				color. </remarks>
		/// 
		/// <param name="enable">	true if the below range color is to be used. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_UseBelowRangeColor(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the color that will represent values above the configured minimum value when 
		/// 			put_UseBelowRangeColor is enabled. </summary>
		///
		/// <param name="red">	The red value. </param>
		/// <param name="green">	The green value. </param>
		/// <param name="blue">	The blue value. </param>
		/// <param name="alpha">	The alpha value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_BelowRangeColor(float red, float green, float blue, float alpha) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary> Enables/Disables using a custom color for values above the configured maximum value. </summary>
		/// 
		/// <remarks>	When disabled, values above the configured maximum will be clamped to the maximum 
		/// 			color. </remarks>
		/// 
		/// <param name="enable">	true if the below range color is to be used. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_UseAboveRangeColor(bool enable) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the color that will represent values above the configured minimum value when 
		///				put_UseAboveRangeColor is enabled. </summary>
		///
		/// <param name="red">	The red value. </param>
		/// <param name="green">	The green value. </param>
		/// <param name="blue">	The blue value. </param>
		/// <param name="alpha">	The alpha value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_AboveRangeColor(float red, float green, float blue, float alpha) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Save the color lookup table to disk. </summary>
		///
		/// <remarks>	Saves the color lookup table to disk with the given filename. </remarks>
		///
		/// <param name="file">	The output filename. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_ToDisk(const char* file) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	loads the color lookup table from disk. </summary>
		///
		/// <remarks>	Loads the color lookup table off disk from the given filename. </remarks>
		///
		/// <param name="file">	The input filename. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult get_FromDisk(const char* file) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a finalization token. </summary>
		///
		/// <remarks>	Once a color lookup table is populated and configured as desired, create a
		/// 			finalization token for use in the Geoweb3d SDK, such as a Terrain Avoidance Test or Viewshed Test.
		/// 			Subsequent changes to the color lookup table and its colors will have no effect on the
		/// 			configuration represented by the token. </remarks>
		///
		/// <returns>	The new finalization token. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DColorLookupTableFinalizationTokenPtr create_FinalizeToken() = 0;
	};
}