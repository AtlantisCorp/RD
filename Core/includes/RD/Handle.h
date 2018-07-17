//
//  Handle.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef Handle_h
#define Handle_h

#include "Global.h"
#include "Exception.h"
#include "Allocator.h"

namespace RD
{
    /**
     * @brief Generic Handle to share a particular object.
     *
     * An object can have a handle only if it can (and will) be shared between
     * multiple other objects. For example, RD::Application static instance is
     * a handle to the first created instance.
     *
     */
    template < typename Handled >
    class Handle
    {
        //! @brief Shared instance of our handled object.
        std::shared_ptr < Handled > instance;
        
    public:
        
        /*! @brief Default constructor. (Null handle) */
        Handle() noexcept = default;
        
        /*! @brief Constructs a handle from a raw pointer.
         *
         * @param[in] raw Pointer to handle. Notes that this handle will own
         *      the pointer as it will have only one share.
         */
        explicit Handle( Handled* raw ) noexcept : instance( raw ) {}
        
        /*! @brief Constructs a handle from a std::shared_ptr.
         *
         * @param[in] ptr Shared pointer to copy.
         */
        explicit Handle( std::shared_ptr < Handled > ptr ) noexcept : instance( ptr ) {}
        
        /*! @brief Copy constructor. */
        Handle( const Handle& ) noexcept = default;
        
        /*! @brief Move constructor. */
        Handle( Handle&& ) noexcept = default;
        
        /*! @brief Conversion when type is derived. */
        template < typename Derived >
        Handle(const Handle < Derived >& rhs,
               typename std::enable_if<std::is_base_of<Handled, Derived>::value>::type* = 0) noexcept
        : instance( std::static_pointer_cast < Handled >( rhs.shared_ptr() ) )
        {
            
        }
        
        /*! @brief Default destructor. */
        ~Handle() noexcept = default;
        
        /*! @brief Default move assignment. */
        Handle& operator = ( Handle&& rhs ) = default;
        
        /*! @brief Returns true if handled pointer is not null. */
        inline bool valid() const noexcept { return static_cast<bool>(instance); }
        
        /*! @brief Same as 'valid()'. */
        inline operator bool() const noexcept { return static_cast<bool>(instance); }
        
        /*! @brief Returns true if handled object has only one shared pointer. */
        inline bool owned() const noexcept { return instance.unique(); }
        
        /*! @brief Returns the handled object.
         *
         * If null, throws a NullPointerException exception.
         */
        Handled& operator * ()
        {
            if ( !instance )
                throw NullPointerException( "%s: Null handled object but 'operator *' is called.", typeid(*this).name() );
            return *instance;
        }
        
        /*! @brief Returns the handled object.
         *
         * If null, throws a NullPointerException exception.
         */
        const Handled& operator * () const
        {
            if ( !instance )
                throw NullPointerException( "%s: Null handled object but 'operator *' is called.", typeid(*this).name() );
            return *instance;
        }
        
        /*! @brief Returns the pointer handled.
         *
         * @note
         * Returned pointer must not be deleted. Handle only is responsible
         * of deleting the pointer.
         */
        inline Handled* operator ->() noexcept { return instance.get(); }
        
        /*! @brief Returns the pointer handled.
         *
         * @note
         * Returned pointer must not be deleted. Handle only is responsible
         * of deleting the pointer.
         */
        inline const Handled* operator ->() const noexcept { return instance.get(); }
        
        /*! @brief Returns the pointer handled.
         *
         * @note
         * Returned pointer must not be deleted. Handle only is responsible
         * of deleting the pointer.
         */
        inline Handled* ptr() noexcept { return instance.get(); }
        
        /*! @brief Returns the pointer handled.
         *
         * @note
         * Returned pointer must not be deleted. Handle only is responsible
         * of deleting the pointer.
         */
        inline const Handled* ptr() const noexcept { return instance.get(); }
        
        /*! @brief Swaps this handle with another one.
         *
         * @param[in] rhs Handle to swap with.
         */
        void swap( Handle& rhs ) noexcept { std::swap(instance, rhs.instance); }
        
        /*! @brief Copy assignment operator. */
        Handle& operator = ( const Handle& ) noexcept = default;
        
        /*! @brief Returns the underlying std::shared_ptr. */
        const std::shared_ptr < Handled > shared_ptr() const noexcept { return instance; }
        
        /*! @brief Resets the Handle with a null value. */
        void reset() { instance.reset(); }
    };
    
    /*! @brief Utility function to create a handle with the given allocator.
     *
     * @tparam T Handled type.
     * @tparam Alloc Allocator type.
     * @tparam Args Arguments to call constructor of the derived type.
     *
     * @param[in] alloc Allocator to use when creating the object.
     * @param[in] args Arguments to use. (can be void)
     *
     * @return A Handle to the created object.
     */
    template < typename T, typename Alloc, typename... Args >
    Handle < T > CreateHandleAlloc( const Alloc& alloc, Args&&... args )
    {
        return Handle < T >( std::allocate_shared < T >( alloc, std::forward<Args>(args)... ) );
    }
    
    /*! @brief Utility function to create a handle with default allocator.
     *
     * Default allocator tracks memory allocation and deallocation through
     * MemoryTracker singleton. @see TrackAllocator for more explanations.
     *
     * @tparam T Handled type.
     * @tparam Args Arguments to call constructor of the derived type.
     * @param[in] args Arguments to use. (can be void)
     *
     * @return A Handle to the created object.
     */
    template < typename T, typename... Args >
    Handle < T > CreateHandle( Args&&... args )
    {
        return CreateHandleAlloc < T >( Allocator<T>(), std::forward<Args>(args)... );
    }
    
    /*! @brief Utility function used by modules to create a pointer to a handle.
     *
     * When a module has its 'loadHash' function called, it must return a void pointer
     * to the created Handle. However, as this implies a 'complicated' typo, this function
     * is here to make it prettier. That's all.
     *
     * @tparam Class Class of the handle.
     * @tparam Derived Class of the derived which will be handled.
     * @tparam Args Arguments to pass to the Derived class constructor.
     * @param args Arguments for the derived class constructor.
     *
     * @return A pointer to a Handle < Derived > object.
     */
    template < typename Class, typename Derived, typename... Args >
    Handle < Class >* CreateHandlePtr( Args&&... args )
    {
        auto hdl = CreateHandle < Derived >( std::forward<Args>(args)... );
        return new Handle < Class >( hdl );
    }
     
}

#endif /* Handle_h */
