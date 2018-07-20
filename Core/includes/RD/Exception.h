//
//  Exception.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef Exception_h
#define Exception_h

#include "Global.h"

namespace RD
{
    /**
     * @brief Generic exception that may be throwed by any part of RD's engine.
     */
    class Exception : public std::exception
    {
        //! @brief Error code for this exception. It might be anything.
        uint32_t errorCode;
        
        //! @brief Message of this exception.
        std::string message;
        
    public:
        
        /*! @brief Default constructor.
         *
         * @param[in] error Error code to output.
         * @param[in] str Message to output.
         */
        Exception( uint32_t error, const std::string& str ) noexcept;
        
        /*! @brief Constructs an Exception with a message handled by vsnprintf.
         *
         * @param[in] error Error code to output.
         * @param[in] format String to use to format output message.
         * @param[in] ... Parameters according to format and vsnprintf.
         */
        Exception( uint32_t error, const char* format, ... );
        
        /*! @brief Default destructor. */
        virtual ~Exception() noexcept = default;
        
        /*! @brief Returns the error code of this exception. */
        uint32_t code() const noexcept;
        
        /*! @brief Returns the message of this exception. */
        const char* what() const noexcept;
    };
    
    /**
     * @brief Exception for Null pointer errors.
     *
     * It is generally throwed when trying to access a Null pointer memory,
     * as trying operator * on null.
     *
     * Its error code is 1.
     */
    class NullPointerException : public Exception
    {
        //! Error code of this exception.
        static constexpr uint32_t ErrorCode = 1;
        
    public:
        
        /*! @brief Constructs this exception.
         *
         * @tparam Args Type of arguments to pass to Exception base constructor.
         * @param[in] format String to use to output the message.
         * @param[in] args Arguments to pass to vsnprintf.
         */
        template < typename... Args >
        NullPointerException( const char* format, Args&&... args )
        : Exception( ErrorCode, format, std::forward<Args>(args)... )
        {}
    };
    
    /**
     * @brief Exception for when a Handle is added to a list where it should
     * be unique, but it is already in.
     *
     * ErrorCode = 2
     */
    class HandleNotUniqueException : public Exception
    {
        //! Error code of this exception.
        static constexpr uint32_t ErrorCode = 2;
        
    public:
        
        /*! @brief Constructs this exception.
         *
         * @param[in] ptr Address of the handle.
         */
        HandleNotUniqueException( uintptr_t ptr );
    };
    
    /**
     * @brief Exception called when a required module is not loaded.
     *
     * ErrorCode = 3.
     */
    class ModuleNotLoadedException : public Exception
    {
        //! @brief Error code for this exception.
        static constexpr uint32_t ErrorCode = 3;
        
    public:
        
        /*! @brief Default constructor.
         * @param[in] libname Library that should be loaded.
         */
        ModuleNotLoadedException( const std::string& libname );
    };
    
    /**
     * @brief Launchd when a module has its abort requested.
     *
     * ErrorCode = 4.
     */
    class AbortRequestedException : public Exception
    {
        //! @brief Error code for this exception.
        static constexpr uint32_t ErrorCode = 4;
        
    public:
        
        /*! @brief Default constructor.
         * @param[in] module Module name.
         * @param[in] function Function name.
         * @param[in] message Message for the abort.
         */
        AbortRequestedException(const std::string& module, const std::string& function, const std::string& message);
    };
}

#endif /* Exception_h */
