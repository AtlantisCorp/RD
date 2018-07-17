cmake_minimum_required(VERSION 3.7)

# Main project. Includes Core, Modules, Tests, etc.
project(RD)

# For main projects, output path is always this. 
set(LIBRARY_OUTPUT_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(ARCHIVE_OUTPUT_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# For modules, output pathes is this one.
set(RD_MODULE_LIB_OUTPUT ${LIBRARY_OUTPUT_PATH}/Modules)

# We always include the Core include directory for those projects. 
include_directories(Core/includes)

# Our main directory and project.
add_subdirectory(Core)

# Adds here every modules.
add_subdirectory(Modules/Gl3Module)

# Adds here every examples.
add_subdirectory(Examples/CAppDelegate)

# CPack configuration. 
include(CPack)
set(CPACK_BUNDLE_NAME "RD Package")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "RD Engine package")
set(CPACK_PACKAGE_VENDOR "Atlanti's Corporation")