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
}

#endif /* Global_h */
