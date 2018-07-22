//
//  Gl3OSXPFAttribs.cpp
//  Gl3Module
//
//  Created by Jacques Tronconi on 21/07/2018.
//

#include "OSX/Gl3OSXPFAttribs.h"
#include "Gl3Driver.h"

namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    std::vector < CGLPixelFormatAttribute > Gl3OSXPixelFormatAttribs(const RD::DriverConfiguration* configuration)
    {
        std::vector < CGLPixelFormatAttribute > result;
        result.push_back(kCGLPFAAccelerated);
        result.push_back(kCGLPFAMinimumPolicy);
        
        if (configuration->extension)
        {
            auto ext = (Gl3DriverConfiguration*)configuration->extension;
            
            if (ext->major == 3)
            {
                result.push_back(kCGLPFAOpenGLProfile);
                result.push_back((CGLPixelFormatAttribute)kCGLOGLPVersion_GL3_Core);
            }
            
            else if (ext->major == 4)
            {
                result.push_back(kCGLPFAOpenGLProfile);
                result.push_back((CGLPixelFormatAttribute)kCGLOGLPVersion_GL4_Core);
            }
        }
        else
        {
            result.push_back(kCGLPFAOpenGLProfile);
            result.push_back((CGLPixelFormatAttribute)kCGLOGLPVersion_GL3_Core);
        }
        
        if (configuration->multisampling)
        {
            result.push_back(kCGLPFAMultisample);
            result.push_back(kCGLPFASampleBuffers);
            result.push_back((CGLPixelFormatAttribute)configuration->sampleBuffers);
            result.push_back(kCGLPFASamples);
            result.push_back((CGLPixelFormatAttribute)configuration->samples);
        }
        
        if (configuration->buffers == 2)
            result.push_back(kCGLPFADoubleBuffer);
        else if(configuration->buffers == 3)
            result.push_back(kCGLPFATripleBuffer);
        
        result.push_back((CGLPixelFormatAttribute)0);
        return result;
    }
}
