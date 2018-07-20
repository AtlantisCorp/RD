//
//  NotificationObserver.cpp
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#include "NotificationObserver.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    NotificationAnswer NotificationObserver::observe(const NotificationCenter*, const Notification&)
    {
        return NotificationAnswer::Null();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    bool NotificationObserver::shouldObserve(const NotificationCenter*, const Notification&) const
    {
        return false;
    }
}
