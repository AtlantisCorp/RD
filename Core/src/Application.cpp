//
//  Application.cpp
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#include "Application.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    Application::Application()
    {
        shouldTerminate.store( false );
        
        defaultCenter = CreateHandle < NotificationCenter >();
        NotificationCenter::defaultCenter = defaultCenter;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Application::~Application()
    {
        NotificationCenter::defaultCenter.reset();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Application::setDelegate(const Handle<RD::ApplicationDelegate> &handle)
    {
        delegate = handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Application::start()
    {
        if ( flag.test_and_set() )
            return;
        
        if ( delegate.valid() )
            delegate->onApplicationWillStart( *this, Clock::now() );
        
        /* Start every modules already registered. */
        
        {
            std::lock_guard < std::mutex > lock( modulesMutex );
            
            for ( auto& module : modules )
            {
                if ( module.valid() )
                    module->start( *this, Clock::now() );
            }
        }
        
        /* Do here platform initialization. */
        
        if ( delegate.valid() )
            delegate->onApplicationDidStart( *this, Clock::now() );
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Application::run()
    {
        start();
        
        while ( !shouldTerminate )
        {
            if ( delegate.valid() )
                delegate->onApplicationWillUpdate( *this, Clock::now() );
            
            /* Updates every registered modules. */
            
            {
                std::lock_guard < std::mutex > lock( modulesMutex );
                
                for ( auto& module : modules )
                {
                    if ( module.valid() )
                        module->update( *this, Clock::now() );
                }
            }
            
            /* Do here platform updates. */
            
            if ( delegate.valid() )
                delegate->onApplicationDidUpdate( *this, Clock::now() );
        }
        
        if ( delegate.valid() )
            shouldTerminate.store( delegate->onApplicationShouldTerminate( *this ) );
        
        if ( !shouldTerminate )
            run();
        
        else
            terminate();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Application::stop()
    {
        shouldTerminate.store( true );
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Application::addModule(const Handle<RD::Module> &module)
    {
        std::lock_guard < std::mutex > lock( modulesMutex );
        auto it = std::find( modules.begin(), modules.end(), module );
        if ( it != modules.end() ) throw HandleNotUniqueException( (uintptr_t) module.ptr() );
        
        modules.push_front( module );
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Handle < Module > Application::loadModule(const std::string &libname, bool required)
    {
        //! @brief Our function to look for in the library.
        typedef Handle < Module > (*CreateModuleFcn) ( void );
        
        void* handle = dlopen( libname.data(), RTLD_LAZY );
        bool result = (handle != nullptr);
        
        if ( !result && required )
            throw ModuleNotLoadedException( libname );
        
        CreateModuleFcn fcn = (CreateModuleFcn) dlsym( handle, "CreateModule" );
        result = result && (fcn != nullptr);
        
        if ( !result && required ) {
            dlclose(handle);
            throw ModuleNotLoadedException( libname );
        }
        if ( !result ) {
            dlclose(handle);
            return Handle < Module >();
        }
        
        Handle < Module > module = fcn();
        result = result && module.valid();
        
        if ( !result && required ) {
            dlclose(handle);
            throw ModuleNotLoadedException( libname );
        }
        else
            addModule( module );
        
        return module;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Handle < NotificationCenter > Application::getNotificationCenter()
    {
        std::lock_guard < Spinlock > lock(defaultCenterSpinlock);
        return defaultCenter;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Handle < Module > Application::findModule(const std::string &name)
    {
        std::lock_guard < std::mutex > lock(modulesMutex);
        
        auto it = std::find_if(modules.begin(), modules.end(), [name](const Handle < Module >& module) {
            if (!module.valid())
                return false;
            
            std::string completeName = module->name();
            std::string moduleName = completeName.substr(0,
                                        completeName.find_first_of(":"));
            
            if (moduleName == name)
                return true;
            else
                return false;
        });
        
        if (it == modules.end())
            return Handle < Module >();
        else
            return *it;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Application::terminate()
    {
        if ( delegate.valid() )
            delegate->onApplicationWillTerminate( *this, Clock::now() );
        
        /* Terminates every registered modules. */
        
        {
            std::lock_guard < std::mutex > lock( modulesMutex );
            
            for ( auto& module : modules )
            {
                if ( module.valid() )
                    module->terminate( *this, Clock::now() );
            }
            
            modules.clear();
        }
        
        /* Do here application's terminating features. */
        
        delegate.reset();
    }
}
