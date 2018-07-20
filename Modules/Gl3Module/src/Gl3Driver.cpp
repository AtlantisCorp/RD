//
//  Gl3Driver.cpp
//  Gl3Module
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#include "Gl3Driver.h"

namespace Gl3
{
    /////////////////////////////////////////////////////////////////////////////////
    Gl3InvalidModuleException::Gl3InvalidModuleException() 
        : RD::Exception(ErrorCode, "Gl3: Invalid module (differs from created).")
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Gl3Driver::Gl3Driver(RD::Module* mod, Gl3DriverConfig* config) : module(mod)
    {
        if (!mod) {
            throw Gl3InvalidModuleException();
        }
        
        mod->addListener((RD::ModuleListener*)this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Gl3Driver::~Gl3Driver()
    {
        RD::Module* mod = module.load();
        
        if (mod) {
            mod->removeListener((RD::ModuleListener*)this);
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string Gl3Driver::name() const noexcept
    {
        return "Gl3Driver";
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Gl3Driver::onModuleWillTerminate(RD::Module *mod)
    {
        if (mod != module.load()) {
            throw Gl3InvalidModuleException();
        }
        
        // NOTE [Concurrency]
        // When RD::Module terminates, it locks its listener list to emit onModuleWillTerminate.
        // However, 'mod->removeListener' locks also the listener list. To resolve this, we can
        // copy the listener list to iterate over the copy and remove the listeners. Or, we can
        // say that when a module terminates, it will always clear its listeners. Regarding
        // performance, copying the listener's list is slower than clearing all listeners after
        // emitting 'onModuleWillTerminate'.
        // mod->removeListener((RD::ModuleListener*)this);
        
        module.store(nullptr);
    }
}
