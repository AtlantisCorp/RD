//
//  ScreenPosition.h
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#ifndef ScreenPosition_h
#define ScreenPosition_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Defines a position relative to a screen.
     */
    struct ScreenPosition
    {
        //! @brief Position along the X axis.
        uint32_t x = 0;
        
        //! @brief Position along the Y axis.
        uint32_t y = 0;
    };
}

#endif /* ScreenPosition_h */
