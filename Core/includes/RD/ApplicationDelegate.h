//
//  ApplicationDelegate.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef ApplicationDelegate_h
#define ApplicationDelegate_h

#include "Global.h"

namespace RD
{
    class Application;
    
    /**
     * @brief Base class for Application's delegate.
     */
    class ApplicationDelegate
    {
    public:
        
        /*! @brief Default constructor. */
        ApplicationDelegate() = default;
        
        /*! @brief Default destructor. */
        virtual ~ApplicationDelegate() = default;
        
        /*! @brief Called before application starts.
         * @param[in] application Application that called this function.
         * @param[in] now Time point of when this function is called.
         */
        virtual void onApplicationWillStart( Application& application, const Clock::time_point& now ) { }
        
        /*! @brief Called after the application started.
         * @param[in] application Application that called this function.
         * @param[in] now Time point of when this function is called.
         */
        virtual void onApplicationDidStart( Application& application, const Clock::time_point& now ) { }
        
        /*! @brief Called before application update.
         * @param[in] application Application that called this function.
         * @param[in] now Time point of when this function is called.
         */
        virtual void onApplicationWillUpdate( Application& application, const Clock::time_point& now ) { }
        
        /*! @brief Called after application updated.
         * @param[in] application Application that called this function.
         * @param[in] now Time point of when this function is called.
         */
        virtual void onApplicationDidUpdate( Application& application, const Clock::time_point& now ) { }
        
        /*! @brief Called before application terminates.
         * @param[in] application Application that called this function.
         * @param[in] now Time point of when this function is called.
         */
        virtual void onApplicationWillTerminate( Application& application, const Clock::time_point& now ) { }
        
        /*! @brief Called when application terminate.
         *
         * A delegate can prevent its application from terminating by deriving this
         * function and returning, on particular case, a false boolean. It can be used
         * to let other processes terminating before ending the application.
         *
         * @param[in] application Application that called this function.
         *
         * @return true if application can terminate, false otherwise.
         */
        virtual bool onApplicationShouldTerminate( Application& application ) { return true; }
    };
}

#endif /* ApplicationDelegate_h */
