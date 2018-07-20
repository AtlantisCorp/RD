//
//  NotificationAnswer.cpp
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#include "NotificationAnswer.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    bool NotificationAnswer::shouldAbort() const
    {
        return shouldAbort_;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    bool NotificationAnswer::shouldDo() const
    {
        return shouldDo_;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    int NotificationAnswer::value() const
    {
        return value_;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    NotificationAnswer NotificationAnswer::ShouldAbort(bool v)
    {
        NotificationAnswer answer;
        answer.shouldAbort_ = v;
        return answer;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    NotificationAnswer NotificationAnswer::ShouldDo(bool v)
    {
        NotificationAnswer answer;
        answer.shouldDo_ = v;
        return answer;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    NotificationAnswer NotificationAnswer::Value(int v)
    {
        NotificationAnswer answer;
        answer.value_ = v;
        return answer;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    NotificationAnswer NotificationAnswer::Null()
    {
        NotificationAnswer answer;
        answer.shouldAbort_ = false;
        answer.shouldDo_ = false;
        answer.value_ = 0;
        return answer;
    }
}
