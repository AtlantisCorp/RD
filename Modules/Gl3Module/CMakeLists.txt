# @file Gl3Module/CMakeLists.txt
# @author Luk2010
# @date 13/07/2018
#
# @brief
# CMake Gl3Module project definition with supported 
# platforms OSX, WIN and UNIX. 
#
# @note
# C++17 is highly recommended to compile this library. Some features
# might not be supported on other C++ versions. 
# OpenGL is also required to compile this project. Linking to this library
# might differ from a platform to another. On OSX, OpenGL's framework is 
# used. On Linux, OpenGL library is included with your distribution. 
# 
# @Platform OSX
# Cocoa and OpenGL frameworks are linked against this library. Cocoa 
# lets us give the user a native window system feature. User can include
# 'Gl3Module/OSX/OSXSurface.h' to handle some complex cases like including
# a Gl3SurfaceView into a custom NSView. 
#
# @Platform Linux
# X11 or XCB are provided to support OpenGL draw surfaces. XCB is preferred
# to X11 as this is a more modern library, but a X11 display must always be 
# available. User can include 'Gl3Module/X11/X11Surface.h' to handle complex
# X11's cases, or 'Gl3Module/XCB/XCBSurface.h' when XCB is required. User can
# stop the program if XCB is not found by using 'Gl3::GetXCBConnection' to 
# check the XCB connection.
# If XCB nor X11 can be found, CMake script will try to get Wayland libraries
# for experimental support by using EGL library with eglBindAPI(EGL_OPENGL_API).
# Wayland requires EGL and GLVND support to be available. 
#
# @Platform WIN
# Windows' native window system is used by the library to provided direct access
# to Windows objects. User can include 'Gl3Module/WIN/WINSurface.h' to handle complex
# cases with HWND handles. 
#
# @Options
#  - INCLUDE_X11: ON by default. Includes X11 when available.
#  - INCLUDE_XCB: ON by default. Includes XCB if X11 AND XCB are available.
#  - INCLUDE_WAYLAND: OFF by default. Includes Wayland support. If X11 and XCB are
#       both unavailable, Wayland will still be included (if available).

# Gl3Module : Manages an OpenGL 3 Driver. 
project(Gl3Module VERSION 1.0.0 LANGUAGES CXX)

option(Gl3IncludeX11 "Includes X11 on Unix platforms." ON)
option(Gl3IncludeXCB "Includes XCB on Unix platforms. Require 'Gl3IncludeX11'." ON)
option(Gl3IncludeWayland "Includes Wayland on Unix platforms." OFF)

# Main sources for all platforms.
file(GLOB Gl3Headers "includes/Gl3Module/*.h")
file(GLOB Gl3Sources "src/*.cpp")

# Main library module. 
add_library(Gl3Module SHARED ${Gl3Headers} ${Gl3Sources})

target_link_libraries(Gl3Module 
    PUBLIC
        RD
)

target_include_directories(Gl3Module
    PRIVATE
        "includes/Gl3Module"
)

# Finds OpenGL with find_package. If found, we can already 
# set some definitions and libraries.
find_package(OpenGL REQUIRED)

target_link_libraries(Gl3Module
    PUBLIC 
        OpenGL::GL
)

target_compile_definitions(Gl3Module
    PUBLIC
        Gl3HaveOpenGL
        Gl3OpenGLPath=${OPENGL_INCLUDE_DIR}
)

# Try to get OpenGL and Cocoa fameworks to work on OSX.
if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    
    # Adds System frameworks OpenGL and Cocoa.
    include_directories("/System/Library/Frameworks")
    
    find_library(COCOA_LIBRARY Cocoa)
    
    target_link_libraries(Gl3Module 
        PUBLIC
            ${COCOA_LIBRARY}
    )
    
    # Adds Darwin Gl3 definitions.
    target_compile_definitions(Gl3Module
        PUBLIC
            Gl3HaveCocoa
    )
    
    # Adds possible platform dependent sources.
    file(GLOB Gl3OSXFiles 
            "includes/Gl3Module/OSX/*.h" 
            "src/OSX/*.mm" 
            "src/OSX/*.cpp"
    )
    
    target_sources(Gl3Module PRIVATE ${Gl3OSXFiles})
    
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    
    # Adds Windows and OpenGL libraries. 
    
    target_compile_definitions(Gl3Module
        PUBLIC
            Gl3HaveWindows
    )
    
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    
    # Try to find X11. 
    find_package(X11)
    
    # If X11 is found, we immediatly links to X11 libraries and to include
    # X11 directories. For X11, OpenGL and GLX are required.
    if(X11_FOUND AND Gl3IncludeX11)
        find_package(OpenGL COMPONENTS GLX)
        
        if(OPENGL_FOUND)
            target_link_libraries(Gl3Module
                PUBLIC
                    OpenGL::GLX
                    ${X11_LIBRARIES}
            )
            
            target_compile_definitions(Gl3Module
                PUBLIC
                    Gl3HaveGLX
                    Gl3HaveX11
            )
            
            target_include_directories(Gl3Module
                PUBLIC
                    ${X11_INCLUDE_DIR}
            )
            
            file(GLOB Gl3X11Files
                "includes/Gl3Module/X11/*.h"
                "src/X11/*.cpp"
            )
        
            target_sources(Gl3Module 
                PRIVATE 
                    ${Gl3X11Files}
            )
            
            # Also see for XCB, which is our main support.
            include(CMake/FindXCB.cmake)
            find_package(XCB)
        
            if(XCB_FOUND AND Gl3IncludeXCB)
                target_link_libraries(Gl3Module 
                    PUBLIC 
                        ${XCB_LIBRARIES}
                )
                
                target_include_directories(Gl3Module 
                    PUBLIC 
                        ${XCB_INCLUDE_DIRS}
                )
                
                target_compile_definitions(Gl3Module 
                    PUBLIC 
                        Gl3HaveXCB
                )
            
                file(GLOB Gl3XCBFiles
                    "includes/Gl3Module/XCB/*.h"
                    "src/XCB/*.cpp"
                )
            
                target_sources(Gl3Module PRIVATE ${Gl3XCBFiles})
            endif() 
        endif() 
    endif()
    
    # When Wayland is found on the host, try to compile with it. However,
    # this is still an experimental support. See options for conditional
    # CMake compilation. Wayland requires EGL, which seems to require for
    # GLVND library. CMake sees GLVND with OpenGL::OpenGL COMPONENTS.
    include(CMake/FindWayland.cmake)
    find_package(Wayland)
    
    if(WAYLAND_FOUND AND WAYLAND_EGL_FOUND AND Gl3IncludeWayland)
        find_package(OpenGL
            COMPONENTS
                EGL
                OpenGL
        )
        
        if(OPENGL_FOUND)
            target_link_libraries(Gl3Module
                PUBLIC
                    OpenGL::EGL
                    OpenGL::OpenGL
                    ${WAYLAND_LIBRARIES}
            )
            
            target_include_directories(Gl3Module
                PUBLIC
                    ${WAYLAND_INCLUDE_DIR}
            )
            
            target_compile_definitions(Gl3Module
                PUBLIC
                    Gl3HaveGLVND
                    Gl3HaveEGL
                    Gl3HaveWayland
                    Gl3HaveWaylandEGL
            )
            
            file(GLOB Gl3WaylandFiles
                "includes/Gl3Module/Wayland/*.h"
                "src/Wayland/*.cpp"
            )
        
            target_sources(Gl3Module 
                PRIVATE 
                    ${Gl3WaylandFiles}
            )
            
        endif()
    endif() 
endif()

set_target_properties(Gl3Module
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY            ${RD_MODULE_LIB_OUTPUT}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG      ${RD_MODULE_LIB_OUTPUT}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE    ${RD_MODULE_LIB_OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY            ${RD_MODULE_LIB_OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG      ${RD_MODULE_LIB_OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE    ${RD_MODULE_LIB_OUTPUT}
)

# Try to set C++17 Flags for Xcode Projects.
if(${CMAKE_GENERATOR} MATCHES "Xcode")

    macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
        set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
                      ${XCODE_VALUE})
    endmacro (set_xcode_property)

    set_xcode_property(Gl3Module CLANG_CXX_LANGUAGE_STANDARD "c++17")
    set_xcode_property(Gl3Module CLANG_CXX_LIBRARY "libc++")

    set_property(TARGET Gl3Module PROPERTY CXX_STANDARD 17)
    set_property(TARGET Gl3Module PROPERTY CXX_STANDARD_REQUIRED ON)

else()

    set_property(TARGET Gl3Module PROPERTY CXX_STANDARD 17)
    set_property(TARGET Gl3Module PROPERTY CXX_STANDARD_REQUIRED ON)

endif(${CMAKE_GENERATOR} MATCHES "Xcode")

# Configure install for this module.
install(TARGETS Gl3Module LIBRARY DESTINATION lib/Modules
                          ARCHIVE DESTINATION lib/Modules
                          FRAMEWORK DESTINATION lib/Modules)