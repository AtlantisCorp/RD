//
//  ThreadedTasks.cpp
//  RD
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#include "ThreadedTasks.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    ThreadedTasks::~ThreadedTasks()
    {
        join();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void ThreadedTasks::push(std::thread &&thread)
    {
        threads.push(std::move(thread));
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void ThreadedTasks::join()
    {
        while(!threads.empty())
        {
            threads.front().join();
            threads.pop();
        }
    }
}
