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
        
        mod->removeListener((RD::ModuleListener*)this);
        module.store(nullptr);
    }
}
