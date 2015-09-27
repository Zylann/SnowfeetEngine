Update cycle
=============

Define updatable entities
-------------------------

In games, game code often comport an "update" method that is called every frame.
In SnowfeetEngine, the main thread's loop (usually gameplay) is controlled by
an UpdateManager.

To be made updatable, an entity has to call `setUpdatable()` to register itself
to this UpdateManager.

Update order
-------------

By default, the update order is not defined at all.
To define an order, update layers must be defined.

Each update layer has a name, and so can be ordered among others.
When you call `setUpdatable()` without arguments, the layer is `<DefaultUpdate>`, but you can also call `setUpdatable(name)`.
If the layer doesn't exists, it is created after all registered layers.

At the moment, it is possible to pre-define update layers in the main runnable .mod file:

```javascript
    "updateLayers":[
        {"name":"Before"},
        {"name":"<DefaultUpdate>"},
        {"name":"After"}
    ]
```

Note: you can use the name of your class as layer name.

Built-in layers
----------------

At time of writing, the only built-in layer is `<DefaultUpdate>`.
Note: built-in layers will have `<>` in their names.
