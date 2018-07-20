//
//  NotificationObserver.h
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#ifndef NotificationObserver_h
#define NotificationObserver_h

#include "Notification.h"
#include "NotificationAnswer.h"

namespace RD
{
    class NotificationCenter;
    
    /**
     * @brief Interface an observer for the NotificationCenter.
     *
     * A NotificationObserver register itself to a NotificationCenter he wants to be notifiated from. The
     * observer must respond to 'observe' function to process notifications, and to 'shouldObserve' to tell
     * the NotificationCenter if the Notification should be processed by the observer.
     */
    class NotificationObserver
    {
    public:
        
        /*! @brief Default constructor. */
        NotificationObserver() noexcept = default;
        
        /*! @brief Default destructor. */
        virtual ~NotificationObserver() noexcept = default;
        
        /*! @brief Process the given notification.
         *
         * @param[in] center Center which sends the notification.
         * @param[in] notification Notification to observe.
         *
         * @return A NotificationAnswer depending on the notification.
         */
        virtual NotificationAnswer observe(const NotificationCenter* center, const Notification& notification);
        
        /*! @brief Returns a boolean indicating true if the NotificationCenter can send the notificatio
         * to this observer.
         *
         * @param[in] center Center which sends the notification.
         * @param[in] notification Notification to observe.
         *
         * @return True if NotificationCenter can call 'observe' on this observer.
         */
        virtual bool shouldObserve(const NotificationCenter* center, const Notification& notification) const;
    };
}

#endif /* NotificationObserver_h */
