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
#include "Handle.h"

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
         *
         * @note
         * Function must be used in main thread only. Flag used to call_once is consumed.
         */
        static Derived& Get()
        {
            std::call_once(flag, [](Handle < Derived >& instance){
                instance = CreateHandle < Derived >();
            }, instance);
            
            return *instance;
        }
        
        /*! @brief Returns true if an instance is present. */
        static bool Valid()
        {
            return instance.valid();
        }
        
        /*! @brief Destroys the current instance.
         *
         * @note
         * Flag used to call once construct the instance is not reset, and thus the instance
         * will never be reachable again.
         */
        static void Destroy()
        {
            instance.reset();
        }
        
    private:
        
        //! @brief A Handle to atomically access the shared instance.
        //! When getting an instance from multiple threads simultaneously, std::shared_ptr control block
        //! is thread-safe for reading the shared pointer. However, this instance must be created and
        //! into main thread.
        static Handle < Derived > instance;
        
        //! @brief Flag to call once to create the Singleton instance.
        static std::once_flag flag;
    };
    
    /////////////////////////////////////////////////////////////////////////////////
    template < typename Derived > Handle < Derived > Singleton < Derived >::instance;
    template < typename Derived > std::once_flag Singleton < Derived >::flag;
}

#endif /* Singleton_h */
