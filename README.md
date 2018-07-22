# RD
Resource's Descriptor Engine

RD is another attempt to make a Game Engine from scratch. It uses C++17 but not all features (like
experimentals, which are not available everywhere yet). RD is based on some simple design choice:

- *Application* object is the main object of everything. It is instanciated by the main program, then started, runned and terminated. It is responsible for loading and updating (and terminating) modules.
- *Modules* are loadable chunk of code that implements one or more classes. One implementation is available for one Module and one Class. For example, a Module can define only one Driver. 
- *Handles* are shared pointer for all objects that may be shared in ownership. Non-owned objects are given as raw pointers and should never be deleted as-it. Handles manages their object with a custom Allocator, which register into Details::AllocationTracker every Allocation and Deallocation it does. This makes memory tracking available easily.
- *Resources* are basically Handles but with some loading/unloading facilities. A Resource is associated to a ResourcePool, which can request the Resource to be unloaded if not used for a certain time, or if memory is requested but not available for this pool. Reloading of the Resource is done by using the same ResourceLoader. A ResourceLoader is registered into a ResourceLoaderDatabase by Modules, or by the main program.

_Exceptions_ are used to handle critical errors. However, Notifications can also be launched for other non
critical errors, or informations. NotificationObservers answers those notifications and can request the 
module to abort its execution, which may result to an Exception throw. Main program should encapsulate 
Application into a try/catch block.
