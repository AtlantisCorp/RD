//
//  Global.h
//  test
//
//  Created by Jacques Tronconi on 10/07/2018.
//  Copyright © 2018 Jacques Tronconi. All rights reserved.
//

#ifndef Global_h
#define Global_h

#include <iostream>
#include <memory>
#include <exception>
#include <cstdarg>
#include <typeinfo>
#include <mutex>
#include <thread>
#include <future>
#include <chrono>
#include <forward_list>
#include <dlfcn.h>
#include <map>
#include <list>
#include <type_traits>
#include <string>
#include <queue>
#include <functional>

namespace RD
{
    /*! @brief Typedef used to define what is a delta time (or ticks) in the engine. */
    using Clock = std::chrono::high_resolution_clock;
    
    /*! @brief Explodes a string in sub parts.
     *
     * @param[in] s String to explode.
     * @param[in] c Character to use to explode the string.
     *
     * @see http://www.cplusplus.com/articles/2wA0RXSz/
     */
    const std::vector<std::string> explode(const std::string& s, const char& c);
    
    /**
     * @brief Simple structure to define a Version object.
     */
    struct Version
    {
        //! @brief Represents the major version number.
        int major = 0;
        
        //! @brief Represents the minor version number.
        int minor = 0;
        
        //! @brief Represents the patch number.
        int patch = 0;
        
        //! @brief Represents the build number.
        int build = 0;
    };
}
    
/*! @brief Prints a Version object to a std::ostream interface. */
extern std::ostream& operator << (std::ostream& stream, const RD::Version& version);
    
/*! @brief Converts an input stream to a version object. */
extern std::istream& operator >> (std::istream& stream, RD::Version& version);

#endif /* Global_h */
