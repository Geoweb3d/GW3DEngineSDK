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
#include "../core/IGW3DClientUserData.h"
#include "../../Geoweb3dCore/GW3DResults.h"

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Vector layer representation. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct  GW3D_DLL IGW3DVectorRepresentation : public IGW3DClientUserData
    {
        virtual ~IGW3DVectorRepresentation() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the animation property object. </summary>
        ///
        /// <remarks>	This is meant for a short term override of a features appearance in the 3D scene,
        /// 			such as for highlighting a selection. </remarks>
        ///
        /// <returns>	the animation property object. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // virtual IGW3DAnimationProperty *get_AnimationProperty() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the driver that loaded this representation. </summary>
        ///
        /// <returns>	The driver. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorRepresentationDriverWPtr get_Driver() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the layer. </summary>
        ///
        /// <remarks>	Gets the vector layer that loaded this representation uses. </remarks>
        ///
        /// <returns>	The vector layer. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DVectorLayerWPtr get_VectorLayer() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enable or disable the vector representation. </summary>
        ///
        /// <remarks>	Enables or disables the representation.  This does not start or stop loading, it
        /// 			just enables/disables the representation from being rendered or analyzed. </remarks>
        ///
        /// <param name="enable">	true to enable, false to disable. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void  put_Enabled(bool enable) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Query if the vector representation is enabled. </summary>
        ///
        /// <returns>	true if enabled, false if disabled. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual bool  get_Enabled() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Force representation to refresh to reflect changes to the data. </summary>
        ///  
        ///	<remarks>	If changes are made to data after the representation is created, or if construction
        ///				properties of a representation are changed, this function forces the 3D scene to
        ///				properly page in all its data with the new changes. </remarks>
        ///
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_Refresh() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="env"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_PropertyUpdate(const GW3DEnvelope& env) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="collection"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void force_PropertyUpdate(const IGW3DIdCollectionPtr& collection) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the default property collection. </summary>
        ///
        /// <remarks>	put_PropertyCollection is used to change the default values of the representation
        /// 			properties from that of the driver defaults. There is minimal error checking in the
        /// 			sdk, as it only validates the property definition matches. If you set the property
        /// 			collection to null, then this representation will go back to using the driver's
        /// 			default values.
        ///
        /// 			Any object/feature that has not been given unique properties will be represented
        /// 			using these default properties. </remarks>
        ///
        /// <param name="p">	The property collection. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	void put_PropertyCollection(const IGW3DPropertyCollection* p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Set the default property collection. </summary>
        ///
        /// <remarks>	put_PropertyCollection is used to change the default values of the representation
        /// 			properties from that of the driver defaults. There is minimal error checking in the
        /// 			sdk, as it only validates the property definition matches. If you set the property
        /// 			collection to null, then this representation will go back to using the driver's
        /// 			default values.
        ///
        /// 			Any object/feature that has not been given unique properties will be represented
        /// 			using these default properties. </remarks>
        ///
        /// <param name="p">	The property collection. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	void put_PropertyCollection(const IGW3DPropertyCollectionPtr p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the default properties for this representation. </summary>
        ///
        /// <remarks>	Gets the default representation properties that will be used for representing all
        /// 			objects/features that do not have specific properties. </remarks>
        ///
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	The property collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DPropertyCollection* get_PropertyCollection(bool reset_next_iterator = false) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the representation properties for the given feature/object id. </summary>
        ///
        /// <remarks>	Gets the properties for this specific feature/object id representation.  Note, this
        /// 			will return null if no properties exist for this feature/object id, or if the
        /// 			feature itself does not exist.
        ///
        /// 			Note:  if using IGW3DVectorLayerStream, do not use this API.  Use the API through
        ///				IGW3DVectorLayerStreamResult, as it will optimize the getting of the properties
        ///				object. </remarks>
        ///
        /// <param name="object_id">	The feature/object id. </param>
        /// <param name="reset_next_iterator"> TODO </param>
        ///
        /// <returns>	the property collection. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual  const IGW3DPropertyCollection* get_PropertyCollection(long object_id, bool reset_next_iterator = false) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the representation properties for the given feature/object id </summary>
        ///
        /// <remarks>	The representation for the given object/feature will be configured according to
        /// 			the given property collection.
        ///
        /// 			If it was using the default properties prior to this call, the feature/object will
        /// 			become unique, using a small amount of memory to store the unique properties.
        /// 			Because of this and because objects/features using the default properties are
        /// 			instanced more efficiently, it is recommended that the most common set of properties
        /// 			across all features of the representation be used as the default properties.  In
        /// 			other words, the unique properties for a feature/object should be an exception to
        /// 			the most common case, when possible.
        ///
   		///             For representations of features that are in editable mode, in order for new properties
    	///             to be applied, properties must be set via layer feature streaming. </remarks>
    	///
        /// <param name="object_id"> 	the object/feature id. </param>
        /// <param name="p">			the properties. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	void put_PropertyCollection(long object_id, const IGW3DPropertyCollection* p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the representation properties for the given feature/object id </summary>
        ///
        /// <remarks>	The representation for the given object/feature will be configured according to
        /// 			the given property collection.
        ///
        /// 			If it was using the default properties prior to this call, the feature/object will
        /// 			become unique, using a small amount of memory to store the unique properties.
        /// 			Because of this and because objects/features using the default properties are
        /// 			instanced more efficiently, it is recommended that the most common set of properties
        /// 			across all features of the representation be used as the default properties.  In
        /// 			other words, the unique properties for a feature/object should be an exception to
        /// 			the most common case, when possible. 
        ///
		///             For representations of features that are in editable mode, in order for new properties
		///             to be applied, properties must be set via layer feature streaming. </remarks>
		///
        /// <param name="object_id"> 	the object/feature id. </param>
        /// <param name="p">			the properties. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual	void put_PropertyCollection(long object_id, const IGW3DPropertyCollectionPtr p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the representation properties for the given container of feature/object id; </summary>
        ///
        /// <remarks>	The representation for the given objects/features will be configured according to
        /// 			the given property collection.
        ///
        /// 			If they were using the default properties prior to this call, the feature/object will
        /// 			become unique, using a small amount of memory to store the unique properties for each.
        /// 			Because of this and because objects/features using the default properties are
        /// 			instanced more efficiently, it is recommended that the most common set of properties
        /// 			across all features of the representation be used as the default properties.  In
        /// 			other words, the unique properties for a feature/object should be an exception to
        /// 			the most common case, when possible.
        ///
		///             For representations of features that are in editable mode, in order for new properties
		///             to be applied, properties must be set via layer feature streaming. </remarks>
		///
        /// <param name="object_id"> 	the object/feature id. </param>
        /// <param name="p">			the properties. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_PropertyCollection(unsigned num_ids, long* object_id_list, const IGW3DPropertyCollection* p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the representation properties for the given container of feature/object id; </summary>
        ///
        /// <remarks>	The representation for the given objects/features will be configured according to
        /// 			the given property collection.
        ///
        /// 			If they were using the default properties prior to this call, the feature/object
        /// 			will become unique, using a small amount of memory to store the unique properties
        /// 			for each.
        /// 			Because of this and because objects/features using the default properties are
        /// 			instanced more efficiently, it is recommended that the most common set of
        /// 			properties across all features of the representation be used as the default
        /// 			properties.  In other words, the unique properties for a feature/object should be
        /// 			an exception to the most common case, when possible.
        ///
		///             For representations of features that are in editable mode, in order for new properties
		///             to be applied, properties must be set via layer feature streaming. </remarks>
		///
        /// <param name="num_ids">	the number of objects/features. </param>
        /// <param name="object_id_list">	list of object/feature ids. </param>
        /// <param name="p">	   	the properties. </param>
        ///
        ///  <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_PropertyCollection(unsigned num_ids, long* object_id_list, const IGW3DPropertyCollectionPtr p) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Removes any unique properties for the given feature/object id. </summary>
        ///
        /// <remarks>	The unique properties for the given feature/object id will be removed and the
        /// 			feature will revert to the representation's default properties. </remarks>
        ///
        /// <param name="object_id">	The object/feature id. </param>
        ///
        ///  <returns> </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void reset_Properties(long object_id) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Removes any unique properties for the given feature/object ids. </summary>
        ///
        /// <remarks>	The unique properties for the given feature/object ids will be removed and the
        /// 			features will revert to the representation's default properties. </remarks>
        ///
        /// <param name="num_ids">		 	the number of objects/features. </param>
        /// <param name="object_id_list">	list of object/feature ids. </param>
        ///
        /// <returns>	 </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void reset_Properties(unsigned num_ids, long* object_id_list) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Removes unique properties for all feature/object ids. </summary>
        ///
        /// <remarks>	The unique properties for all feature/object ids will be removed and all
        /// 			features will revert to the representation's default properties. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void reset_Properties() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the paging level </summary>
        ///
        /// <remarks>	Sets the paging level, which must less than or equal to the maximum terrain tree
        /// 			depth. This can be a very expensive call so its usage should be limited.  It
        /// 			is recommended to set the page level at the time the representation is created.
        ///
        /// 			Note that you may determine the distance in meters that a page level will come in
        /// 			using the Geoweb3d::Coordinates::PageLevelToMeterRadius function. </remarks>
        ///
        /// <param name="page_level">	The page level. </param>
        ///
        /// <returns>	GW3D_Ok if succeeded. </returns>
        ///
        /// <see cref="Geoweb3d::IGW3DInitializationConfiguration"/>
        /// <see cref="Geoweb3d::Coordinates::PageLevelToMeterRadius"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_PagingLevel(unsigned page_level) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the paging level. </summary>
        ///
        /// <returns>	The paging level. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual unsigned get_PagingLevel() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Get the clamp radius filter. </summary>
        ///
        /// <remarks>	The clamp radius filter, when enabled, will only allow rendering of features/objects
        /// 			within a given distance from the eye.  Any features/objects outside the specified
        /// 			range will not be visible.
        ///
        /// 			This is useful for managing performance, forcing certain representations to only be
        /// 			visible at certain distances, or even to set up a level of detail (LOD) system.
        ///
        /// 			The clamp radius is enabled by default and set to a minimum radius of 0 and a
        /// 			maximum radius of 3218.688f meters.
        ///
        /// 			Note that features/objects not visible to the eye due to the clamp radius filter
        /// 			may still show up in a viewshed analysis. </remarks>
        ///
        /// <returns>	the clamp radius filter. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DClampRadius* get_ClampRadiusFilter() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Get the feature id filter. </summary>
        ///
        /// <returns>	the feature id filter. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DFeatureIdFilter* get_FeatureIdFilter() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the camera specific properties of this representation. </summary>
        ///
        /// <remarks>	Some representations have properties that can be configured per camera
        /// 			Once example would be for things like water, where one can control
        /// 			what is used to be part of the reflection.  Example, one may want to remove
        /// 			reps driver adding to reflections for performance reasons. </remarks>
        ///
        /// <returns>	IGW3DPropertyCollectionWPtr, but expired if no properties exist </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DPropertyCollection* get_CameraProperties(const IGW3DCameraWPtr& camera) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the camera specific properties of this representation. </summary>
        ///
        /// <param name="camera">	 	The camera. </param>
        /// <param name="properties">	The properties. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_CameraProperties(const IGW3DCameraWPtr& camera, const IGW3DPropertyCollectionWPtr& properties) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the frustum analysis contents configuration </summary>
        ///
        /// <remarks>	The frustum analysis contents configuration controls what gets streamed for this
        /// 			representation when this representation is processed by a
        /// 			IGW3DFrustumAnalysisStream.  You can stream the results off the
        /// 			representation driver collection.</remarks>
        ///
        /// <returns>	the camera frustum intersection configuration. </returns>
        ///
        /// <see cref="IGW3DFrustumAnalysisStream"/>
        /// <see cref="IGW3DVectorRepresentationDriverCollection::Stream"/>
        ///	<see cref="IGW3DFeatureID2DPixelLocationConfiguration::put_Enabled"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual IGW3DFrameAnalysisContentsConfiguration* get_FrameAnalysisContentsConfiguration() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Gets the frustum analysis contents configuration </summary>
        ///
        /// <remarks>	The frustum analysis contents configuration controls what gets streamed for this
        /// 			representation when this representation is processed by a
        /// 			IGW3DFrustumAnalysisStream.  You can stream the results off the
        /// 			representation driver collection.</remarks>
        ///
        /// <returns>	the camera frustum intersection configuration. </returns>
        ///
        /// <see cref="IGW3DFrustumAnalysisStream"/>
        /// <see cref="IGW3DVectorRepresentationDriverCollection::Stream"/>
        ///	<see cref="IGW3DFeatureID2DPixelLocationConfiguration::put_Enabled"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual const IGW3DFrameAnalysisContentsConfiguration* get_FrameAnalysisContentsConfiguration() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Sets the image palette to be used in this representation. </summary>
        ///
        /// <remarks>	The images defined in the token will be used as a palette, where the properties that
        /// 			use it will reference the palette images using the index of the image within the
        /// 			image collection that created it.  Note that changes to the image collection after
        /// 			this finalization token was generated do not affect the representation.  Any
        /// 			existing palette will be overwritten.  Undefined results if you have  feature/object
        /// 			ids mapped outside of a palette's maximum index. </remarks>
        ///
        /// <param name="images">	The image finalization token. </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ///
        /// <see cref="IGW3DImageCollection::create_FinalizeToken"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult put_GW3DFinalizationToken(IGW3DTokenPtr images) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Blocks until the timeout expires or all properties are downloaded into the 3d scene
        ///				</summary>
        ///
        /// <remarks>	***** Implemented but NOT A TESTED API****
        ///				Setting properties are an async operation.  If you need to block, so you can
        /// 			ensure a visual change happened within the 3d scene (if in the field of view),
        /// 			you can call this function which will not return until the scene update queue has
		///				been drained. </remarks>
        ///
        /// <param name="timout_milliseconds">	timeout in milliseconds  </param>
        ///
        /// <returns>	GW3D_sOk if succeeded. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DResult flush_AsyncPropertyCollectionSettings(int timout_milliseconds = -1 /*-1 is infinite*/) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Applies a mask to filter which cameras will render this representation. </summary>
        ///
        /// <remarks>	Changes the set of bits that represent the cameras that will render this
        ///				representation if none of its draw mask bits intersect with the bits of this
        ///				representation's mask. </remarks>
        ///
        /// <param name="mask"> TODO </param>
        ///
        ///	<see cref="IGW3DCamera::put_CameraMask"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void put_CullMask(GW3DCullMask mask) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Returns the set of bits that represent the subset of cameras that will render this
        ///				representation. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual GW3DCullMask get_CullMask() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> this is the results of ALL(mixed) line segments if anything intersected, only added to help clients
		/// that do not want to expose the line intersection test itself, but want to provide 
		/// them with the results of all the intersections.  This is not the most efficient way to get results, the other way is to use the
        /// line intersection collection api to get results.  Keep in mind if you go through the line intersection collection
        /// to read results, you can also know the line intersection that caused the intersection. </summary>
        ///
        /// <param name="any_enabled"> TODO </param>
        /// <param name="any_valid"> TODO </param>
        ///
        /// <returns> TODO </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual Geoweb3d::IGW3DLineSegmentIntersectionDetailCollection* get_LineSegmentIntersectionDetailCollection(bool* any_enabled, bool* any_valid) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the font to be used in this representation.</summary>
		/// 
		/// <remarks>   All labels that are assigned via the properties will be rendered using this font.
		///             Note: Currently only supported by Billboards. </remarks>
		///
		/// <param name="font">	The Font. </param>
		/// 
		/// <returns>	GW3D_sOk if succeeded. </returns>
		///
		///	<see cref="IGW3DFont::create"/>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_Font( Geoweb3d::IGW3DFontPtr font ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the label specific properties of this representation. </summary>
		///
		/// <remarks>	Label properties are applied to each representation.
		///				Note: Currently only supported by Billboards. </remarks>
		///
		/// <returns>	IGW3DPropertyCollectionWPtr, but expired if no properties exist </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual const IGW3DPropertyCollection * get_LabelProperties() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the Label specific properties of this representation. </summary>
		///
		/// <param name="properties">	The properties. </param>
		///
		/// <returns>	GW3D_sOk if succeeded. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual GW3DResult put_LabelProperties( const IGW3DPropertyCollectionPtr &properties ) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets whether the rep is set to auto include itself in analytics. </summary>
		///
		/// <remarks>    Only hooked up for Terrain Avoidance. </remarks>
		/// 
		/// <returns>	The result of auto inclusion.  </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual bool get_AutoAnalyticInclusion() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enables the ability to automatically include this rep in analytics. </summary>
		///
		/// <remarks>    Only hooked up for Terrain Avoidance. </remarks>
		/// 
		/// <param name="enable">	Whether to auto include. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_AutoAnalyticInclusion(const bool enable) = 0;


    };
}