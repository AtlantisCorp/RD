//
//  Surface.cpp
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#include "Surface.h"
#include "NotificationCenter.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    Surface::Surface(Driver* driver, uint32_t width, uint32_t height, const std::string& title, const std::string& objectName, uint32_t style, const void* extension) : DriverResource(driver)
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Surface::~Surface()
    {
        // When destroying the surface object, we must assert the underlying surface is closed. If not, this
        // might means the application is terminating but Surface::close() has not been called, which might
        // be strange because Driver calls close() when receiving onModuleWillTerminate().
        // However, calling pure virtual methods in destructor is really a bad idea. So you, who reads this
        // useless paragraph, think about closing your surface when deriving this class.
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Surface::onDriverClear()
    {
        close();
    }
}
