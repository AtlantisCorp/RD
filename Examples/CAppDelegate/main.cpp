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
public:
    
    ~CAppDelegate() = default;
    
    void onApplicationDidUpdate( RD::Application& application, const RD::Clock::time_point& )
    {
        std::cout << "Called." << std::endl;
        application.stop();
    }
};

class CModule : public RD::Module
{
public:
    
    bool start( RD::Application& application, const RD::Clock::time_point& ticks ) { return true; }
    bool update( RD::Application& application, const RD::Clock::time_point& ticks ) { return true; }
    bool terminate( RD::Application& application, const RD::Clock::time_point& ticks ) { return true; }
    
    void* loadHash( size_t hash, void* user )
    {
        if ( hash == typeid(RD::ApplicationDelegate).hash_code() )
            return (void*) RD::CreateHandlePtr < RD::ApplicationDelegate, CAppDelegate >();
        return nullptr;
    }
};

int main(int argc, const char * argv[])
{
    try
    {
        RD::Application& application = RD::Application::Get();
        
        auto cmodule = RD::CreateHandle < CModule >();
        application.addModule( cmodule );
        
        auto appdelegate = cmodule->loadClass < RD::ApplicationDelegate >();
        application.setDelegate( appdelegate );
        
        char buffer[1024] = {0};
        getcwd(buffer, 1024);
        
        std::cout << "CWD = " << buffer << std::endl;
        
        auto gl3module = application.loadModule("../lib/Modules/libGl3Module.dylib");
        auto gl3driver = gl3module->loadClass < RD::Driver >();
        
        application.run();
        
        auto b = RD::CreateHandle<CAppDelegate>();
        std::cout << "Leaks: " << RD::Details::AllocationTracker::GetTotalLeaksSize() << std::endl;
        
        /*
         
         #include <Gl3Module/Gl3Driver.h>
         
         Gl3::Gl3DriverConfig config;
         config.version.major = 3;
         config.version.minor = 3;
         config.sampleBuffers = 1;
         config.samples = 4;
         config.colors.red = 8;
         config.colors.blue = 8;
         config.ws = Gl3::WindowSystem::OSX;
         
         #include <Gl3Module/OSX/OSXSurface.h>
         
         config.parent = myview;
         config.parentType = Gl3::OSX::ParentType::View;
         config.sharedContext = myCGLContext;
         
         #include <Gl3Module/XCB/XCBSurface.h>
         
         config.parent = xcb_window_t;
         config.sharedContext = myGLXContext;
         
         auto driver = cmodule->loadClass < RD::Driver >( &config );
         
         */
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
