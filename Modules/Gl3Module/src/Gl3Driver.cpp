//
//  Gl3Driver.cpp
//  Gl3Module
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#include "Gl3Driver.h"
#include <RD/NotificationCenter.h>

#ifdef Gl3HaveCocoa
#   include "OSX/Gl3OSXPFAttribs.h"

#endif

namespace Gl3
{
    RDImplementException(Gl3InvalidModuleException, "Gl3: Invalid module (differs from created).")
    
    /////////////////////////////////////////////////////////////////////////////////
    Gl3Driver::Gl3Driver(RD::Module* mod, RD::DriverConfiguration* config) : module(mod)
    {
        if (!mod) {
            throw Gl3InvalidModuleException();
        }
        
        mod->addListener((RD::ModuleListener*)this);
        
#       ifdef Gl3HaveCocoa
        // Creates a CGLContextObj from the DriverConfiguration object. This context will be used in all
        // the OpenGL objects created by this driver. To have a multicontext program, you may use more than
        // one Driver, each for one Context.
        
        CGLError error = kCGLNoError;
        auto attribs = Gl3OSXPixelFormatAttribs(config);
        
        if (attribs.empty())
        {
            RD::NotifiateAbort("Gl3Module",
                               "Gl3Driver::Gl3Driver",
                               "Gl3OSXInvalidPixelFormatAttributesNotification",
                               "Invalid PixelFormat Attributes.");
            
            glPixelFormat = NULL;
            glContext = NULL;
            return;
        }
        
        GLint npix;
        error = CGLChoosePixelFormat(attribs.data(), &glPixelFormat, &npix);
        
        if (error != kCGLNoError)
        {
            RD::NotifiateAbort("Gl3Module",
                               "Gl3Driver::Gl3Driver",
                               "Gl3OSXCGLFailedNotification",
                               "CGLChoosePixelFormat() failed: %s",
                               CGLErrorString(error));
            
            glPixelFormat = NULL;
            glContext = NULL;
            return;
        }
        
        error = CGLCreateContext(glPixelFormat, NULL, &glContext);
        
        if (error != kCGLNoError)
        {
            CGLReleasePixelFormat(glPixelFormat);
            
            RD::NotifiateAbort("Gl3Module",
                               "Gl3Driver::Gl3Driver",
                               "Gl3OSXCGLFailedNotification",
                               "CGLCreateContext() failed: %s",
                               CGLErrorString(error));
            
            glPixelFormat = NULL;
            glContext = NULL;
            return;
        }
        
        RD::NotificationCenter::Notifiate("Gl3Module",
                                          "Gl3Driver::Gl3Driver",
                                          "Gl3ContextCreatedNotification",
                                          "OpenGL Context created: %l", (long) glContext);
        
        CGLContextObj oldContext = CGLGetCurrentContext();
        error = CGLSetCurrentContext(glContext);
        
        if (error != kCGLNoError)
        {
            CGLReleaseContext(glContext);
            CGLReleasePixelFormat(glPixelFormat);
            CGLSetCurrentContext(oldContext);
            
            RD::NotifiateAbort("Gl3Module",
                               "Gl3Driver::Gl3Driver",
                               "Gl3OSXCGLFailedNotification",
                               "CGLSetCurrentContext() failed: %s.",
                               CGLErrorString(error));
            
            glPixelFormat = NULL;
            glContext = NULL;
            return;
        }
        
        std::string glVersion((const char*)glGetString(GL_VERSION));
        auto nums = RD::explode(glVersion, '.');
        major = nums.size() ? (GLint)(nums[0][0] - '0') : 0;
        minor = nums.size() > 1 ? (GLint)(nums[1][0] - '0') : 0;
        
        CGLSetCurrentContext(oldContext);
        
#       endif
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Gl3Driver::~Gl3Driver()
    {
        RD::Module* mod = module.load();
        
        if (mod) {
            mod->removeListener((RD::ModuleListener*)this);
        }
        
#       ifdef Gl3HaveCocoa
        
        if (glContext)
            CGLReleaseContext(glContext);
        if (glPixelFormat)
            CGLReleasePixelFormat(glPixelFormat);
        
#       endif
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string Gl3Driver::name() const noexcept
    {
        return "Gl3Driver";
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Gl3Driver::onModuleWillTerminate(RD::Module *mod)
    {
        RD::Driver::onModuleWillTerminate(mod);
        
        if (mod != module.load()) {
            throw Gl3InvalidModuleException();
        }
        
        // NOTE [Concurrency]
        // When RD::Module terminates, it locks its listener list to emit onModuleWillTerminate.
        // However, 'mod->removeListener' locks also the listener list. To resolve this, we can
        // copy the listener list to iterate over the copy and remove the listeners. Or, we can
        // say that when a module terminates, it will always clear its listeners. Regarding
        // performance, copying the listener's list is slower than clearing all listeners after
        // emitting 'onModuleWillTerminate'.
        // mod->removeListener((RD::ModuleListener*)this);
        
        module.store(nullptr);
        
#       ifdef Gl3HaveCocoa
        
        if (glContext) {
            CGLReleaseContext(glContext);
            glContext = NULL;
        }
        if (glPixelFormat) {
            CGLReleasePixelFormat(glPixelFormat);
            glPixelFormat = NULL;
        }
        
#       endif
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const RD::Version Gl3Driver::version() const
    {
        return { major, minor, 0, 0 };
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    bool Gl3Driver::valid() const
    {
#       ifdef Gl3HaveCocoa
        return (glPixelFormat != NULL)
            && (glContext != NULL);
        
#       else
        return false;
        
#       endif
    }
}
