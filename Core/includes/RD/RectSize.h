//
//  RectSize.h
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#ifndef RectSize_h
#define RectSize_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Defines size of a Rect.
     */
    struct RectSize
    {
        //! @brief Width in pixels.
        uint32_t width = 0;
        
        //! @brief Height in pixels.
        uint32_t height = 0;
    };
}

#endif /* RectSize_h */
