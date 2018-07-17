//
//  OSXModule.h
//  RD
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#ifndef OSXModule_h
#define OSXModule_h

#ifndef Gl3HaveCocoa
#   error "OSX/OSXModule.h included but Gl3HaveCocoa not defined."
#endif

#ifdef __OBJC__
#   include <Cocoa/Cocoa.h>
#   include <OpenGL/OpenGL.h>
#
#else
#   include <OpenGL/OpenGL.h>
#   define id void*
#
#endif

namespace Gl3
{
    class Module;
    
    /*! @brief Starts the Module.
     *
     * Asserts NSApplication is valid. If not, it creates a NSApplication that only launches but stop
     * when it has finished launching. Events are handled by our own event loop, so user don't have to
     * update the main NSApplication object.
     *
     * @note
     * If user wants to use its own NSApplication object or event loop, it must compile this module with
     * the definition 'Gl3OSXNoEventLoop'. In this case, 'Module::update()' will not call NSApplication's
     * updateWindows function, nor poll for next event. It will just do nothing.
     *
     * @return True if NSApp could be initialized.
     */
    bool Gl3OSXStartApplication(Module* module);
    
    /*! @brief Updates the Module.
     *
     * Currently, if a delegate is present in the module and Gl3OSXNoEventLoop is not defined, it calls
     * 'updateWindows' from NSApplication and poll for the next event in the main event loop.
     *
     * @return True if no error occured.
     */
    bool Gl3OSXUpdateApplication(Module* module);
}

#endif /* OSXModule_h */
