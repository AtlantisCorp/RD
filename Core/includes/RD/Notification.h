//
//  Notification.h
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#ifndef Notification_h
#define Notification_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Defines a Notification structure.
     */
    class Notification
    {
        //! @brief Module that sends this notification.
        std::string moduleStr;
        
        //! @brief Function that sends this notification.
        std::string functionStr;
        
        //! @brief Unique notification identifier (name).
        std::string notification;
        
        //! @brief Message associated.
        std::string messageStr;
        
    public:
        
        /*! @brief Default constructor. */
        Notification() noexcept = default;
        
        /*! @brief Extended constructor. */
        Notification(const char* moduleName,
                     const char* functionName,
                     const char* notifName,
                     const char* message);
        
        /*! @brief Default destructor. */
        ~Notification() noexcept = default;
        
        /*! @brief Returns the notification's module. */
        const std::string& module() const;
        
        /*! @brief Returns the notification's function. */
        const std::string& function() const;
        
        /*! @brief Returns the notification's name. */
        const std::string& name() const;
        
        /*! @brief Returns the notification's message. */
        const std::string& message() const;
    };
    
    //! @brief Notification that can be sent when a NotificationObserver has request a module abort from
    //! another notification.
    extern const char* kNotificationAbortRequested;
}

#endif /* Notification_h */
