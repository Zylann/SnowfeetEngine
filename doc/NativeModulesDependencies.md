Native module dependencies
============================

THIS IS AN OLD, DEPRECATED DOCUMENT.
I keep it in the repo because the explained design could contain ideas
useable in the future, if supporting this feature is really needed.


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

