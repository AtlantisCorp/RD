//
//  ThreadedTasks.h
//  RD
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#ifndef ThreadedTasks_h
#define ThreadedTasks_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Groups threaded tasks into different threads.
     *
     * Helper to group many tasks and spawn one thread for each of them. Before destruction,
     * it tries to join all thread, ensuring they have finished their task.
     */
    class ThreadedTasks
    {
        //! @brief Groups of threads.
        std::queue < std::thread > threads;
        
    public:
        
        /*! @brief Default constructor. */
        ThreadedTasks() noexcept = default;
        
        /*! @brief Destroys the tasks. */
        ~ThreadedTasks();
        
        /*! @brief Pushes a new task. */
        virtual void push(std::thread&& thread);
        
        /*! @brief Joins all threads. */
        virtual void join();
    };
}

#endif /* ThreadedTasks_h */
