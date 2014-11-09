Snowfeet Engine
====================

This is an experimental, cross-platform, modular, general purpose game engine.
Its core programming language is C++, and AngelScript for scripting.
I develop it as a hobby, for fun.

It is a spin-off of an earlier version called "Snowfeet Framework",
with a completely new modular structure and integrated scripting.

The engine is in its early stages of development, so lots of code is WIP,
breaking changes may occur, and the main platform I develop on/for is win32 with Visual Studio 2013.
However, the engine is aimed at being cross-platform (desktop or maybe mobile),
so any platform-specific code can go in separate implementation files (no #ifdef of the death please).
Thanks to Premake, it's easy to maintain project files for multiple platforms.

Engine structure
=================

The engine is made of a core and any number of modules.

The core is written in C++ and includes only common functionnality.
Keep in mind that anything present in the core cannot be removed for a game project's needs.
Then, for instance, the core does NOT includes any rendering engine, sound or anything visual,
because you could implement a singleplayer RPG as well as a command-line game server.

Modules are pluggable parts you can get for different projects, depending on your needs.
A game is created as a runnable module.
A module can contain assets, scripts and native code.
C++ code in modules compile into libraries, exposing a common interface for the core to load them.
The core will never directly use native modules, so each native module code must have bindings for the scripting layer.
The graphics engine will be a module, as well as sound, physics or advanced networking.
Modules can have dependencies, but they must be optional as much as possible,
and may only interact on scripting side, not native: 
Take a module, put it into your project, and it should just work.

Scripts are the glue between the core and modules.
Current candidates include AngelScript, Squirrel or maybe the future Javascript Harmony.
The requirements for scripts are the following:
- Scalability: can we build big projects with it?
- Minimal probability of runtime human errors (I'm looking at you, Lua)
- Reasonable performance
- Ability to be hot-reloaded
- Be user-friendly
- Be lightweight

Module ideas:
- mods/framework (utility scripts for game creation)
- mods/freetype (font loading)
- mods/soundfile (sound loading)
- mods/render (graphics engine)
- mods/audio (audio engine)
- mods/enet (binding for ENet, a fast and reliable UDP networking library)
- tools/texteditor (Scintilla)
- tools/editor (tools aggregator for game development)
- tools/exporter (to export a final application)
- samples/... (some test apps and examples)
- games/... (my game... or yours? Not versionned here, heheh.)

Notes
=====

Scripting
---------
Althouth scripting must be easier than writing C++, I believe scripts must be written by decent programmers.
Creative people without good knowledge in programming should use tools adapted for them, not writing code.
I take this into account for the choice of a scripting language.


Web export
----------
For me as a single dev, exporting native code with llvm-emscripten can be a pain and performance killer,
so it's not in my plans to implement support for it at the moment.
Maybe NaCl or web-native-java hacks, but not plain HTML5 yet.


Binaries
--------
There is no pre-build binaries available.
At the moment, the whole engine and its modules must be compiled with the SAME
compiler, no matter which one, for compatibility reasons.
Building your own binaries is the best way to fit your platform,
if it is supported enough by the engine.


How to compile
=============

You will need the following:
- A C++11 compiler
- Premake5 (get the nightly here, it's less than 1mo: [TODO PREMAKE LINK])
- Optionally, an IDE
- Optionally, Git
That's all, the project then should work out of the box.

The premake build system is used to generate IDE solutions or makefiles.
I choosed it because I know it better than Cmake, it is very small,
and it uses a well-known language (Lua) rather than a custom cryptic one,
which makes writing build scripts very easy.

0) Get the engine from its Git repository, or download a zip file from Github

1) Run premake_yourPlatform_yourBuildSystem_yourConfig
2) You should now see your project file at the root of the repo.

Example with Visual Studio 2013:
1) Run premake_win32_vs2013_debug.bat
2) SnowfeetEngine.sln appeared at the root, open it
3) Build the solution

you need to re-run step 1) only if you pulled a new version of the engine,
or if you are unsure of you project files. Otherwise, you can add files
manually from your IDE without any problem, when possible.

Note for VS: don't create files from it, because they'll end up at the wrong place.
drag and drop them instead.

Note for Code::Blocks: premake5 can't generate workspaces yet, but it's coming ;) 

To version your game separately from the engine's git repo, you can use multiple
git repos feature or use another versionning system.
The engine should otherwise support multiple project roots in the future.

For any question or issues, PM me.

License
========

I don't have decided yet which license the engine should have, but you guessed
that the core is free and open-source.

However, modules could have different licenses:

Essential ones such as graphics or sound may have the same license as the core,
also depending on which third-party code they include / modified.

And obviously, because user applications will be modules,
they can be private-source and/or paid if you want to.


