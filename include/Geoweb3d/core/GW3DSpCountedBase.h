//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

# pragma once
#include "Geoweb3dCore/Geoweb3dExports.h"
#include "GW3DSpTypeinfo.h"

//for the interlocks.

#if _MSC_VER < 1900 && _MSC_VER
#include <windows.h> //for the interlocks.
#else
#include <atomic>
#endif

extern "C" GW3D_DLL void* GW3DNew( size_t size);
extern "C" GW3D_DLL void GW3DDelete( void* p );

namespace Geoweb3d
{
class GW3DSpCountedBase
{
private:
    GW3DSpCountedBase( GW3DSpCountedBase const & );
    GW3DSpCountedBase & operator= ( GW3DSpCountedBase const & );
	
#if _MSC_VER < 1900 && _MSC_VER
	long atomic_use_count_;        // #shared
	long atomic_weak_count_;       // #weak + (#shared != 0)
#else
	std::atomic<long> atomic_use_count_;
	std::atomic<long> atomic_weak_count_;
#endif

public:

    GW3DSpCountedBase(): atomic_use_count_( 1 ), atomic_weak_count_( 1 ) {}

    virtual ~GW3DSpCountedBase() // nothrow
    {
    }

    // dispose() is called when use_count_ drops to zero, to release
    // the resources managed by *this.
    virtual void dispose() = 0; // nothrow

    // destroy() is called when gw3d_weak_count_ drops to zero.
    virtual void destroy() // nothrow
    {
        delete this;
    }

    void operator delete( void * p )
    {
        GW3DDelete( p);// std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

    virtual void * get_deleter( detail::GW3DSpTypeinfo const & ti ) = 0;
    virtual void * get_untyped_deleter() = 0;

    void add_ref_copy()
    {
		#if _MSC_VER < 1900 && _MSC_VER
				_InterlockedIncrement(&atomic_use_count_);
		#else
		++atomic_use_count_;
		#endif

		
    }

    bool add_ref_lock() // true on success
    {
        for( ;; )
        {

			#if _MSC_VER < 1900 && _MSC_VER
				long tmp = static_cast<long const volatile&>(atomic_use_count_);
				if (tmp == 0) return false;
						
				if (_InterlockedCompareExchange(&atomic_use_count_, tmp + 1, tmp) == tmp) 
					return true;
			#else

				long tmp = atomic_use_count_.load();
				if (tmp == 0) return false;
			
				if (atomic_use_count_.compare_exchange_weak(tmp, tmp + 1))
				return true;

			#endif

        }
    }

    void release() // nothrow
    {

		#if _MSC_VER < 1900 && _MSC_VER
				if (_InterlockedDecrement(&atomic_use_count_) == 0)
				{
					dispose();
					weak_release();
				}
		#else
				if (--atomic_use_count_ == 0)
				{
					dispose();
					weak_release();
				}
		#endif
    }

    void weak_add_ref() // nothrow
    {

		#if _MSC_VER < 1900 && _MSC_VER
				_InterlockedIncrement(&atomic_weak_count_);
		#else
				++atomic_weak_count_;
		#endif
    }

    void weak_release() // nothrow
    {

		#if _MSC_VER < 1900 && _MSC_VER
				if (_InterlockedDecrement(&atomic_weak_count_) == 0)
				{
					destroy();
				}
		#else
		if (--atomic_weak_count_ == 0)
		{
			destroy();
		}
		#endif


    }

    long use_count() const // nothrow
    {

		#if _MSC_VER < 1900 && _MSC_VER
				return static_cast<long const volatile &>(atomic_use_count_);
		#else
				return atomic_use_count_;
		#endif
    }
};
}