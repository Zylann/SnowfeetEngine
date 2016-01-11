SML
=======

Description
--------------

SML is a human-readable superset of JSON with a few more features and things made optional:
- Quotes in object keys are optional
- '@' is allowed in object keys
- ',' is optional when line breaks are explicit
- New data type: the TypedObject, which is basically an array or object with a string tag.

JSON data can be parsed without modification.
SML is also close enough to JSON to be serialized as is, except for TypedObjects which are usage-dependent.


Integration in the core
------------------------

SML is integrated in the engine's core rather than a library,
essentially because it uses the same Variant class to store values.
This way, it saves the engine from switching between different data structures.


Typed objects
-------------

Some types specific to the engine will be bound to SML, such as vectors, colors, refs or ids,
which were previously saved as JSON objects having two members "@type" and "value".
SML makes this data lighter and involves less hashing.
Also, sometimes TypedObject structures aren't even required because Variant can store additional type information inside its 16 most significant bits (see Variant implementation), so everything is done during parsing.


Example
---------

	#format SML1
	Player {
		position: (1,2,3)
		rotation: (0,0,0,1) // Or can be Quaternion(0,0,0,1)
		color: "#ffaa44"
		children: [
			Gun // A node of type Gun with no value
			Sprite {
				material: {
					shader: "emissive"
					texture: "C:/game/assets/player.png"
					// Or we could write texture: fullpath("...")
				}
			}
		]
	}
