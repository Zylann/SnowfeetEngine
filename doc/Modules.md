Modules
==========

Modules are native libraries extending the engine.
They can add bindings to scripts, give access to low-level stuff or third-party C++ libraries. You can even develop your game entirely in C++, and let scripting only for modding.

To use a module in your project, see the related section in Projects.md.


How to create a module
-----------------------

Here is a quick summary of things to do to create a native binding:


1) Create your C++ project folder under the modules/ directory.

2) Configure your project to depend on SnowfeetCore, and output the binary next to the engine's executable. You can use a premake5.lua file to generate your IDE project or makefile, depending on your target platform. You can take model on existing modules to know what to put in.

3) Create two C++ files, one .h and one .cpp, defining two C functions:

```cpp
extern "C"
{
	// Called after your library is loaded
	SN_MOD_EXPORT int loadSnowfeetModule_MyLibrary(ModuleLoadArgs args);

	// Called before your library is unloaded
	SN_MOD_EXPORT int unloadSnowfeetModule_MyLibrary(ModuleUnloadArgs args);
}
```

The args parameter gives you access to various systems such as the script engine, reflection API and maybe future options.

4) Run premake to generate your project: now you should be able to compile and get your native lib.

Once it's done, you can start adding anything you want in your library.

Tip: if you have Python on your system, you can use the create_module.py script located under the tools/ folder.

Conventions
------------

These are important rules to follow when making a native module:

- To allow static builds, you HAVE to use a namespace that is NOT already in use in the engine and other modules.
- Try to be as cross-platform as you can, use the engine's system API.
- Avoid global variables of any sort the best you can (only constants are fine). You can also use the entity system and create a singleton entity.
- Use the same compiler used by other modules for your platform. If you compiled the engine yourself it should be fine (that's because of STL versions).
- Don't forget to register your reflectable/scriptable classes in the engine! See existing modules for examples.


Native dependencies
---------------------

It's possible to make a module depend on others.  
In order to do that, you can use the premake commands "dependson", "includedirs" and "link" just like any other libraries.

Because loading a module might trigger others to be loaded by the system, all modules are next to the executable to simplify the process.
In dynamic builds, When the engine itself asks to load a module, the system will first check if the library is alreay loaded, loads it if needed, and returns the handle accordingly.


Static builds
--------------------

TODO
