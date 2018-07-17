//
//  Allocator.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef Allocator_h
#define Allocator_h

#include "Global.h"

namespace RD
{
    namespace Details
    {
        /*! @brief Generic allocation infos. */
        struct Allocation
        {
            uintptr_t p;
            std::string type;
            size_t size;
            size_t n;
        };
        
        //! @brief Deallocation info.
        typedef Allocation Deallocation;
        
        /**
         * @brief Tracks every allocations.
         *
         * User can access it to have a snapshot of the current memory used by its program,
         * or it can even track leaked memory or memory not yet released.
         */
        class AllocationTracker
        {
            static std::map < uintptr_t, Allocation > _allocations;
            static std::map < uintptr_t, Deallocation > _deallocations;
            static std::mutex _mutex;
            
        public:
            
            static void PushAllocation( uintptr_t p, const Allocation& allocation );
            static void PushDeallocation( uintptr_t p, const Deallocation& deallocation );
            
            static std::list < Allocation > GetLeakedAllocations();
            static size_t GetTotalLeaksSize();
        };
        
        extern std::atomic < std::allocator < void >::const_pointer > AllocationLastHint;
    }
    
    /**
     * @brief Generic Allocator to allocate memory tracked by Details::AllocationTracker.
     *
     * This allocator is used to keep trace of memory allocated and deallocated. It
     * register allocations and deallocations directly to MemoryTracker, a singleton
     * available to manager memory currently available.
     *
     * @tparam Class Object to allocate.
     */
    template < class Class >
    class Allocator : public std::allocator < Class >
    {
    public:
        
        typedef typename std::allocator<Class>::pointer   pointer;
        typedef typename std::allocator<Class>::size_type size_type;
        
        template <typename U>
        struct rebind { typedef Allocator<U> other; };
        
        Allocator() throw() {}
        Allocator(const Allocator& other) throw() {}
        
        template <typename U>
        Allocator(const Allocator<U>& other) throw() {}
        
        ~Allocator() {}
        
        template <typename U>
        Allocator& operator = (const Allocator<U>& other) { return *this; }
        Allocator<Class>& operator = (const Allocator& other) { return *this; }
        
        template <typename U> bool operator == (const Allocator<U>& other) { return true; }
        template <typename U> bool operator != (const Allocator<U>& other) { return !(*this == other); }
        
        /*! @brief Allocates a block of memory and registers it to Details::AllocationTracker Singleton.
         *
         * @param[in] n Number of elments to allocate.
         * @param[in] hint Hint to allocate the memory.
         *
         * @return A pointer to the memory newly allocated.
         */
        pointer allocate( size_type n, std::allocator < void >::const_pointer hint = Details::AllocationLastHint.load() )
        {
            pointer p = std::allocator<Class>::allocate( n, hint );
            Details::AllocationLastHint.store( p );
            
            Details::Allocation allocation;
            allocation.p = (uintptr_t)p;
            allocation.type = typeid(Class).name();
            allocation.size = sizeof(Class) * n;
            allocation.n = n;
            Details::AllocationTracker::PushAllocation( (uintptr_t)p, allocation );
            
            return p;
        }
        
        /*! @brief Deallocates a block of memory and registers this operation into Details::AllocationTracker
         * Singleton.
         *
         * @param[in] p Pointer to deallocate.
         * @param[in] n Number of elements in the pointer, if it is an array, or 1.
         */
        void deallocate( pointer p, size_type n )
        {
            std::allocator<Class>::deallocate( p, n );
            
            Details::Deallocation deallocation;
            deallocation.p = (uintptr_t)p;
            deallocation.type = typeid(Class).name();
            deallocation.size = sizeof(Class) * n;
            deallocation.n = n;
            Details::AllocationTracker::PushDeallocation( (uintptr_t)p, deallocation );
        }
    };
}

#endif /* Allocator_h */
