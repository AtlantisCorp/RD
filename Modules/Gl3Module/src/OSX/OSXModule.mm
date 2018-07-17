//
//  OSXModule.m
//  Gl3Module
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#include "Gl3Module.h"
#include "OSX/OSXModule.h"

/////////////////////////////////////////////////////////////////////////////////
static void SendEmptyEvent( void )
{
    @autoreleasepool
    {
        NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                            location:NSMakePoint(0, 0)
                                       modifierFlags:0
                                           timestamp:0
                                        windowNumber:0
                                             context:nil
                                             subtype:0
                                               data1:0
                                               data2:0];
        [NSApp postEvent:event atStart:YES];
    }
}

/**
 * @brief Application delegate to handle NSApplication notifications.
 *
 * Basically it serve to stop the NSApplication from running its main event loop. This is because our module
 * already updates all windows and handle events. User can disable this feature and provide its own NSApp
 * object by compiling with definition Gl3OSXNoEventLoop.
 */
@interface Gl3ApplicationDelegate : NSObject < NSApplicationDelegate >
@property(atomic) Gl3::Module* module;

@end

@implementation Gl3ApplicationDelegate
@synthesize module;

/////////////////////////////////////////////////////////////////////////////////
- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
    [NSApp stop:nil];
    SendEmptyEvent();
}

/////////////////////////////////////////////////////////////////////////////////
- (void) applicationDidChangeScreenParameters:(NSNotification*)notification
{
    /*
     
     Gl3::ScreenPixelFormat newFormat = Gl3::OSXGetCurrentScreenPixelFormat();
     module->onScreenPixelFormatChange(newFormat);
     
     {
        for each listener in listeners
            -> listener.onScreenPixelFormatChange(newFormat);
     }
     
     */
}

@end

namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    bool Gl3OSXStartApplication(Module* module)
    {
        if ( NSApp )
            return true;
        
#ifndef Gl3OSXNoEventLoop
        NSApp = [NSApplication sharedApplication];
        
        Gl3ApplicationDelegate* delegate = [[Gl3ApplicationDelegate alloc] init];
        [delegate setModule:module];
        [NSApp setDelegate:delegate];
        [NSApp run];
        
        module->delegate = delegate;
        return true;
        
#else
        return false;
        
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    bool Gl3OSXUpdateApplication(Module* module)
    {
        if ( !NSApp )
            return false;
        
#ifndef Gl3OSXNoEventLoop
        
        if ( !module->delegate )
            return false;
        
        @autoreleasepool
        {
            NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:nil
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
            
            if (event)
            {
                [NSApp sendEvent:event];
            }
            
            [NSApp updateWindows];
        }
        
        return true;
        
#else
        return true;
        
#endif
    }
}
