Projects
===========

A project is a set of assets, scripts and configurations that make up a framework or an application.
Basically, if you want to make a game, you'll create a new project.

Note: this document explains how projects work by setupping them by hand.
The planned graphical editor will do all that for you, but it's good to know how,
in case things don't work as expected.

Note 2: This document explains a planned design. It may be not fully implemented
at the moment, and will be refreshed as soon as features get operational.
Things are subject to change, if greater ideas emerge. After all, I'm still exploring the design.


Built-in structure
-----------------------

The engine stores projects under directories, called "project roots".

The default directory is the built-in "projects/", which is split into several sub-folders:
- mods: projects defining basic data for modules (name subject to change).
- samples: application projects showing how to do basic things.
- tools: built-in tools (made for the engine, with the engine!)
- apps: this is where you may create your own projects
- usermods: future special folder for user-generated content (actual "mods" from player's point of view).


How to create a project
-----------------------

A project is defined by simply creating a project.json file under one of the existing folders we saw before.
The minimal example for a project file is:

```javascript
	{}
```

That's all. The only presence of the file is enough for the engine to know a directory is a project.

Once the folder becomes a project, any file in it will be interpreted as an asset by the engine.
It's up to you to organize them as you like.


Startup scene
-----------------

Unlike many engines, you don't start an app by implementing a script.
In some cases, you don't even need to code.

Projects can be defined as runnable, by specifying a startup scene.
If you have an asset main.scene, this is what you'll need to add this in your project file:

```javascript
"startupScene":"main"
```

You don't need to append the extension.
(See scene creation docs for more info about creating a scene.)


Dependencies
------------

Projects can depend on others, so the engine will automatically load required projects before.

In your project file, you can add dependencies by writing paths to their folders:

```javascript
"dependencies":{
	"framework/projectA", // path relative to the projects root
	"mods/projectB",
	//etc...
}
```

Modules
---------------------------------------------------------

Your project can also require modules.
Modules are native libraries next to the engine's executable, that can be added or removed according to your needs.  
After making sure you have them, you can require modules like this:

```javascript
"modules":{
	"ModRender",
	"ModAudio",
	//etc...
}
```

Please note that if your project depends on another that already requires modules, then you don't need to add these yourself.

For more information about how modules work, see Modules.md.
