![Snowfeet Engine](http://zylannprods.fr/snowfeet/engine/artwork/snowfeet_engine_title_alpha_w800_v2.png)
====================

This is an experimental, cross-platform, modular, general purpose game engine.
Its core programming language is C++, and Squirrel for scripting.
I develop it as a hobby, for fun.

It is a spin-off of an earlier version called "Snowfeet Framework",
with a completely new modular structure and integrated scripting.

The engine is in its early stages of development, so lots of code is WIP,
breaking changes may occur, and the main platform I develop on/for is win32 with Visual Studio 2013.
However, the engine is aimed at being cross-platform (desktop or maybe mobile),
so any platform-specific code can go in separate implementation files.

Engine structure
=================

The engine is made of a core and any number of modules.
The core is written in C++ and includes only common functionnality.
Modules are dynamic and you can adjust what you need for any project.

For more info about how modules work, see documentation files.


Notes
=====

What the engine does now
------------------------

![Screenshot](http://zylannprods.fr/snowfeet/engine/screenshots/2015_04_06_2031_oculus6_lights_black.png)
Testing Oculus Rift client-mode display

The current focus is basic rendering, scripting and assets management.
Things like texture and shaders live-edition are in the works too :)


Web export
----------
I might try Emscripten, one day. Sounds like fun.


Binaries
--------
There is no pre-build binaries available.
At the moment, the whole engine and its modules must be compiled with the SAME
compiler, no matter which one, for compatibility reasons.
Building your own binaries is the best way to fit your platform,
if it is supported enough by the engine.


This is not a library
----------------------

Althought there are exports and shared libraries, this project is not aimed at being a library.
It's actually an "end-user" framework, based on simple libraries, just as actual applications.
That's why headers and cpp files aren't separated and why everything is self-contained as much as possible
(rather than dependencies supposedly setup in your build environnment).


How to compile
=============


The whole engine is entirely provided as source code.

You will need the following to compile everything:
- A C++11 compiler
- Premake5 (See bottom of this README)
- Optionally, an IDE
- Optionally, Git

That's all, the project then should work out of the box.

The premake build system is used to generate IDE solutions or makefiles.
I choosed it because I know it better than Cmake, it is very small,
and it uses a well-known language (Lua), which makes writing build scripts very easy.

All you have to do to obtain working project files is to run one of the
scripts named "premake_xxx".
For Windows, the one I mostly use is premake_vs2013_debug.bat.

Another thing to keep in mind:
Never manage files and build options from IDEs, because sometimes they end up in the wrong place.
You should create them by hand and re-run premake to update project files. 

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


Premake5
========

You can get the source from the premake-dev repo and build it yourself:
https://bitbucket.org/premake/premake-dev/wiki/Building_Premake

Or use a windows executable I built:
http://zylannprods.fr/dl/premake5.exe

There are also nightlies on SourceForge:
http://sourceforge.net/projects/premake/files/Premake/nightlies/
