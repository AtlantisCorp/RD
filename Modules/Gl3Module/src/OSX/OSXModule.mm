//
//  OSXModule.m
//  Gl3Module
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#include "Gl3Module.h"
#include "OSX/OSXModule.h"

#include <RD/NotificationCenter.h>
#include <RD/Exception.h>

//! @brief Constant to describe Gl3NotificationNibMenuNotFound.
static const char* kGl3NotificationNibMenuNotFound = "Gl3NotificationNibMenuNotFound";

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
        
        // In case we are unbundled, makes a proper UI activation.
        
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        // Menu bar setup must go between sharedApplication above and
        // finishLaunching below, in order to properly emulate the behavior
        // of NSApplicationMain
        
        if ( [[NSBundle mainBundle] pathForResource:@"MainMenu" ofType:@"nib" ] )
        {
            [NSApp loadMainMenu];
        }
        else
        {
            auto answers = RD::NotificationCenter::Notifiate("Gl3Module",
                                "Gl3OSXStartApplication",
                                kGl3NotificationNibMenuNotFound,
                                "'MainMenu.nib' resource not found. One can use this file to create a"
                                " custom menu for its application.");
            
            for (auto answer : answers)
            {
                if (answer.shouldAbort())
                {
                    RD::NotificationCenter::Notifiate("Gl3Module",
                                                      "Gl3OSXStartApplication",
                                                      RD::kNotificationAbortRequested,
                                                      "Abort has been requested by an observer of NotificationCenter.");
                    
                    throw RD::AbortRequestedException("Gl3Module",
                                                      "Gl3OSXStartApplication",
                                                      "Abort has been requested by an observer of NotificationCenter.");
                }
            }
        }
        
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
