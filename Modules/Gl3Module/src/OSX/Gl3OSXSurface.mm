//
//  Gl3OSXSurface.m
//  Gl3Module
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#include "Gl3Surface.h"
#include <RD/NotificationCenter.h>

@interface Gl3OSXView : NSView
{
    NSOpenGLContext context_;
    Gl3::Gl3Surface* surface_;
    NSTrackingArea* trackArea;
    
}

- (id) initWithWindow:(NSWindow*)window surface:(Gl3::Gl3Surface*)surface context:(CGLContextObj)context;
- (id) initWithView:(NSView*)view surface:(Gl3::Gl3Surface*)surface context:(CGLContextObj)context;

@end

@interface Gl3OSXWindowDelegate : NSObject < NSWindowDelegate >
{
    Gl3OSXView* responder;
}
@end

@implementation Gl3OSXView

- (id) initWithWindow:(NSWindow *)window surface:(Gl3::Gl3Surface *)surface context:(CGLContextObj)context
{
    assert(window && "Null NSWindow given.");
    assert(surface && "Null Gl3Surface given.");
    assert(context && "Null CGLContextObj given.");
    
    if (self = [super initWithFrame:[window frame]])
    {
        context_ = [[NSOpenGLContext alloc] initWithCGLContextObj:context];
        surface_ = surface;
        trackArea = nil;
        
        
    }
}

@end

namespace Gl3
{
    struct Gl3OSXSurfaceConfiguration
    {
        Gl3ID parent;
    };
    
    /////////////////////////////////////////////////////////////////////////////////
    Gl3Surface::Gl3Surface(RD::Driver* driver, uint32_t width, uint32_t height, const std::string& title, const std::string& objectName,
                           uint32_t style, const void* extension) : RD::Surface(driver, width, height, title, objectName, style, extension)
    {
        Gl3OSXSurfaceConfiguration* configuration = static_cast < Gl3OSXSurfaceConfiguration const* >(extension);
        CGLContextObj currentContext = CGLGetCurrentContext();
        glWindow = nil;
        glView = nil;
        
        if (!currentContext)
        {
            RD::NotifiateAbort("Gl3Module", "Gl3Surface::Gl3Surface", "Gl3InvalidContextNotification",
                               "Null OpenGL context to create surface %s.", objectName.data());
            return;
        }
        
        if (configuration && configuration->parent)
        {
            Gl3ID parent = configuration->parent;
            
            if ([parent isKindOfClass:NSWindow])
            {
                glWindow = parent;
                glView = [[Gl3OSXView alloc] initWithWindow:(NSWindow*)parent
                                                    surface:this
                                                    context:currentContext];
            }
            
            else if ([parent isKindOfClass:NSView])
            {
                NSView* view = (NSView*)parent;
                glWindow = [view window];
                glView = [[Gl3OSXView alloc] initWithView:view
                                                  surface:this
                                                  context:currentContext];
            }
        }
        
        if (!glView)
        {
            NSRect content = NSMakeRect(0, 0, (float)width, (float)height);
            NSWindowStyleMask mask = Gl3OSXWindowStyleMask(style);
            
            glWindow = [[NSWindow alloc] initWithContentRect:content
                                                   styleMask:mask
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
            
            glView = [[Gl3OSXView alloc] initWithWindow:glWindow
                                                surface:this
                                                context:currentContext];
        }
    }
}
