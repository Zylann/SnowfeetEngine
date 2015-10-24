Scenes
=====================

Description
-----------

One of the key features of the engine is its ability to define games as data.
In order to do this, the entry point of an executable module is a scene.

A scene is a file containing the serialized form of various entities, script states
and other objects that compose a game's scene tree, a bit like Unity3D or Flash.

To accomplish this, the state of all C++ entities and a lot of other objects in the engine are serializable as key/value objects, which can be stored in databases for later re-use.
The reflection system defined in the core helps a lot in this task.


Scene composition with sub-scenes
----------------------------------

The engine allows scenes to be nested. If you need to reuse a pattern, this is the way to go.
Scene instances are basically references to another scene file, and behave like Unity3D's prefabs,
except they can be nested at will.
Scene instances can also have a few modifications that are specific to the instance,
with a few limitations: only property changes are supported. Adding or removing
entities might be possible in theory, but hasn't been tested and can become hard to debug,
so there is no development effort in that direction.

There is no difference between a scene or a sub-scene, both are .scene files.
Maybe, the only point of interest is that some of them may define a root object,
which is sometimes a requirement for specific modules.


Loading
-------

The engine fully supports the loading of scenes, because otherwise there wouldn't be much to see yet, right :)
What the scene loader does is basically parsing all the objects in the scene file.
Then, when a scene needs to be instantiated, data is flattened so all sub-scenes are calculated and ready to be created in the tree.

It is done in several passes:
1. If not done already, the data is processed.
2. All objects are allocated, so they have an address in memory we can map on IDs
3. Entities-only: the hierarchy is rebuilt thanks to IDs
4. States are deserialized (properties).
5. The onReady() callback is called on all entities (like Unity3D's Awake() event).

[See sn::PackedEntity for more details.](../core/scene/PackedEntity.h)


Saving
--------

Saving is not fully implemented, and might require additional development on the entity system.
It has been delayed because it's linked to the official Editor's development, and also
because scenes can already be created by hand (the format is human-readable).
Not pretty, but the time will come this will be as easy as right-click/create :)


Scene format
--------------

The current file format is decoupled in two layers:
* The transport format
* The structure format

The transport format is the way data is stored in a file, and is not directly how the engine expects it. We currently use JSON, but it will be tweaked in the future
to become SML (see docs), which adds a few handy features.

The structure format is the actual form the engine expects to find in the file, after parsing and processing.
Here is an example (JS so I can put comments):

```javascript
{
	// Must be present as a header so the engine knows what this JSON file is
	"format":"SN2",

	// An optional property defining which is the root object in the file.
	// in case of sub-scenes, it might be the root entity.
	"root":111,

	// Then, a list of objects indexed by ID, much like an actual database.
	"objects":[
		111, {
			"@type":"tgui::Panel",
			"bounds":{
				"x":10,
				"y":10,
				"w":200,
				"h":100
			},
			"_children":[
				// Children and references are specified like this,
				// so the engine can recognize them directly in the data
				// (necessary to the instancing process).
				{"@ref":222},
				{"@ref":333}
			]
		},

		222, {
			"@type":"tgui::Text",
			"text":"Hello world!"
		},

		// Here is a sub-scene instance.
		// This one will be processed during the instancing process by the
		// ObjectDB system, so C++ objects can access final values without
		// having to know they are sub-scenes.
		333, {
			"@src":"mymodule:myprefab",
			// With a few modifications specific to the instance
			"changes":[
				// <ID in myprefab>, <property path>, <new value>
				100, "color", [255,255,0],
				101, "name", "overrided name"
			]
		},

		// etc...
	]
}
```

Please note that this format is not specific to scenes:
it can be used with any object that can be saved like that, so we can imagine
making prefabs with materials, configuration files...

- [See sn::ObjectDB for more details.](../core/asset/ObjectDB.h)
- [See more complex scene file](../projects/samples/tguitest/main.scene)
- [See SML JSON-superset format](../doc/SML.md)

Legacy format
-------------

There is a more straightforward legacy format that was used in the early days of the engine.
However, it is now deprecated because it doesn't supports references, sub-scenes, and was too focused on entities.
A built-in converter transform those old scenes when they are loaded,
so they can still be used. However, it may disappear in the future.

This is what it looked like:

```javascript
{
	"format":"SN1",
	"entities":[
		// List of entities
		{
			"@type":"tgui::Panel",
			"_children":[
				// Children were directly specified, making sub-scenes and references hard to support.
				{
					"@type":"tgui::Text",
					//...
				}
			]
		},
		{
			//...
		}
	]
}
```
