//
//  Exception.cpp
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#include "Exception.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    Exception::Exception( uint32_t error, const std::string& str ) noexcept
    : errorCode(error), message(str)
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Exception::Exception( uint32_t error, const char* format, ... )
    : errorCode(error)
    {
        if ( format && strlen(format) )
        {
            va_list ap;
            va_start(ap, format);
            
            char buffer[2048] = { 0 };
            vsnprintf(buffer, sizeof(buffer), format, ap);
            
            va_end(ap);
            
            message = std::string( buffer );
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    uint32_t Exception::code() const noexcept
    {
        return errorCode;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    const char* Exception::what() const noexcept
    {
        return message.data();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    HandleNotUniqueException::HandleNotUniqueException( uintptr_t ptr )
    : Exception( ErrorCode, "Handle not unique (%l).", ptr )
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    ModuleNotLoadedException::ModuleNotLoadedException( const std::string& libname )
    : Exception( ErrorCode, "%s required but not loaded.", libname.data() )
    {
        
    }
}
