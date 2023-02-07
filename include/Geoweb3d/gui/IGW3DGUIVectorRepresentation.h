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
#include "core/GW3DGUIInterFace.h"
#include <Geoweb3dCore/GW3DResults.h>

/* Primary namespace */
namespace Geoweb3d
{
		
namespace GUI
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A vector representation. </summary>
	///
	/// <remarks>	A vector representation is the visual representation of a vector layer within
	/// 			the 3D scene. 
	/// 			
	/// 			A vector representation has a set of default values that are 
	/// 			applied to all features in the representation.  Individual features or groups
	/// 			of features may be modified by creating one or more attribute maps, which are
	/// 			rules that define whether the settings for the given rule should be applied
	/// 			to any given feature.  If multiple rules apply for the same feature, they are
	/// 			evaluated in order from 'top' to 'bottom', starting with the layer defaults
	/// 			(which are stored inside the 'All Features' attribute map).</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorRepresentation 
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the vector layer. </summary>
		///
		/// <remarks>	Gets vector layer that this representation belongs to. </remarks>
		///
		/// <returns>	The vector layer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorLayerWPtr get_VectorLayer() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get driver. </summary>
		///
		/// <remarks>	Gets the representation driver that was used to create this representation. 
		///				</remarks>
		///
		/// <returns>	The representation driver. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationDriverWPtr get_Driver () = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Flash feature. </summary>
		///
		/// <remarks>	Briefly highlights a given feature's representations. </remarks>
		///
		/// <param name="feature_id">	The feature ID. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void flash_Feature (long feature_id) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the root attribute map collection. </summary>
		///
		/// <remarks>	Get the root attribute maps for the representation. </remarks>
		///
		/// <returns>	The root attribute map for the representation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIAttributeMapRootCollection* get_RootAttributeMapCollection( bool reset_next_iterator = false ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assign an image finalization token. </summary>
		///
		/// <remarks>	For representations that accept an image palette for texturing, this function
		/// 			assigns an image finalization token as the palette that is referenced by
		/// 			properties of the representation, such as the side or top texture of an 
		/// 			extruded polygon. An image finalization token is a finalized configuration of 
		/// 			an IGW3DImageCollectionPtr, which is created using the IGW3DSceneGraphContext 
		/// 			interface.</remarks>
		///
		/// <param name="images">	The images. </param>
		/// 
		/// <see cref="IGW3DSceneGraphContext, IGW3DImageCollectionPtr"/>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_GW3DFinalizationToken(  IGW3DFinalizationTokenPtr images ) = 0;

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A basic vector representation. </summary>
	///
	/// <remarks>	A basic vector representation is a low-level, high-performance representation 
	/// 			designed for highly dynamic data.
	/// 			
	/// 			It provides an interface to directly set representation properties of individual 
	/// 			features. It requires less memory and performance overhead, but doesn't support 
	/// 			the attribute mapping framework found in standard representations in Geoweb3d 
	/// 			Desktop. 
	/// 			
	/// 			A basic vector representation can only be created on a layer that has been
	/// 			loaded or created with the addToToc argument set to false.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct IGW3DGUIVectorRepresentationBasic 
	{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the vector layer. </summary>
		///
		/// <remarks>	Gets vector layer that this representation belongs to. </remarks>
		///
		/// <returns>	The vector layer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorLayerWPtr get_VectorLayer() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get driver. </summary>
		///
		/// <remarks>	Gets the representation driver that was used to create this representation. 
		///				</remarks>
		///
		/// <returns>	The representation driver. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIVectorRepresentationDriverWPtr get_Driver () = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Flash feature. </summary>
		///
		/// <remarks>	Briefly highlights the given feature. If this is called before a previous flash on
		/// 			this representation, the previous flash will be canceled.</remarks>
		///
		/// <param name="feature_id">	The feature ID. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void flash_Feature (long feature_id) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set enabled. </summary>
		///
		/// <remarks>	Set whether or not the representation is enabled in the 3D scene.
		/// 			This does not start or stop loading, it just enables/disables being rendered
		///				or analyzed. </remarks>
		///
		/// <param name="enable">	true to enable, false to disable. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void  put_Enabled( bool enable )= 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets enabled state. </summary>
		///
		/// <remarks>	Check whether or not the representation is enabled in the 3D scene. </remarks>
		///
		/// <returns>	true if enabled, false if disabled. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool  get_Enabled( ) const= 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the representation default properties. </summary>
		///
		/// <remarks>	Change the default values of the representation from those of the driver defaults. </remarks>
		///
		/// <param name="properties">	the new representation default properties. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual	GW3DResult put_PropertyCollection( const IGW3DPropertyCollection* properties ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the representation default properties. </summary>
		///
		/// <remarks>	Change the default values of the representation from those of the driver defaults. </remarks>
		///
		/// <param name="properties">	the new representation default properties. </param> 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual	GW3DResult put_PropertyCollection( const IGW3DPropertyCollectionPtr properties ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the representation default properties. </summary>
		///
		/// <remarks>	Gets the default properties for this representation. </remarks>
		///
		/// <returns>	the property collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DPropertyCollection* get_PropertyCollection( bool reset_next_iterator = false ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the property collection for a feature. </summary>
		///
		/// <remarks>	Gets the properties for this specific feature/object id representation. 
		/// 			Note, this will return null if no unique properties exist for this feature,
		/// 			(i.e. it has no properties set and it is using the defaults), or if the feature
		/// 			itself does not exist. </remarks>
		///
		/// <param name="object_id">	the feature/object ID. </param>
		///
		/// <returns>	null if no unique properties exist, else the property collection. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual  const IGW3DPropertyCollection* get_PropertyCollection( long object_id,  bool reset_next_iterator = false  ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the unique properties for a feature. </summary>
		///
		/// <remarks>	Set the properties for a single feature.  If the feature has no unique properties
		/// 			and is using the default properties, this will also make the feature unique.
		/// 			
		/// 			If is best practice to ensure that unique features are the exception to the 
		/// 			defaults, and that the majority of features use the defaults when possible. 
		/// 			Making a feature unique means that it must have its unique property data stored,
		/// 			resulting in a slight increase in required memory and less efficient instancing.</remarks>
		///
		/// <param name="object_id"> 	the feature/object ID. </param>
		/// <param name="parameter2">	the property collection. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual	GW3DResult put_PropertyCollection(long object_id, const IGW3DPropertyCollection* properties ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set the unique properties for a feature. </summary>
		///
		/// <remarks>	Set the properties for a single feature.  If the feature has no unique properties
		/// 			and is using the default properties, this will also make the feature unique.
		/// 			
		/// 			If is best practice to ensure that unique features are the exception to the 
		/// 			defaults, and that the majority of features use the defaults when possible. 
		/// 			Making a feature unique means that it must have its unique property data stored,
		/// 			resulting in a slight increase in required memory and less efficient instancing.</remarks>
		///
		/// <param name="object_id"> 	the feature/object ID. </param>
		/// <param name="parameter2">	the property collection. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual	GW3DResult put_PropertyCollection(long object_id, const IGW3DPropertyCollectionPtr properties ) = 0;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Resets the properties for a feature. </summary>
		///
		/// <remarks>	Resets the properties of a single feature, making it non-unique. The feature will
		/// 			reset to the defaults for the representation.  Any unique properties for this
		/// 			feature will be removed. </remarks>
		///
		/// <param name="object_id">	the feature/object ID. </param>
		///
		/// <returns>	GW3D_Ok if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult reset_Properties( long object_id ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Resets the properties for all features. </summary>
		///
		/// <remarks>	Resets the properties of all features in the layer, making them all non-unique. 
		/// 			All features will be reset to the defaults for the representation.  Any unique
		/// 			properties for will be removed. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void reset_Properties() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets paging level. </summary>
		///
		/// <remarks>	Sets the level of the terrain tree at which data will begin to page in. </remarks>
		///
		/// <param name="page_level">	This has to be in the range of the max terrain depth.  This can
		/// 							be a very expensive call so use it wisely. </param>
		///
		/// <returns>	GW3D_Ok if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_PagingLevel( unsigned page_level ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets paging level. </summary>
		///
		/// <remarks>	Gets the level of the terrain tree at which data will begin to page in. </remarks>
		///
		/// <returns>	The paging level. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual unsigned get_PagingLevel( ) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the clamp radius filter. </summary>
		///
		/// <remarks>	Filter at what minimum and maximum distance features are visible. 
		/// 			
		/// 			This is useful if you want to render different representations at different
		/// 			distance ranges in order to do some sort of LOD system). 
		/// 			
		/// 			Note, this is enabled by default, where the min radius is 0 and max radius is 
		/// 			around 3218.688f (two miles).  Icon representations are defaulted to always be 
		/// 			visible.</remarks>
		///
		/// <returns>	the clamp radius filter. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual IGW3DGUIClampRadius * get_ClampRadiusFilter() = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assign an image finalization token. </summary>
		///
		/// <remarks>	For representations that accept an image palette for texturing, this function
		/// 			assigns an image finalization token as the palette that is referenced by
		/// 			properties of the representation, such as the side or top texture of an 
		/// 			extruded polygon. An image finalization token is a finalized configuration of 
		/// 			an IGW3DImageCollectionPtr, which is created using the IGW3DSceneGraphContext 
		/// 			interface.</remarks>
		///
		/// <param name="images">	The images. </param>
		/// 
		/// <see cref="IGW3DSceneGraphContext, IGW3DImageCollectionPtr"/>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_GW3DFinalizationToken(  IGW3DFinalizationTokenPtr images ) = 0;

	};

}
}