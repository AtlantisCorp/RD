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
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Application::~Application()
    {
        
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
    bool Application::loadModule(const std::string &libname, bool required)
    {
        //! @brief Our function to look for in the library.
        typedef Module* (*CreateModuleFcn) ( void );
        
        void* handle = dlopen( libname.data(), RTLD_LAZY );
        bool result = (handle != nullptr);
        
        CreateModuleFcn fcn = (CreateModuleFcn) dlsym( handle, "CreateModule" );
        result = result && (fcn != nullptr);
        
        Handle < Module > module( fcn() );
        result = result && module.valid();
        
        if ( !result && required )
            throw ModuleNotLoadedException( libname );
        else
            addModule( module );
        
        return result;
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
