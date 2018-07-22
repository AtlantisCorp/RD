//
//  Gl3OSXPFAttribs.h
//  RD
//
//  Created by Jacques Tronconi on 21/07/2018.
//

#ifndef Gl3OSXPFAttribs_h
#define Gl3OSXPFAttribs_h

#ifndef Gl3HaveCocoa
#   error "File 'OSX/Gl3OSXPFAttribs.h' cannot be included on not OSX platforms."
#endif

#include "Gl3Includes.h"

namespace RD
{
    struct DriverConfiguration;
}

namespace Gl3
{    
    /*! @brief Converts the Gl3DriverConfiguration structure to a pointer to CGLPixelFormatAttribute.
     *
     * OSX defines some attributes we use to compute our custom attributes. Currently, flags translated are:
     * - multisampling: Converts to kCGLPFAMultisampling.
     * - sampleBuffers: Converts to kCGLPFASampleBuffers.
     * - samples: Converts to kCGLPFASamples.
     * - bpp: Ignored, because OSX assumes it is always 32bpp.
     * - buffers: kCGLPFADoubleBuffer if value is 2, kCGLPFATripleBuffer if value is 3, ignored otherwise.
     *
     * @return A pointer to a valid set of attributes or NULL if a valid set of attributes cannot be
     * translated from the given structure.
     */
    std::vector < CGLPixelFormatAttribute > Gl3OSXPixelFormatAttribs(const RD::DriverConfiguration* configuration);
}

#endif /* Gl3OSXPFAttribs_h */
