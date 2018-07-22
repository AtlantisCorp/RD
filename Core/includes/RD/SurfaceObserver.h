//
//  SurfaceObserver.h
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#ifndef SurfaceObserver_h
#define SurfaceObserver_h

#include "Global.h"
#include "ScreenPosition.h"
#include "RectSize.h"

namespace RD
{
    class Surface;
    
    /**
     * @brief Observes a surface.
     */
    class SurfaceObserver
    {
    public:
        
        /*! @brief Default constructor. */
        SurfaceObserver() noexcept = default;
        
        /*! @brief Default destructor. */
        virtual ~SurfaceObserver() noexcept = default;
        
        /*! @brief Called when the surface has moved to a new position. */
        virtual void onSurfaceDidMove(const Surface* surface, const ScreenPosition& newPosition) {}
        
        /*! @brief Called when a surface has resized. */
        virtual void onSurfaceDidResize(const Surface* surface, const RectSize& newSize) {}
        
        /*! @brief Called when a surface enters resizing mode. */
        virtual void onSurfaceEntersResizing(const Surface* surface) {}
        
        /*! @brief Called when a surface finishes resizing mode. */
        virtual void onSurfaceExitsResizing(const Surface* surface) {}
        
        /*! @brief Called when a surface will close. */
        virtual void onSurfaceWillClose(const Surface* surface) {}
        
        /*! @brief Called when a surface will hide. */
        virtual void onSurfaceWillHide(const Surface* surface) {}
        
        /*! @brief Called when a surface lock focus. */
        virtual void onSurfaceLockFocus(const Surface* surface) {}
        
        /*! @brief Called when a surface unlock focus. */
        virtual void onSurfaceUnlockFocus(const Surface* surface) {}
        
        /*! @brief Called when a surface unhide (or show). */
        virtual void onSurfaceUnhide(const Surface* surface) {}
    };
}

#endif /* SurfaceObserver_h */
