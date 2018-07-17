cmake_minimum_required(VERSION 3.7)

project(cappdelegate)

add_executable(cappdelegate main.cpp)
target_link_libraries(cappdelegate RD)

# Try to set C++17 Flags for Xcode Projects.
if(${CMAKE_GENERATOR} MATCHES "Xcode")

    macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
        set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
                      ${XCODE_VALUE})
    endmacro (set_xcode_property)

    set_xcode_property(cappdelegate CLANG_CXX_LANGUAGE_STANDARD "c++17")
    set_xcode_property(cappdelegate CLANG_CXX_LIBRARY "libc++")

    set_property(TARGET cappdelegate PROPERTY CXX_STANDARD 17)
    set_property(TARGET cappdelegate PROPERTY CXX_STANDARD_REQUIRED ON)

else()

    set_property(TARGET cappdelegate PROPERTY CXX_STANDARD 17)
    set_property(TARGET cappdelegate PROPERTY CXX_STANDARD_REQUIRED ON)

endif(${CMAKE_GENERATOR} MATCHES "Xcode")

install(TARGETS cappdelegate RUNTIME DESTINATION bin)