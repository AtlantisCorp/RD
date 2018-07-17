//
//  Gl3Module.h
//  RD
//
//  Created by Jacques Tronconi on 15/07/2018.
//

#ifndef Gl3Module_h
#define Gl3Module_h

#include "Gl3Includes.h"
#include <RD/Module.h>

namespace Gl3
{
    /**
     * @brief Gl3Module module class.
     *
     * Installs the Gl3Module interface into the current RD::Application object.
     * Gl3Module is able to create a RD::Driver that will operate with the current
     * OpenGL driver.
     */
    class Module : public RD::Module
    {
#       if defined(Gl3HaveCocoa)
        //! @brief Gl3ApplicationDelegate registered into NSApplication to handle some specific
        //! notifications, like applicationDidHide or applicationDidChangeScreenParameters.
        Gl3Id delegate;
        
        //! @brief Declares OSX startup function as a friend to let it initialize the module's
        //! private members.
        friend bool Gl3OSXStartApplication(Module* module);
        friend bool Gl3OSXUpdateApplication(Module* module);
        
#       endif
        
    public:
        
        /*! @brief Default constructor. */
        Module() noexcept = default;
        
        /*! @brief Default destructor. */
        ~Module() noexcept = default;
        
        /*! @brief Starts the module.
         *
         * @Platform OSX
         * Currently it does assert a NSApplication object is currently available.
         * If not, it will create its own instance of NSApplication and register a
         * custom delegate to receive NSApplication events.
         * If an instance of NSApplication is already available, then a custom delegate
         * is added but NSApplication::updateWindows is never called.
         *
         * @param[in] application Application that launched this module.
         * @param[in] ticks Clock::time_point when this function is called.
         *
         * @return true on success, false otherwise.
         */
        bool start(RD::Application& application, const RD::Clock::time_point& ticks);
        
        /*! @brief Updates the module.
         *
         * @Platform OSX
         * Currently it assumes NSApplication is not running at the same time RD::Application
         * is running too. This function calls NSApplication::updateWindows, and poll for the
         * next event in the main event queue.
         *
         * @param[in] application Application that launched this module.
         * @param[in] ticks Clock::time_point when this function is called.
         *
         * @return true on success, false otherwise.
         */
        bool update(RD::Application& application, const RD::Clock::time_point& ticks);
        
        /*! @brief Stops the module.
         *
         * @Platform OSX
         * Basically it does nothing, except destroying the custom NSApplication delegate that was
         * registered. NSApp is destroyed by the ARC at the end of the main program. Notices that
         * NSApplication::terminate() is never called.
         *
         * @param[in] application Application that launched this module.
         * @param[in] ticks Clock::time_point when this function is called.
         *
         * @note
         * Objects registered by this module (listeners like Gl3Driver) will receive ModuleWillTerminate
         * notification. Objects which receive this notification should destroy everything they might use
         * from this module (in this case, OpenGL resources). 
         *
         * @return true on success, false otherwise.
         */
        bool terminate(RD::Application& application, const RD::Clock::time_point& ticks);
        
        /*! @brief Returns this module name ('Gl3Module'). */
        const std::string name() const;
        
        /*! @brief Returns a new instance of a given class hash, if supported.
         *
         * Currently, supported hashes are:
         *   - RD::Driver: Will create an instance of Gl3Driver.
         *
         * @param hash Hash of the class we want to instanciate.
         * @param user A custom initialization parameter that can only be Gl3DriverConfig*
         *  for now, as Gl3Driver is the only supported class right now.
         *
         * @return A pointer to an handle to the referred class, or nullptr if this hash is
         *  not supported by the module.
         */
        void* loadHash(size_t hash, void* user);
    };
}

#endif /* Gl3Module_h */
