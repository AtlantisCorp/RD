//
//  Gl3Surface.h
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#ifndef Gl3Surface_h
#define Gl3Surface_h

#include "Gl3Includes.h"
#include <RD/Surface.h>

namespace Gl3
{
    /**
     * @brief Interface to interact with the window subsystem.
     *
     * ### Platform OSX
     * OSX defines interface Gl3OSXView, which is always created by this class. Gl3OSXSurfaceConfiguration
     * lets user define a parent class, which can be a NSView or a NSWindow. If no parent is specified, a NSWindow
     * is created and Gl3OSXView is set as the content view of this window. The window is owned by this object
     * and \ref close also close the window.
     * If a parent is specified, \ref close only destroys the Gl3OSXView, which is neither the NSView nor the
     * NSWindow parent.
     *
     * @note
     * Keys events are received by this view only if the window has focus and is the key window.
     *
     * ### Platform X11
     * Not implemented yet.
     *
     * ### Platform XCB
     * Not implemented yet.
     *
     * ### Platform Wayland
     * Not implemented yet.
     *
     * ### Platform Windows
     * Not implemented yet.
     *
     * @note
     * This is a work in progress, please be kind.
     */
    class Gl3Surface : public RD::Surface
    {
#       ifdef Gl3HaveCocoa
        //! @brief Our derived from NSView.
        Gl3ID glView;
        
        //! @brief Our parent window, if we have one (a NSView always have a parent window, isn't it ?).
        Gl3ID glWindow;
        
#       endif
        
    public:
        
        /*! @brief Default constructor.
         *
         * @param[in] driver Driver which constructed this object.
         * @param[in] width Width of the surface in pixels.
         * @param[in] height Height of the surface in pixels.
         * @param[in] title Title for this Surface, if available.
         * @param[in] objectName Name for this surface.
         * @param[in] style Style used to create the surface, applicable if it is a Window.
         * @param[in] extension A pointer to a specific Gl3*SurfaceConfiguration structure.
         */
        Gl3Surface(RD::Driver* driver, uint32_t width, uint32_t height, const std::string& title, const std::string& objectName,
                   uint32_t style = RD::SurfaceStyle::Default,
                   const void* extension = nullptr);
        
        /*! @brief Default destructor. */
        ~Gl3Surface() = default;
    };
}

#endif /* Gl3Surface_h */
