Modules
===========

Note: this document explains how modules work by setupping them by hand.
The planned graphical editor will do all that for you, but it's good to know how,
in case things don't work as expected.

Note 2: This document explains a planned design. It may be not fully implemented
at the moment, and will be refreshed as soon as features are operational.
Things are subject to change, if greater ideas emerge. After all, I'm still lrearning.


Built-in structure
-----------------------

The engine is made of an unchangeable core and any number of removable modules.
They can be found under a global root directory, defined on launch time
by the command line.

The default directory is the built-in "projects/", which is split into several sub-folders:
- mods: core modules, not runnable.
- samples: runnable modules showing how to do basic things.
- tools: built-in tools (made for the engine, with the engine!)
- games: the actual projects you may work on!
- usermods: future special folder for user-generated content (actual "mods" from player's point of view).


How to define a module
-----------------------

A module is defined by simply creating a .mod file under one of the existing folders we saw before.
.mod files are actually JSON files, in which you can write required metadata the engine will interpret.
The minimal example for a .mod file is:

	{}

That's all. The only presence of the file is enough for the engine.


What modules provide
----------------------

Each module adds new features to the engine, in various ways.
They can contain the following:

- Assets (images, sounds, shaders...)
- Scripts (Squirrel, AngelScript, Lua...)
- C++ with script bindings (new Entities, CPU-intensive functions, low-level access...)
- C++ without bindings (Generally useless, but possible)

Exception: modules containing native code must put their binaries into the bin/ folder, next to the engine.
For more info about this, see below.

Runnable modules
-----------------

Unlike many engines, you don't start an app by implementing a script.
In some cases, you don't even need to code.

Modules can be defined as runnable, by specifying a startup scene.
Obviously, this is what you'll search for if you're creating a game, or visual tool:

	{
		"startupscene":"main.scene"
	}

(See scene creation docs for more info about creating a scene.)


Dependencies
------------

Modules can depend on others, then the engine will automatically load required modules before.
This is straightforward if they contains only assets, scripts or dependence-free C++ bindings.

In your .mod file, declare the following:

	"dependencies":{
		"mods/moduleA", // path relative to the projects root
		"mods/moduleB",
		//etc...
	}


Native modules (or the beginning of complicated things)
---------------------------------------------------------

I labelled this section like this because it targets programmers with good skills with C/C++.

Modules can contain native libraries in C++, that can add bindings to scripts and other native stuff.

Here is a quick summary of things to do to create a native binding:

1) Declare it in your .mod file

	{
		"bindings"[
			"MyLibrary" // This will be the name of your .DLL/.SO/...
		]
	}

2) Create a cpp/ folder in which you put a premake5.lua file.
This will allow you to generate your IDE project or makefile, depending on your target platform.
You can take model on existing modules to know what to put in.

3) Create two C++ files, one .h and one .cpp, defining two C functions:

	extern "C"
	{
		// Called after your library is loaded
		SN_MOD_EXPORT int loadSnowfeetModule_MyLibrary(ModuleLoadArgs args);

		// Called before your library is unloaded
		SN_MOD_EXPORT int unloadSnowfeetModule_MyLibrary(ModuleUnloadArgs args);
	}

The args parameter gives you access to various systems such as the script engine,
reflection API and maybe future options.

4) Run premake to generate your project: now you should be able to compile and get your native lib.

Once it's done, you can start adding anything you want in your library, with the following conventions:
- You HAVE to use a namespace that is NOT already in use in the engine and other modules
- NEVER rely on global variables of any sort. If you still need that, use the entity system and create a singleton entity.
- Use the same compiler used by other modules for your platform. If you compiled the engine yourself it should be fine.
- Don't forget to register your reflectable/scriptable classes in the engine! See existing modules for examples.


Native dependencies
---------------------

Dependencies are bad.
Dependencies canceal your freedom.

But even in a modular engine, at some point, you can't avoid them.

So I made the terrible choice to allow C++ modules to have some, if they need.
I allowed that because it's possible, if great care if given. Not because it's a good choice.
You can program your project in full C++ and only expose scripts for a modding API, if you want.
But still, keep in mind that the less dependencies you create, the better.

In SnowfeetEngine, all native code is compiled into libraries, under the bin folder, next to the runnable binary.
This way, you can setup your C++ project to use another module like any library,
and it will load just fine when the engine loads it because binaries are at the same place.

Native dependencies are specified differently in the .mod file:

	{
		"nativedependencies":{
			"mods/moduleA",
			"mods/moduleB",
			//etc...
		}
		// DON'T use "dependencies" for them
	}

This is a way to tell the engine not to preload dependencies, because YOU will have to do it:

First you obviously don't need to load libraries by hand, because you already link at compile time,
the system does that for us.

Second, you have to call yourself the native entry points of each modules you depend on:

	// Put this into your loadSnowfeetModule_MyModule
	sn::Application::get().loadStaticModule("moduleA", &loadSnowfeetModule_moduleA);
	sn::Application::get().loadStaticModule("moduleB", &loadSnowfeetModule_moduleA);
	//...

	// And this into your unloadSnowfeetModule_MyModule
	sn::Application::get().unloadStaticModule("moduleA", &loadSnowfeetModule_moduleA);
	sn::Application::get().unloadStaticModule("moduleB", &loadSnowfeetModule_moduleA);
	//...


Mistakes to avoid
--------------------

In short: always produce forward, tree dependencies. Keep it simple.
We're not responsible for loss of matter caused by dependencies abuse, especially native.


The engine executes a first pass to determine all kind of dependencies BEFORE anything is loaded.
If an unsupported or bad design is detected, it will produce meaningful error messages,
as long as .mod files and native code are setup correctly.

Let's say you have three standalone modules, A, B and C.
Then, you have a module D, which is natively required by C.
What happens if A is defined like this?

	{
		"dependencies":[
			"B", "C", "D"
		]
	}

An error will be raised, because you are trying to load D, that is already
natively loaded by C. The engine knows this because of the contents of C.mod.

Again, another typical example:

What if A depdends on B, B depends on C, and C depends on A?
This is both a terrible design mistake and an error that the engine will report as well.
Just don't do that. Keep dependencies as a forward tree.

