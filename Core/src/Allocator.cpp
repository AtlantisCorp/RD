//
//  Allocator.cpp
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#include "Allocator.h"

namespace RD
{
    namespace Details
    {
        std::map < uintptr_t, Allocation > AllocationTracker::_allocations;
        std::map < uintptr_t, Deallocation > AllocationTracker::_deallocations;
        std::mutex AllocationTracker::_mutex;
        
        void AllocationTracker::PushAllocation( uintptr_t p, const Allocation& allocation )
        {
            std::lock_guard < std::mutex > lock( _mutex );
            _allocations.insert( std::make_pair(p, allocation) );
        }
        
        void AllocationTracker::PushDeallocation( uintptr_t p, const Deallocation& deallocation )
        {
            std::lock_guard < std::mutex > lock( _mutex );
            _deallocations.insert( std::make_pair(p, deallocation) );
        }
        
        std::list < Allocation > AllocationTracker::GetLeakedAllocations()
        {
            std::lock_guard < std::mutex > lock( _mutex );
            std::list < Allocation > results;
            
            for ( auto pair : _allocations )
            {
                auto it = _deallocations.find( pair.first );
                
                if ( it == _deallocations.end() )
                    results.push_back( pair.second );
            }
            
            return results;
        }
        
        size_t AllocationTracker::GetTotalLeaksSize()
        {
            std::list < Allocation > leaks = GetLeakedAllocations();
            size_t result = 0;
            
            for ( Allocation& alloc : leaks )
                result += alloc.size;
            
            return result;
        }
        
        std::atomic < std::allocator < void >::const_pointer > AllocationLastHint( 0 );
    }
}
