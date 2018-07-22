//
//  NotificationCenter.cpp
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#include "NotificationCenter.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    std::forward_list < NotificationAnswer > NotificationCenter::notifiate(const Notification& notification)
    {
        std::lock_guard < std::mutex > lock(mutex);
        answers.clear();
        
        for (auto observer : observers)
        {
            if (observer.valid())
            {
                if (observer->shouldObserve(this, notification))
                {
                    answers.push_front(observer->observe(this, notification));
                }
            }
        }
        
        
        return answers;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::forward_list < NotificationAnswer > NotificationCenter::collectedAnswers() const
    {
        std::lock_guard < std::mutex > lock(mutex);
        return answers;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void NotificationCenter::addObserver(const Handle<NotificationObserver>& observer)
    {
        if (observer.valid())
        {
            std::lock_guard < std::mutex > lock(mutex);
            observers.push_front(observer);
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void NotificationCenter::removeObserver(const Handle<NotificationObserver>& observer)
    {
        if (observer.valid())
        {
            std::lock_guard < std::mutex > lock(mutex);
            observers.remove(observer);
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void NotificationCenter::clearObservers()
    {
        std::lock_guard < std::mutex > lock(mutex);
        observers.clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::forward_list < NotificationAnswer > NotificationCenter::Notifiate(const Notification& notification)
    {
        if (defaultCenter.valid())
            return defaultCenter->notifiate(notification);
        else
            return std::forward_list < NotificationAnswer >();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::forward_list < NotificationAnswer > NotificationCenter::Notifiate(const char *moduleName,
                                                                           const char *functionName,
                                                                           const char *notificationName,
                                                                           const char *format, ...)
    {
        if (defaultCenter.valid())
        {
            char buffer[2048];
            memset(buffer, 0, 2048);
            
            va_list args;
            va_start(args, format);
            vsnprintf(buffer, 2048, format, args);
            va_end(args);
            
            Notification notification(moduleName, functionName, notificationName, buffer);
            return defaultCenter->notifiate(notification);
        }
        
        else
        {
            return std::forward_list < NotificationAnswer >();
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::forward_list < NotificationAnswer > NotificationCenter::CollectedAnswers()
    {
        if (defaultCenter.valid())
            return defaultCenter->collectedAnswers();
        else
            return std::forward_list < NotificationAnswer >();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Handle < NotificationCenter > NotificationCenter::defaultCenter;
    
    /////////////////////////////////////////////////////////////////////////////////
    std::forward_list < NotificationAnswer > NotifiateAbort(const char* moduleName,
                                                            const char *functionName,
                                                            const char *notificationName,
                                                            const char *format, ...)
    {
        char buffer[2048];
        memset(buffer, 0, 2048);
        
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, 2048, format, args);
        va_end(args);
        
        Notification notification(moduleName, functionName, notificationName, buffer);
        auto answers = NotificationCenter::Notifiate(notification);
        
        auto it = std::find_if(answers.begin(), answers.end(), [](const RD::NotificationAnswer& answer){
            if (answer.shouldAbort())
                return true;
            return false;
        });
        
        if (it != answers.end())
        {
            RD::NotificationCenter::Notifiate(moduleName,
                                              functionName,
                                              RD::kNotificationAbortRequested,
                                              buffer);
            
            throw RD::AbortRequestedException(moduleName,
                                              functionName,
                                              buffer);
        }
        
        return answers;
    }
}
