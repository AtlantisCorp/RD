//
//  Driver.h
//  RD
//
//  Created by Jacques Tronconi on 17/07/2018.
//

#ifndef Driver_h
#define Driver_h

#include "Global.h"
#include "HashedString.h"
#include "DriverResource.h"
#include "Surface.h"
#include "Handle.h"
#include "Emitter.h"
#include "SurfaceObserver.h"
#include "Module.h"

namespace RD
{
    /**
     * @brief Generic properties for a Graphics API.
     */
    struct DriverConfiguration
    {
        //! @brief Boolean indicating if multisampling should be used.
        bool multisampling = false;
        
        //! @brief Number of buffers used when multisampling.
        short sampleBuffers = 1;
        
        //! @brief Number of samples used when multisampling.
        short samples = 4;
        
        //! @brief Individual colors descriptions, in bits.
        struct
        {
            short red = 0;
            short green = 0;
            short blue = 0;
            short alpha = 0;
        } colors;
        
        //! @brief Bits per pixel. (if colors is not used)
        short bpp = 32;
        
        //! @brief Number of buffers to swap and use. (Generally this is 2 for double buffers and 3 for triple
        //! buffers, but some API may offer the possibility to have more than 3 back buffers.)
        short buffers = 2;
        
        //! @brief Pointer to an additional structure defined by Modules to configure a specific driver.
        //! @see Gl3DriverConfiguration for an example.
        void* extension = nullptr;
    };
    
    class Driver;
    
    /**
     * @brief Observer for the Driver class.
     */
    class DriverObserver
    {
    public:
        
        /*! @brief Default destructor. */
        virtual ~DriverObserver() noexcept = default;
        
        /*! @brief Called when a driver created a new surface. */
        virtual void onDriverCreatesSurface(const Driver* driver, const Surface* surface) {}
        
        /*! @brief Called before a driver clears its resources. */
        virtual void onDriverWillClear(const Driver*) {}
        
        /*! @brief Called after a driver did clear its resources. */
        virtual void onDriverDidClear(const Driver*) {}
    };
    
    /** @defgroup DriverNotifications
     * @{
     */
    
    static constexpr const char* kDriverInvalidSurfaceCreationNotification = "DriverInvalidSurfaceCreationNotification";
    static constexpr const char* kDriverSurfaceCreatedNotification = "DriverSurfaceCreatedNotification";
    static constexpr const char* kDriverDidClearNotification = "DriverDidClearNotification";
     
    /** @} */
    
    /**
     * @brief Generic interface for a Graphic API wrapper.
     *
     * A Driver is a wrapper for a given Graphic API (like OpenGL, DirectX, or others). It
     * has many functions like handling Windows (or Surfaces) creation, handling buffers swaps,
     * and creation of most of the objects related to the graphic pipeline.
     *
     * A Driver has one or more CommandQueues. Those queues can be commited to the graphic pipeline
     * whenever you want, and the commands commited will be executed after a call to 'present()'.
     *
     * @note
     * Driver can only be created by Modules, as they always implie some platform-dependent or API-dependent
     * code. If you want to create your own Driver, either create your external module or create an internal
     * module and load it with \ref Application::addModule.
     *
     * Driver does a kind of garbage collection with DriverResources. When a resource is about to be destroyed,
     * as other threads might use them (like Surface) or did not finish their tasks, the resource is sent into
     * a later released queue. At \ref onModuleDidUpdate, the queue is popped from all the resources untill one
     * has its used flag set. DriverResource's users have the responsability to lock and unlock the resource
     * (which set and unset this flag) to tell the Driver if it can destroy the Handle or not.
     *
     * A Surface doesn't need any locking, apart when the surface is closing. Surface may lock itself untill it
     * is closed and unlock itself when its done.
     *
     */
    class Driver : public Emitter < DriverObserver >, public ModuleListener
    {
        /**
         * @brief Helper to be notifiated when a Surface is closed.
         *
         * Registered for each surface created. When a surface closes, it calls onSurfaceWillClose
         * which makes us remove the surface from the surface's list. Resource is moved to a later released
         * queue, released when module calls \ref ModuleListener::onModuleUpdate.
         *
         * @note
         * If you want the surface to be completly destroyed, you can still make your own observer and
         * wait for onSurfaceWillClose. However, this will destroy every handle instance of your surface,
         * which will cause it to be deleted while closing at the same time. Driver already sets a flag to
         * destroy the surface at another tick, i.e. when it's not in use.
         */
        class SurfaceHelper final : public SurfaceObserver
        {
            //! @brief This driver.
            Driver* driver;
            
        public:
            
            /*! @brief Default constructor. */
            SurfaceHelper(Driver* d);
            
            /*! @brief Default destructor. */
            ~SurfaceHelper() noexcept = default;
            
            /*! @brief Removes the surface from the driver's list and append the handle for later release. */
            void onSurfaceWillClose(const Surface*);
        };
        
        // Makes it a friend because we own this class.
        friend class SurfaceHelper;
        
        //! @brief Resources created by this Driver.
        std::map < HashedString, Handle < DriverResource > > resources;
        
        //! @brief Surfaces handled by the Driver.
        std::map < HashedString, Handle < Surface > > surfaces;
        
        //! @brief Mutex to lock when accessing data.
        mutable std::mutex mutex;
        
        //! @brief Helper to destroy closed surfaces.
        SurfaceHelper surfaceHelper;
        
        //! @brief An array of DriverResource to be destroyed at next update.
        std::queue < Handle < DriverResource > > laterReleaseQueue;
        
        //! @brief Mutex to access later release queue.
        mutable std::mutex lrqMutex;
        
    public:
        
        /*! @brief Default constructor. */
        Driver() noexcept;
        
        /*! @brief Default destructor. */
        virtual ~Driver() noexcept = default;
        
        /*! @brief Returns the driver's name. */
        virtual const std::string name() const noexcept = 0;
        
        /*! @brief Returns the driver current version. */
        virtual const Version version() const = 0;
        
        /*! @brief Returns true if the driver was successfully initialized. */
        virtual bool valid() const = 0;
        
        /*! @brief Creates a surface and returns it, if no error occured.
         *
         * @param[in] width Width of the surface in pixels.
         * @param[in] height Height of the surface in pixels.
         * @param[in] title Title for this Surface, if available.
         * @param[in] objectName Name for this surface.
         * @param[in] style Style used to create the surface, applicable if it is a Window.
         * @param[in] extension A custom structure for platform-dependent configuration.
         */
        Handle < Surface > createSurface(uint32_t width, uint32_t height, const std::string& title, const std::string& objectName, uint32_t style = SurfaceStyle::Default, const void* extension = nullptr);
        
        /*! @brief Copies the surface's list to be accessed by a derived class.
         *
         * As this might seems a slow operation, this lets us locking the internal data and
         * copy it into a new map. Derived classes can then access this array freely.
         *
         * @sa lockResources(), unlockResources().
         */
        std::map < HashedString, Handle < Surface > > loadSurfaces();
        
        /*! @brief Copies the resource's list to be accessed by a derived class.
         *
         * As this might seems a slow operation, this lets us locking the internal data and
         * copy it into a new map. Derived classes can then access this array freely.
         *
         * @sa lockResources(), unlockResources().
         */
        std::map < HashedString, Handle < DriverResource > > loadResources();
        
        /*! @brief Clears every DriverResources created by this driver.
         *
         * Basically it only emits \ref DriverResource::onDriverClear() function to every DriverResource
         * present. Notes that Surface objects will call \ref Surface::close() when other function is called.
         *
         * @note
         * It also notifiate its delegates for the same function (\ref DriverObserver::onDriverWillClear) and
         * emits notification kDriverDidClearNotification.
         */
        void clearResources();
        
        /*! @brief Returns the number of surfaces present. */
        std::size_t getSurfacesCount() const;
        
        /*! @brief Called right after the Module has updated.
         *
         * Tries to clear the laterReleaseQueue from its resources. Once a resource which has its used flag set
         * is met, the function returns and other resources are not released yet.
         */
        virtual void onModuleDidUpdate(Module* module);
        
        /*! @brief Called right before module terminates.
         *
         * Currently only call \ref clearResources to clear all DriverResource. Notes that if some resources
         * are in use, they will be placed in \ref laterReleaseQueue and will be released next update time.
         * However, as no update will be called because module is terminated, \ref laterReleaseQueue will be
         * destroyed in destructor, thus releasing all DriverResource objects at this time. Hopefully, terminating
         * a module doesn't unload it (with dlclose) so code will not be broken.
         */
        virtual void onModuleWillTerminate(Module*);
        
    protected:
        
        /*! @brief Virtual method to override to create the surface handle.
         *
         * Derived class can use this method to create the custom Surface class depending
         * on given arguments. However, no registration of this object must be made by the
         * derived class.
         *
         * @param[in] width Width of the surface in pixels.
         * @param[in] height Height of the surface in pixels.
         * @param[in] title Title for this Surface, if available.
         * @param[in] objectName Name for this surface.
         * @param[in] style Style used to create the surface, applicable if it is a Window.
         * @param[in] extension A custom structure for platform-dependent configuration.
         *
         * @return a Handle to the newly created surface.
         */
        virtual Handle < Surface > _createSurface(uint32_t width, uint32_t height, const std::string& title, const std::string& objectName, uint32_t style, const void* extension) const;
        
        /*! @brief Locks the data mutex to access resource's map.
         *
         * \ref unlockResources must be called after accessing data to unlock the internal
         * mutex. It can be used as a faster solution than \ref loadSurfaces or \ref loadResources.
         * However, if \ref unlockResources is not called, mutex is never unlocked.
         *
         * @return A reference to the internal resource map.
         */
        std::map < HashedString, Handle < DriverResource > >& lockResources();
        
        /*! @brief Locks the data mutex to access surface's map.
         *
         * \ref unlockResources must be called after accessing data to unlock the internal
         * mutex. It can be used as a faster solution than \ref loadSurfaces or \ref loadResources.
         * However, if \ref unlockResources is not called, mutex is never unlocked.
         *
         * @return A reference to the internal surfaces map.
         */
        std::map < HashedString, Handle < Surface > >& lockSurfaces();
        
        /*! @brief Unlocks the data mutex. */
        void unlockResources();
    };
}

#endif /* Driver_h */
