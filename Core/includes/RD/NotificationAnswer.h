//
//  NotificationAnswer.h
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#ifndef NotificationAnswer_h
#define NotificationAnswer_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Generic answer that can be returned by a NotificationObserver when called
     * by a NotificationCenter.
     */
    class NotificationAnswer
    {
        //! @brief Boolean set to true if the module should abort its execution (and throw an exception).
        bool shouldAbort_;
        
        //! @brief Boolean set to true if the module has requested a boolean value (like do we have to do something or not).
        bool shouldDo_;
        
        //! @brief Integer value if the module requests an integer answer.
        int value_;
        
    public:
        
        /*! @brief Default constructor. */
        NotificationAnswer() noexcept = default;
        
        /*! @brief Default destructor. */
        ~NotificationAnswer() noexcept = default;
        
        /*! @brief Returns shouldAbort. */
        bool shouldAbort() const;
        
        /*! @brief Returns shouldDo. */
        bool shouldDo() const;
        
        /*! @brief Returns integer value. */
        int value() const;
        
    public:
        
        /*! @brief Creates an answer with shouldAbort set to boolean. */
        static NotificationAnswer ShouldAbort(bool v);
        
        /*! @brief Creates an answer with shouldDo set to boolean. */
        static NotificationAnswer ShouldDo(bool v);
        
        /*! @brief Creates an answer with an integer value. */
        static NotificationAnswer Value(int v);
        
        /*! @brief Creates a null NotificationAnswer. */
        static NotificationAnswer Null();
    };
}

#endif /* NotificationAnswer_h */
