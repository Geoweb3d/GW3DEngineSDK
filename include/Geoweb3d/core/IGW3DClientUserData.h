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
#include <Geoweb3dCore/Geoweb3dExports.h>

/* Primary namespace */
namespace Geoweb3d
{
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Client User Data base class. </summary>
	///
	/// <remarks>	An interface inheriting from IGW3DClientUserData allows a developer to store
	/// 			their own pointer in the object. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	class GW3D_DLL IGW3DClientUserData
	{
	public:
		IGW3DClientUserData() : userdata_(0) {}
		virtual ~IGW3DClientUserData() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the user data pointer. </summary>
		///
		/// <remarks>	Store a pointer, which may be retrieved later using get_UserData.  This is
		/// 			especially useful for cross-referencing with custom data structures. </remarks>
		///
		/// <param name="ptr">	[in] the pointer. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void put_UserData(void* ptr)
		{
			userdata_ = ptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the user data pointer. </summary>
		///
		/// <remarks>	Retrieve the pointer that was stored using put_UserData. </remarks>
		///
		/// <returns>	null if no pointer is stored, else the user data pointer. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void* get_UserData()
		{
			return userdata_;
		}

	private:
		void* userdata_;
	};
}
