//
//  main.cpp
//  test
//
//  Created by Jacques Tronconi on 09/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#include <RD/Application.h>
#include <RD/Driver.h>

#include <unistd.h>

class CAppDelegate : public RD::ApplicationDelegate
{
    //! @brief Handle to our driver.
    RD::Handle < RD::Driver > driver;
    
public:
    
    ~CAppDelegate() = default;
    
    void onApplicationDidStart(RD::Application& application, const RD::Clock::time_point& ticks)
    {
        auto module = application.findModule("Gl3Module");
        
        if (module.valid())
        {
            RD::DriverConfiguration config;
            config.multisampling = true;
            config.sampleBuffers = 1;
            config.samples = 4;
            config.bpp = 32;
            config.buffers = 2;
            
            driver = module->loadClass < RD::Driver >(&config);
            std::cout << "Driver name: " << driver->name() << std::endl;
            std::cout << "Driver version: " << driver->version() << std::endl;
            
            if (!driver->valid())
                return;
            
            auto surface = driver->createSurface(1024, 768, "CAppDelegate window", "Window1");
        }
    }
    
    void onApplicationDidUpdate(RD::Application& application, const RD::Clock::time_point&)
    {
        if (!driver.valid() || !driver->valid())
            application.stop();
        
        if (driver->getSurfacesCount() == 0)
            application.stop();
    }
    
    void onApplicationWillTerminate(RD::Application& application, const RD::Clock::time_point&)
    {
        driver.reset();
    }
};

int main(int argc, const char * argv[])
{
    try
    {
        {
            RD::Application& application = RD::Application::Get();
            
            auto appdelegate = RD::CreateHandle < CAppDelegate >();
            application.setDelegate( appdelegate );
            
            char buffer[1024] = {0};
            getcwd(buffer, 1024);
            
            std::cout << "CWD = " << buffer << std::endl;
            
            auto gl3module = application.loadModule("../lib/Modules/libGl3Module.dylib");
            assert(gl3module.valid());
            
            application.run();
        }
        
        std::cout << "Allocated: " << RD::Details::AllocationTracker::GetTotalLeaksSize() << std::endl;
        RD::Application::Destroy();
    }
    
    catch ( RD::Exception const& e )
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return e.code();
    }
    
    size_t leaks = RD::Details::AllocationTracker::GetTotalLeaksSize();
    std::cout << "Leaks: " << leaks << std::endl;
    
    if (leaks)
    {
        auto leakeds = RD::Details::AllocationTracker::GetLeakedAllocations();
        
        for (auto leaked : leakeds)
        {
            std::cout << "[0X" << leaked.p << "]<" << leaked.type << ">: " << leaked.size << " byte(s)" << std::endl;
        }
    }
    
    return 0;
}
