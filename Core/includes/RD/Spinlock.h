//
//  Spinlock.h
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#ifndef Spinlock_h
#define Spinlock_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Spinlock implementation.
     *
     * Directly took from https://stackoverflow.com/questions/26583433/c11-implementation-of-spinlock-using-atomic .
     */
    class Spinlock
    {
        //! @brief Atomic flag.
        std::atomic_flag locked = ATOMIC_FLAG_INIT ;
        
    public:
        
        //! @brief Locks the spinlock.
        void lock();
        
        //! @brief Unlocks the spinlock.
        void unlock();
    };
}

#endif /* Spinlock_h */
