//
//  Singleton.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef Singleton_h
#define Singleton_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Generic singleton interface.
     *
     * To declare a singleton, the derived class must follow this rules:
     * - inherits from Singleton,
     * - declare base class as friend,
     * - declare constructor as private or protected, to prevent other object
     *   to create it.
     *
     * @note
     * Derived classes must respect the CRTP idiom.
     */
    template < typename Derived >
    class Singleton
    {
        /*! @brief Private copy constructor. */
        Singleton( const Singleton& ) = delete;
        
        /*! @brief Private move constructor. */
        Singleton( Singleton&& ) = delete;
        
    protected:
        
        /*! @brief Private default constructor. */
        Singleton() noexcept = default;
        
        /*! @brief Private destructor. */
        virtual ~Singleton() = default;
        
    public:
        
        /*! @brief Returns the current instance.
         *
         * If not created, it will create a first instance. Notes that default
         * constructor is always used to create the first instance.
         */
        static Derived& Get()
        {
            //! Static instance for this singleton.
            static Derived instance;
            return instance;
        }
    };
}

#endif /* Singleton_h */
