//
//  Emitter.h
//  RD
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#ifndef Emitter_h
#define Emitter_h

#include "ThreadedTasks.h"

namespace RD
{
    /**
     * @brief Emitting policy of a declared Emitter.
     *
     * An emitting policy is staticaly declared for an emitter, as Synchronized (not multithreaded)
     * or Asynchronized (multithreaded). If you don't want your emitter to emit its notification in
     * multithreaded mode, use EmittingPolicy::Asynchronized when declaring your emitters.
     *
     * @note
     * When compiling a custom version of this library, one can set EmittingPolicy::Default to whatever
     * value he wants to affect every emitting objects of the library.
     */
    enum class EmittingPolicy
    {
        Synchronized,
        Asynchronized,
        
        Default = Asynchronized
    };
    
    /**
     * @brief Generic interface for an emitter.
     *
     * An emitter is always tighly packed with a listener type. However, some listeners
     * might have another type, and some functions might be declared in a derived listener
     * type. To simplify all of this, we let the emitter send to any function (if available)
     * to its listeners by instanciating templated methods.
     *
     * @tparam Class Class of the listeners for this type.
     */
    template < typename Class, EmittingPolicy emitPolicy = EmittingPolicy::Default >
    class Emitter
    {
        //! @brief List of listeners for this type. Notes listeners are stored only as pointers.
        //! It is impossible for an emitter to ensure its listener is still valid. Listeners must
        //! unregiser themself from their emitter when they are destroyed.
        std::forward_list < Class* > listeners;
        
        //! @brief Locks the forward_list when iterating over it.
        std::mutex mutex;
        
    public:
        
        /*! @brief Default constructor. */
        Emitter() noexcept = default;
        
        /*! @brief Default destructor. */
        virtual ~Emitter() noexcept = default;
        
        /*! @brief Register a listener for all events emitted by this object.
         *
         * @param[in] listener Pointer to the listener object. Throw a NullPointerException
         *      if null. Listener is added to the end of the listening list.
         */
        virtual void addListener(Class* listener)
        {
            if (!listener)
                throw NullPointerException("Null pointer 'listener' for '%s::addListener()'.", typeid(*this).name());
            
            std::lock_guard<std::mutex> lock(mutex);
            listeners.push_front(listener);
        }
        
        /*! @brief Unregisters a listener from this object's lists.
         *
         * @param[in] listener Pointer to the listener object. Throw a NullPointerException
         *      if null.
         */
        virtual void removeListener(Class* listener)
        {
            if (!listener)
                throw NullPointerException("Null pointer 'listener' for '%s::removeListener()'.", typeid(*this).name());
            
            std::lock_guard<std::mutex> lock(mutex);
            listeners.remove(listener);
        }
        
        /*! @brief Clear all listeners in this emitter. */
        virtual void clearListeners()
        {
            std::lock_guard < std::mutex > lock(mutex);
            listeners.clear();
        }
        
    protected:
        
        /*! @brief Dispatches between 'emitSync' and 'emitAsync' functions regarding
         * the emitPolicy flag.
         *
         * @tparam Listener Class of the listener to send the notification.
         * @tparam ConnectFunc Function where to send the notification.
         * @tparam Args Arguments to pass to the function.
         *
         * @param[in] func Pointer to the connected function.
         * @param[in] args Arguments to pass.
         */
        template < typename Listener, typename ConnectFunc, typename... Args >
        inline void emit( ConnectFunc func, Args&&... args )
        {
            if ( emitPolicy == EmittingPolicy::Synchronized )
                emitSync< Listener >( func, std::forward < Args >(args)... );
            else if ( emitPolicy == EmittingPolicy::Asynchronized )
                emitAsync< Listener >( func, std::forward < Args >(args)... );
        }
        
        /*! @brief Emits to each listener the connected function with passed arguments.
         *
         * @tparam Listener Class of the listener to send the notification.
         * @tparam ConnectFunc Function where to send the notification.
         * @tparam Args Arguments to pass to the function.
         *
         * @param[in] func Pointer to the connected function.
         * @param[in] args Arguments to pass.
         *
         * @note
         * Synchronized version notifiates each listener one by one. Order of invocation
         * is guaranteed between listeners.
         */
        template < typename Listener, typename ConnectFunc, typename... Args >
        inline void emitSync( ConnectFunc func, Args&&... args )
        {
            std::lock_guard < std::mutex > lock( mutex );
            
            for ( auto l : listeners )
            {
                auto listener = static_cast < Listener* >( l );
                std::invoke( func, listener, std::forward < Args >( args )... );
            }
        }
        
        /*! @brief Emits to each listener the connected function with passed arguments.
         *
         * @tparam Listener Class of the listener to send the notification.
         * @tparam ConnectFunc Function where to send the notification.
         * @tparam Args Arguments to pass to the function.
         *
         * @param[in] func Pointer to the connected function.
         * @param[in] args Arguments to pass.
         *
         * @note
         * Asynchronized version spawns a new thread for each listener. Order of invocation
         * is not guaranteed and depends on many architectural factors.
         */
        template < typename Listener, typename ConnectFunc, typename... Args >
        inline void emitAsync( ConnectFunc func, Args&&... args )
        {
            std::lock_guard < std::mutex > lock( mutex );
            ThreadedTasks threads;
            
            for ( auto l : listeners )
            {
                threads.push( std::thread([func, l]( Args&&... args ) {
                    
                    auto listener = static_cast < Listener* >( l );
                    std::invoke( func, listener, std::forward < Args >( args )... );
                    
                }, std::forward < Args >( args )... ));
            }
        }
    };
}

#endif /* Emitter_h */
