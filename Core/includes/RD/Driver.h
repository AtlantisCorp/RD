//
//  Driver.h
//  RD
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#ifndef Driver_h
#define Driver_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Generic interface for a Graphic API wrapper.
     *
     * A Driver is a wrapper for a given Graphic API (like OpenGL, DirectX, or others). It
     * has many functions like handling Windows (or Surfaces) creation, handling buffers swaps,
     * and creation of most of the objects related to the graphic pipeline.
     *
     * A Driver has one or more CommandQueues. Those queues can be commited to the graphic pipeline
     * whenever you want, and the commands commited will be executed after a call to 'present()'.
     *
     */
    class Driver
    {
    public:
        
        /*! @brief Default constructor. */
        Driver() noexcept = default;
        
        /*! @brief Default destructor. */
        virtual ~Driver() noexcept = default;
        
        /*! @brief Returns the driver's name. */
        virtual const std::string name() const noexcept = 0;
    };
}

#endif /* Driver_h */
