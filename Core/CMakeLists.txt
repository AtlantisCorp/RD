# @file Core/CMakeLists.txt
# @author Luk2010
# @date 13/07/2018
#
# @brief
# CMake RD project definition. This is the main library of
# the RD ecosystem. libRD is required for all RD's program,
# and additionals modules bring other functionalities as Driver's
# implementations, or other subsystem definitions. 
#
# @note
# A dynamic library is required to compile this project. On Windows
# platforms, libdlfcn-win32 is included and compiled automatically.
# This provide the same dynamic library loading on all platforms. However,
# libraries file extension may differ on those platforms. 
# C++11 is also required. C++17 is advised, but not required to compile
# this project. 

cmake_minimum_required(VERSION 3.7)

# Core project of RD ecosystem. This is where all begins. 
project(libRD)

file(GLOB Sources "src/*.cpp")
file(GLOB Headers "includes/RD/*.h")

add_library(RD SHARED ${Sources} ${Headers})

target_include_directories(RD PRIVATE "includes/RD")
target_link_libraries(RD dl)

target_compile_features(RD PUBLIC cxx_nullptr)
target_compile_features(RD PUBLIC cxx_lambdas)
target_compile_features(RD PUBLIC cxx_noexcept)

# =========================================================================
# Enables only on Darwin platform.
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# =========================================================================
    
    # Make the framework. 
    
    set_target_properties( RD PROPERTIES
        FRAMEWORK TRUE 
        FRAMEWORK_VERSION A
        MACOSX_FRAMEWORK_IDENTIFIER com.acorp.atl 
        VERSION 1.0.0 
        SOVERSION 1.0.0
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Mac Developer" 
    )

# =========================================================================
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# =========================================================================

# Try to set C++17 Flags for Xcode Projects.
if(${CMAKE_GENERATOR} MATCHES "Xcode")

    macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
        set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
                      ${XCODE_VALUE})
    endmacro (set_xcode_property)

    set_xcode_property(RD CLANG_CXX_LANGUAGE_STANDARD "c++17")
    set_xcode_property(RD CLANG_CXX_LIBRARY "libc++")

    set_property(TARGET RD PROPERTY CXX_STANDARD 17)
    set_property(TARGET RD PROPERTY CXX_STANDARD_REQUIRED ON)

else()

    set_property(TARGET RD PROPERTY CXX_STANDARD 17)
    set_property(TARGET RD PROPERTY CXX_STANDARD_REQUIRED ON)

endif(${CMAKE_GENERATOR} MATCHES "Xcode")

install(
    TARGETS RD LIBRARY DESTINATION lib
               ARCHIVE DESTINATION lib
               FRAMEWORK DESTINATION lib
)