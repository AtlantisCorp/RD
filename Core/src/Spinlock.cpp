//
//  Spinlock.cpp
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#include "Spinlock.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    void Spinlock::lock()
    {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Spinlock::unlock()
    {
        locked.clear(std::memory_order_release);
    }
}
