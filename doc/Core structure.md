Core structure
==============

Quick description
---------------------

The Snowfeet Core is a library that contains the common API used by all modules and most of platform-specific code.
It exposes a set of abstracted functions and classes enabling modules to communicate on a native level without having to depend on each other.
None of the functionality in the core can be removed for a project's needs, it is fixed and must remain robust.
There isn't any rendering engine, physics or gameplay framework here.
The key point is that it is designed to be extended by modules.


Source folders
-----------------

- app
Entry points of the engine. The Application singleton lies here, with modules and script engine management.

- system
Platform-specific API such as threading, windows, events, network...

- asset
Resources database and abstraction classes for most of common asset types used in games.
Asset types and loaders can be extended with modules.

- scene
Entity system defining the base for scene trees and application hierarchy.
Every application has a main scene to load, and that's where everything happens.

- math
Math library including primitives such as vectors, matrices and quaternions

- reflect
Reflection library with cross-assembly in mind. This is where the Object class and macros are defined.

- util
C++ utilities (inheritance patterns, reference counting, logging...)

- json
JSON bindings for engine's primitive types.

- squirrel
Squirrel script binding utilities


The folllowing are not yet officially included in the core, or might be excluded:

- space
Containers for spatial objects (Octree, QuadGrid...)

- pcg
Procedural generation utilities

