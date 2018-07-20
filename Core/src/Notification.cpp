//
//  Notification.cpp
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#include "Notification.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    Notification::Notification(const char* moduleName, const char* functionName, const char* notifName, const char* message)
    : moduleStr(moduleName), functionStr(functionName), notification(notifName), messageStr(message)
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string& Notification::module() const
    {
        return moduleStr;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string& Notification::function() const
    {
        return functionStr;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string& Notification::name() const
    {
        return notification;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const std::string& Notification::message() const
    {
        return messageStr;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const char* kNotificationAbortRequested = "RDNotificationAbortRequested";
}
