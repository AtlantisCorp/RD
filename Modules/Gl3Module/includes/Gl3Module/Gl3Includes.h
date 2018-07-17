/**
 * @file Gl3Module/Gl3Includes.h
 * @author Luk2010
 * @date 15/07/2018
 *
 * @brief
 * Includes system libraries for the whole module. Different libraries
 * are included upon the selected CMake compile definitions.
 */

#ifndef Gl3HaveOpenGL
#   error "Gl3HaveOpenGL is not defined. This means OpenGL is not present on this system."
#endif

#ifdef Gl3HaveCocoa
#   include <OpenGL/OpenGL.h>
#
#   define Gl3ContextHandle CGLContextObj
#
#   ifdef __OBJC__
#       include <Cocoa/Cocoa.h>
#       define Gl3Id id
#   else
#       define Gl3Id void*
#   endif
#
#endif
