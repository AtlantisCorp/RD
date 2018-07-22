//
//  Module.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef Module_h
#define Module_h

#include "Handle.h"
#include "Emitter.h"

namespace RD
{
    class Application;
    class Module;
    
    /**
     * @brief Generic module listener interface.
     */
    class ModuleListener
    {
    public:
        
        /*! @brief Default constructor. */
        ModuleListener() noexcept = default;
        
        /*! @brief Default destructor. */
        virtual ~ModuleListener() noexcept = default;
        
        /*! @brief Called when module is started. */
        virtual void onModuleDidStart(Module* module) {}
        
        /*! @brief Called before module is terminated. */
        virtual void onModuleWillTerminate(Module* module) {}
        
        /*! @brief Called right before the module update itself. */
        virtual void onModuleWillUpdate(Module*) {}
        
        /*! @brief Called right after the module updated itself. */
        virtual void onModuleDidUpdate(Module*) {}
    };
    
    /**
     * @brief Generic module design.
     *
     * A module is a piece of code that can be loaded externally.
     * User must define the pure virtual functions start(), update() and
     * terminate(). Other virtual override are recommanded but not needed
     * to load it.
     *
     * A module is managed by the Application instance. When registering a module,
     * it is started only if the Application is already started.
     *
     * When a module implements some implementable class, as Driver, user can
     * create this class by using 'loadClass < Driver >'. Choice is made by using
     * the result of 'typeid(Driver).hash_code()'.
     */
    class Module : public Emitter < ModuleListener >
    {
    public:
        
        /*! @brief Default constructor. */
        Module() noexcept = default;
        
        /*! @brief Default destructor. */
        virtual ~Module() noexcept = default;
        
        /*! @brief Starts the module.
         *
         * @param[in] application Application that launched this module.
         * @param[in] ticks Clock::time_point when this function is called.
         *
         * @return A boolean indicating wether the module is started or not.
         */
        virtual bool start( Application& application, const Clock::time_point& ticks ) = 0;
        
        /*! @brief Updates the module.
         *
         * Called by Application at each loop of run(). It can be used by the user
         * to update its Module, like updating every Windows for a Window subsystem.
         *
         * @param[in] application Application that launched this module.
         * @param[in] ticks Clock::time_point when this function is called.
         *
         * @return A boolean indicating wether the module has been updated or not.
         */
        virtual bool update( Application& application, const Clock::time_point& ticks ) = 0;
        
        /*! @brief Terminates the module.
         *
         * Called by Application before it terminates. It can be used to destroy every resources
         * related to this module, like windows in a window subsystem.
         *
         * @param[in] application Application that launched this module.
         * @param[in] ticks Clock::time_point when this function is called.
         *
         * @return A boolean indicating wether the module has been terminated or not.
         */
        virtual bool terminate( Application& application, const Clock::time_point& ticks ) = 0;
        
        /*! @brief Returns the module name.
         *
         * Module's name is made of two parts, separated by a ':' :
         * - Main module name, which is how a user can found the module in the Application.
         * - Specification module name, which is a custom second part that can specify some
         *   implementations depending on the platform the module was compiled for.
         *
         * @return Complete module name, including main and specification names separated
         * by a ':' character.
         */
        virtual const std::string name() const { return std::string(); }
        
        /*! @brief Loads a class that might be implemented by this module.
         *
         * @tparam Class Class to load.
         * @param user Custom parameter that can pass any custom configuration for the
         *  derived module.
         *
         * @return A Handle to the newly created class, or an invalid Handle if the Class
         *      could not be created. When a module is not able to create a Class, invalid
         *      handle is also returned.
         */
        template < typename Class >
        Handle < Class > loadClass( void* user = nullptr )
        {
            const size_t hash = typeid(Class).hash_code();
            void* ptr = loadHash( hash, user );
            if ( !ptr ) return Handle < Class >();
            
            Handle < Class >* hdl = reinterpret_cast < Handle < Class >* >( ptr );
            Handle < Class > hdl2( *hdl );
            delete hdl;
            
            return hdl2;
        }
        
        /*! @brief Loads a class from its Hash code.
         *
         * @param hash Hash to identify the class.
         * @param user Custom parameter.
         *
         * @return A generic pointer to a Handle structure (formerly a cast from Handle*
         *      to void*). void* is used because we can't now the final type of this function.
         */
        virtual void* loadHash( size_t hash, void* user ) { return nullptr; }
    };
}

#endif /* Module_h */
