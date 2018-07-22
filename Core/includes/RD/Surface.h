//
//  Surface.h
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#ifndef Surface_h
#define Surface_h

#include "Global.h"
#include "DriverResource.h"
#include "Emitter.h"
#include "SurfaceObserver.h"

namespace RD
{
    /**
     * @brief Styles that can be used to create a Surface.
     */
    namespace SurfaceStyle
    {
        //! @brief If surface is a window, surface is closable.
        static constexpr uint32_t Closable = 1 << 0;
        
        //! @brief If surface is a window, surface is resizable.
        static constexpr uint32_t Resizable = 1 << 1;
        
        //! @brief If surface is a window, surface has a title.
        static constexpr uint32_t Titled = 1 << 2;
        
        //! @brief If surface is a window, surface is borderless.
        static constexpr uint32_t Borderless = 1 << 3;
        
        //! @brief If surface is a window, it will be initialized as a Fullscreen window.
        static constexpr uint32_t Fullscreen = 1 << 4;
        
        //! @brief Default flags.
        static constexpr uint32_t Default = Closable | Resizable | Titled;
    }
    
    /**
     * @brief Generic Surface interface.
     *
     * A Surface is a generic object which either represents a Window, or a rectangle on a screen where a
     * driver can draw something into to present to the user.
     * As Surface is really platform-dependent and API dependent (multiple APIs can be available on some
     * platforms), this class is only a generic interface which show some basic methods to the user.
     *
     * Surface is initialized by the Driver, and thus is owned by it.
     */
    class Surface : public DriverResource, public Emitter < SurfaceObserver >
    {
    public:
        
        /*! @brief Default constructor.
         *
         * @param[in] driver Driver which constructed this object.
         * @param[in] width Width of the surface in pixels.
         * @param[in] height Height of the surface in pixels.
         * @param[in] title Title for this Surface, if available.
         * @param[in] objectName Name for this surface.
         * @param[in] style Style used to create the surface, applicable if it is a Window.
         * @param[in] extension A custom structure for platform-dependent configuration.
         */
        Surface(Driver* driver,
                uint32_t width,
                uint32_t height,
                const std::string& title,
                const std::string& objectName,
                uint32_t style = SurfaceStyle::Default,
                const void* extension = nullptr);
        
        /*! @brief Default destructor. */
        virtual ~Surface();
        
        /*! @brief Returns true if the surface is a window. */
        virtual bool isWindow() const = 0;
        
        /*! @brief Returns true if the surface is a view, i.e. a view in a window. */
        virtual bool isView() const = 0;
        
        /*! @brief Shows the surface (if applicable). */
        virtual void show() = 0;
        
        /*! @brief Moves the surface to new position (if applicable). */
        virtual void move(uint32_t x, uint32_t y) = 0;
        
        /*! @brief Resizes the surface to new size (if applicable). */
        virtual void resize(uint32_t width, uint32_t height) = 0;
        
        /*! @brief Returns the position of this Surface, relative to the Screen. */
        virtual ScreenPosition position() const = 0;
        
        /*! @brief Returns the size of this Surface, in pixels. */
        virtual RectSize size() const = 0;
        
        /*! @brief Focus the surface (if applicable). Some events like keyboard, mouse, etc.
         * will be focused on this surface. */
        virtual void lockFocus() = 0;
        
        /*! @brief Close the surface. */
        virtual void close() = 0;
        
        /*! @brief Hides the surface (if applicable). */
        virtual void hide() = 0;
        
        /*! @brief Unhides the surface (if applicable). */
        virtual void unhide() = 0;
        
        /*! @brief Returns true if surface is closed. */
        virtual bool closed() const = 0;
        
    protected:
        
        /*! @brief Calls \ref close when called. */
        virtual void onDriverClear();
    };
    
}

#endif /* Surface_h */
