//
//  NotificationCenter.h
//  RD
//
//  Created by Jacques Tronconi on 20/07/2018.
//

#ifndef NotificationCenter_h
#define NotificationCenter_h

#include "NotificationObserver.h"
#include "Handle.h"

namespace RD
{
    class Application;
    
    /**
     * @brief Defines a basic interface for a NotificationCenter.
     *
     * NotificationCenter groups every Notification an object can send to the Engine. A Notification
     * is a message, identified by its module's name, its function's name (the sending function), a unique
     * notification name and a message.
     *
     * Observers can be added to the NotificationCenter to answer to those Notifications. Each observer returns
     * an Answer object, and the NotificationCenter collects all answers. The emitter of the notification can
     * then inspect those answers. Some notifications might expect an answer, or have a different behaviour.
     *
     * Application main object creates the NotificationCenter. Before Application is instanciated, no
     * NotificationCenter is available. It is then stored as a Handle.
     */
    class NotificationCenter
    {
        //! @brief Last answers collected.
        std::forward_list < NotificationAnswer > answers;
        
        //! @brief Observers registered.
        std::forward_list < Handle < NotificationObserver > > observers;
        
        //! @brief Mutex to access data.
        mutable std::mutex mutex;
        
    public:
        
        /*! @brief Default constructor. */
        NotificationCenter() noexcept = default;
        
        /*! @brief Default destructor. */
        virtual ~NotificationCenter() noexcept = default;
        
        /*! @brief Calls every observers for the given notification.
         * @return Answers collected while notifiating to every observers.
         */
        virtual std::forward_list < NotificationAnswer > notifiate(const Notification& notification);
        
        /*! @brief Returns answers collected with the last notification call. */
        virtual std::forward_list < NotificationAnswer > collectedAnswers() const;
        
        /*! @brief Register an observer. */
        virtual void addObserver(const Handle < NotificationObserver >& observer);
        
        /*! @brief Unregister an observer. */
        virtual void removeObserver(const Handle < NotificationObserver >& observer);
        
        /*! @brief Clear all observers. */
        virtual void clearObservers();
        
    public:
        
        /*! @brief Notifiate by using the default NotificationCenter. */
        static std::forward_list < NotificationAnswer > Notifiate(const Notification& notification);
        
        /*! @brief Notifiates by using the default NotificationCenter and creates a default notification. */
        static std::forward_list < NotificationAnswer > Notifiate(const char* moduleName,
                                                                  const char* functionName,
                                                                  const char* notificationName,
                                                                  const char* format, ...);
        
        /*! @brief Returns last answers collected by the default NotificationCenter. */
        static std::forward_list < NotificationAnswer > CollectedAnswers();
        
    private:
        
        //! @brief Default NotificationCenter.
        static Handle < NotificationCenter > defaultCenter;
        
        //! @brief Declares Application as a friend to let it access this member.
        friend class Application;
    };
    
    /*! @brief Notifiates something and throw a AbortRequestedException if one of the answer's shouldAbort()
     * returns true.
     */
    extern std::forward_list < NotificationAnswer > NotifiateAbort(const char* moduleName,
                                                                   const char* functionName,
                                                                   const char* notificationName,
                                                                   const char* format, ...);
}

#endif /* NotificationCenter_h */
