//
//  Driver.cpp
//  RD
//
//  Created by Jacques Tronconi on 22/07/2018.
//

#include "Driver.h"
#include "NotificationCenter.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    Driver::SurfaceHelper::SurfaceHelper(Driver* d) : driver(d)
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Driver::SurfaceHelper::onSurfaceWillClose(const RD::Surface * surface)
    {
        std::lock_guard<std::mutex> lock(driver->mutex);
        
        auto it = std::find_if(driver->surfaces.begin(), driver->surfaces.end(), [surface](const Handle < Surface >& handle){
            if (handle.ptr() == surface)
                return true;
            return false;
        });
        
        if (it != driver->surfaces.end())
        {
            auto itr = std::find_if(driver->resources.begin(), driver->resources.end(), [surface](const Handle < DriverResource >& handle){
                if (handle.ptr() == surface)
                    return true;
                return false;
            });
            
            Handle < DriverResource > handle = itr->second;
            driver->surfaces.erase(it);
            driver->resources.erase(itr);
            
            std::lock_guard<std::mutex> lock2(driver->lrqMutex);
            driver->laterReleaseQueue.push(handle);
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Driver::Driver() noexcept : surfaceHelper(this)
    {
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    Handle < Surface > Driver::createSurface(uint32_t width, uint32_t height, const std::string &title, const std::string &objectName, uint32_t style, const void* extension)
    {
        HashedString id(objectName.data());
        std::lock_guard < std::mutex > lock(mutex);
        
        auto it = surfaces.find(id);
        
        if (it == surfaces.end())
        {
            Handle < Surface > handle = _createSurface(width, height, title, objectName, style, extension);
            
            if (!handle.valid())
            {
                NotifiateAbort("Core",
                               "Driver::CreateSurface",
                               kDriverInvalidSurfaceCreationNotification,
                               "Driver %s can't create surface %s.",
                               name().data(), objectName.data());
            }
            
            resources.insert(std::make_pair(id, handle));
            surfaces.insert(std::make_pair(id, handle));
            handle->addListener(&surfaceHelper);
            
            emit < DriverObserver >(&DriverObserver::onDriverCreatesSurface, this, handle.ptr());
            
            NotificationCenter::Notifiate("Core",
                                          "Driver::CreateSurface",
                                          kDriverSurfaceCreatedNotification,
                                          "Driver %s created surface %s.",
                                          name().data(), objectName.data());
            
            return handle;
        }
        
        return it->second;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::map < HashedString, Handle < Surface > > Driver::loadSurfaces()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return surfaces;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::map < HashedString, Handle < DriverResource > > Driver::loadResources()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return resources;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Driver::clearResources()
    {
        emit < DriverObserver >(&DriverObserver::onDriverWillClear, this);
        
        {
            auto resources2 = loadResources();
            
            for (auto pair : resources2)
            {
                if (pair.second.valid())
                {
                    if (!pair.second->isUsed())
                    {
                        pair.second->onDriverClear();
                    }
                    else
                    {
                        std::lock_guard < std::mutex > lock(lrqMutex);
                        laterReleaseQueue.push(pair.second);
                    }
                }
            }
            
            std::lock_guard < std::mutex > lock(mutex);
            
            resources.clear();
            surfaces.clear();
        }
        
        emit < DriverObserver >(&DriverObserver::onDriverDidClear, this);
        NotificationCenter::Notifiate("Core", "Driver::clearResources", kDriverDidClearNotification,
                                      "Driver %s cleared its resources.", name().data());
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::size_t Driver::getSurfacesCount() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return surfaces.size();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Driver::onModuleDidUpdate(RD::Module *module)
    {
        std::lock_guard < std::mutex > lock(lrqMutex);
        
        while (!laterReleaseQueue.empty())
        {
            auto handle = laterReleaseQueue.front();
            
            if (!handle.valid()) {
                laterReleaseQueue.pop();
                continue;
            }
            
            if (handle->isUsed())
                break;
            
            handle->onDriverClear();
            laterReleaseQueue.pop();
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Driver::onModuleWillTerminate(RD::Module *)
    {
        clearResources();
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::map < HashedString, Handle < Surface > >& Driver::lockSurfaces()
    {
        mutex.lock();
        return surfaces;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    std::map < HashedString, Handle < DriverResource > >& Driver::lockResources()
    {
        mutex.lock();
        return resources;
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    void Driver::unlockResources()
    {
        mutex.unlock();
    }
}
