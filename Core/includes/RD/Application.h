//
//  Application.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef Application_h
#define Application_h

#include "Global.h"
#include "Singleton.h"
#include "Handle.h"
#include "ApplicationDelegate.h"
#include "Module.h"

namespace RD
{
    /**
     * @brief Generic Application design interface.
     *
     * An Application is the base class of every RD program. It can do multiple things,
     * like loading modules, handling basic application events (not keyboard events, as they
     * are related to the Window subsystem).
     *
     * An Application life is always the same:
     *  - it begins with start(), which send to delegate onApplicationWillStart() and onApplicationDidStart().
     *  - it continues with run(), while its stop() function is not called. For each tick, it does call
     *    onApplicationWillUpdate() and onApplicationDidUpdate().
     *  - it ends with terminate(), which send onApplicationWillTerminate().
     *
     * When deriving Application, user should call parent functions to send events correctly to the application
     * delegate. Users should use ApplicationDelegate instead of deriving this class.
     */
    class Application : public Singleton < Application >
    {
        friend class Singleton < Application >;
        
        //! @brief Handle to the delegate.
        Handle < ApplicationDelegate > delegate;
        
        //! @brief Flag to set when 'start' is called.
        std::atomic_flag flag;
        
        //! @brief Boolean indicating we want to terminate.
        std::atomic_bool shouldTerminate;
        
        //! @brief List with modules installed.
        std::forward_list < Handle < Module > > modules;
        
        //! @brief Mutex to protect modules from multithread concurrency.
        //! It might happens when registering a module and updating modules.
        std::mutex modulesMutex;
        
        /*! @brief Default constructor. */
        Application();
        
    protected:
        
        /*! @brief Default destructor. */
        virtual ~Application();
        
    public:
        
        /*! @brief Changes the current delegate.
         * @param[in] handle Delegate to use. It may be null, thus invalidating
         *      the current delegate, if one was present.
         */
        void setDelegate( const Handle < ApplicationDelegate >& handle );
        
        /*! @brief Starts the application. */
        virtual void start();
        
        /*! @brief Runs the application.
         *
         * It calls start(), then enters an update loop ended by a call to stop().
         * Then, when the loop ends, it calls terminate() to clear everything.
         *
         * @note
         * If delegate's onApplicationShouldTerminate returns false, terminate() does
         * nothing and the application re-enter its update loop.
         */
        virtual void run();
        
        /*! @brief Tells the application to stop its update loop. */
        virtual void stop();
        
        /*! @brief Registers a new module.
         *
         * If Application is already started, module is started immediatly. If not,
         * module will be started when Application::start() is called.
         *
         * @param[in] module Module to register.
         */
        virtual void addModule( const Handle < Module >& module );
        
        /*! @brief Loads a module from a dynamic library.
         *
         * Dynamic library must have function 'CreateModule' that returns a valid Module
         * pointer (and not a Handle). Module must be not started, not updated nor terminated,
         * but in an initialized state.
         *
         * @param[in] libname Module's library name.
         * @param[in] required Throws an exception if required is true and module could not be
         *      loaded. Useful if you require some module to be loaded.
         *
         * @return A boolean indicating wether the module could be loaded or not.
         */
        virtual bool loadModule( const std::string& libname, bool required = false );
        
    protected:
        
        /*! @brief Terminates the application. */
        virtual void terminate();
    };
}

#endif /* Application_h */
