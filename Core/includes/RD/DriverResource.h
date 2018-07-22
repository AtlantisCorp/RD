//
//  DriverResource.h
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#ifndef DriverResource_h
#define DriverResource_h

#include "Global.h"

namespace RD
{
    class Driver;
    
    /**
     * @brief A Resource created and owned by a Driver.
     *
     * A DriverResource is created and owned by the Driver. It can represent any object that is on the GPU,
     * like buffers, framebuffers, shader objects, etc. or even a surface, as the driver is responsible for
     * its management.
     *
     * @note
     * DriverResource object can be used by several threads at the same time. However, driver can request the
     * object to clear itself (by the intermediary of \ref onDriverClear) while the object is still used by another
     * thread (like a rendering queue). To solve this problem, DriverResource can be 'locked' by anyone who use it,
     * and unlocked when not used anymore. If the resource is locked, \ref Driver can't request a release.
     *
     * @note
     * When using a DriverResource for some critical action, it is recommended to use std::lock_guard < DriverResource >
     * to lock/unlock the resource in a C++ way.
     *
     * @sa DriverResource::lock, DriverResource::unlock
     */
    class DriverResource
    {
        friend class Driver;
        
        //! @brief Pointer to the driver which created this resource.
        Driver* creator;
        
        //! @brief Flag to tell the driver if the resource is currently in use.
        mutable std::atomic < std::size_t > uses;
        
    public:
        
        /*! @brief Default constructor. */
        DriverResource(Driver* driver);
        
        /*! @brief Default destructor. */
        ~DriverResource();
        
        /*! @brief Returns the driver which created this resource. */
        const Driver* driver() const;
        
        /*! @brief Locks the resource for use.
         *
         * 'Locking' a resource is not related to mutex locking or thread safety mechanism. Instead,
         * it represents the locking of a resource that cannot be destroyed, because in use in another
         * thread or processus. Basically, it increments \ref uses by one.
         *
         * @warning
         * \ref lock must always be followed by a \ref unlock call, or the resource will never be destroyed.
         */
        void lock() const;
        
        /*! @brief Unlocks the resource for a use.
         *
         * Decrements \ref uses by one.
         */
        void unlock() const;
        
        /*! @brief Returns \ref uses. */
        std::size_t usesCount() const;
        
        /*! @brief Returns true if \ref uses is superior to zero. */
        bool isUsed() const;
        
    protected:
        
        /*! @brief Called when Driver must clear every resources.
         *
         * Function must be used to clear all cached or used data by this resource. For example, Surface calls
         * \ref Surface::close, which destroys the surface.
         */
        virtual void onDriverClear() = 0;
    };
}

#endif /* DriverResource_h */
