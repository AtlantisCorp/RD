//
//  Gl3Driver.h
//  RD
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#ifndef Gl3Driver_h
#define Gl3Driver_h

#include "Gl3Includes.h"
#include <RD/Driver.h>
#include <RD/Module.h>

namespace Gl3
{
    /**
     * @brief Generic configuration of the Gl3Driver object.
     *
     * You can give a pointer to an instance of this structure to member RD::DriverConfiguration::extension
     * to enable Gl3 module custom configuration.
     */
    struct Gl3DriverConfiguration
    {
        //! @brief OpenGL major version (minimum).
        short major = 3;
        
        //! @brief OpenGL minor version (minimum).
        short minor = 2;
    };
    
    /** @brief Thrown when given Module does not correspond to the driver's module. */
    RDDefineException(Gl3InvalidModuleException, 1 << 1);
    
    /**
     * @brief OpenGL implementation of RD::Driver.
     *
     * An OpenGL 3 driver has one context shared between all objects and OpenGL surfaces. OpenGL command
     * calls are monothreaded due to the single-context architecture of OpenGL. Multithreaded OpenGL requires
     * two different contexts and thus, objects in one context might not be available in the other context.
     *
     * When Gl3::Module is asked to terminate, this object receives onModuleWillTerminate() notification and
     * destroys every objects it has created. Thus, it is assumed all objects created by this driver are owned
     * by itself. To destroy manually a driver owned object, call either DriverResource::destroy() or
     * Driver::destroy(object).
     */
    class Gl3Driver : public RD::Driver
    {
        //! @brief Module that created this driver. It is used to compare with the module given in
        //! onModuleWillTerminate, and also to unregister itself at destruction.
        std::atomic < RD::Module* > module;
        
        //! @brief Detected OpenGL Major version.
        GLint major;
        
        //! @brief Detected OpenGL Minor version.
        GLint minor;
        
#       ifdef Gl3HaveCocoa
        //! @brief OSX CGL Context.
        CGLContextObj glContext;
        
        //! @brief CGL PixelFormat for the context.
        CGLPixelFormatObj glPixelFormat;
        
#       endif
        
    public:
        
        /*! @brief Default constructor. */
        Gl3Driver(RD::Module* mod, RD::DriverConfiguration* config);
        
        /*! @brief Default destructor. */
        ~Gl3Driver();
        
        /*! @brief Returns 'Gl3Driver'. */
        const std::string name() const noexcept;
        
        /*! @brief Called when module is terminating.
         *
         * Destroys every resources owned by this driver and destroys the OpenGL main context.
         * Notes also that OpenGL surfaces are destroyed during the process.
         *
         * @param[in] mod Module that created this driver. If mod does not correspond to module,
         *      exception Gl3InvalidModuleException is thrown.
         */
        void onModuleWillTerminate(RD::Module* mod);
        
        /*! @brief Returns the driver current version. */
        const RD::Version version() const;
        
        /*! @brief Returns true if the driver was successfully initialized. */
        bool valid() const;
    };
}

#endif /* Gl3Driver_h */
