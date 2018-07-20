/**
 * @file Gl3Module/Gl3Module.cpp
 * @author Luk2010
 * @date 15/07/2018
 *
 * @brief
 * Creates the RD::Module for Gl3Module extension.
 */

#include "Gl3Module.h"
#include "Gl3Driver.h"

#if defined(Gl3HaveCocoa)
#include "OSX/OSXModule.h"

namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    bool Module::start(RD::Application &application, const RD::Clock::time_point &ticks)
    {
        bool result = Gl3OSXStartApplication(this);
        
        if (result)
            emit < RD::ModuleListener >(&RD::ModuleListener::onModuleDidStart, this);
        
        return result;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    bool Module::update(RD::Application& application, const RD::Clock::time_point& ticks)
    {
        return Gl3OSXUpdateApplication(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    bool Module::terminate(RD::Application &application, const RD::Clock::time_point &ticks)
    {
        delegate = nullptr;
        emit < RD::ModuleListener >(&RD::ModuleListener::onModuleWillTerminate, this);
        
        // NOTE [Design]
        // To resolve a concurrency problem when emitting 'onModuleWillTerminate' (see Gl3Driver),
        // Gl3::Module must clear all its listeners after emitting this event.
        clearListeners();
        
        return true;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string Module::name() const
    {
        return "Gl3Module:OSX";
    }
    
#elif defined(Gl3HaveWindows)
#include "Windows/WINModule.h"
    
namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    bool Module::start(RD::Application &application, const RD::Clock::time_point &ticks)
    {
        bool started = Gl3WINStartApplication(application, ticks);
        hInstance = Gl3WINGetHInstance();
        return started;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string Module::name() const
    {
        return "Gl3Module:WIN";
    }
    
#elif defined(Gl3HaveXCB)
#include "XCB/XCBModule.h"

namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    bool Module::start(RD::Application &application, const RD::Clock::time_point &ticks)
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string Module::name() const
    {
        return "Gl3Module:XCB";
    }
    
#elif defined(Gl3HaveX11)
#include "X11/X11Module.h"
    
namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    bool Module::start(RD::Application &application, const RD::Clock::time_point &ticks)
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string Module::name() const
    {
        return "Gl3Module:X11";
    }
    
#elif defined(Gl3HaveWayland)
#include "Wayland/WaylandModule.h"
    
namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    bool Module::start(RD::Application &application, const RD::Clock::time_point &ticks)
    {
            
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string Module::name() const
    {
        return "Gl3Module:Wayland";
    }
    
#else
#error "Implementation not available on your platform. You can still implement it yourself!"

#endif
    
    /////////////////////////////////////////////////////////////////////////////////
    void* Module::loadHash(size_t hash, void* user)
    {
        if (hash == typeid(RD::Driver).hash_code())
            return (void*) RD::CreateHandlePtr < RD::Driver, Gl3Driver >(this, (Gl3DriverConfig*)user);
        return nullptr;
    }
}

/////////////////////////////////////////////////////////////////////////////////
extern "C" RD::Handle < RD::Module > CreateModule( void )
{
    return RD::CreateHandle < Gl3::Module >();
}
