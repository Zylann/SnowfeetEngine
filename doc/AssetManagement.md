Asset management
==================

What is an asset
-----------------

Assets are every resource that your application might load (or save?) from the file system during its execution.
The engine centralizes all assets into a singleton, AssetDatabase.

The engine provides the Asset class as a base for all assets.
It's a container that is empty upon instantiate, can load its data, save or unload it.
This way, the asset can change and still be referenced by parts of the application that need it.
For instance, if the rendering engine references a texture, it can be reloaded on runtime
without re-creating the instance.


How to define assets types
---------------------------

Assets definition is usually done in modules.
You can derive from Asset directly and implement its interface,
or from generic types defined in the core.

The reason why the core defines generics is because it allows modules to
interoperate more easily on native level, without having to depend against each other.

For each asset type, you have to define a loader for it.
Loaders must derive from AssetLoader, and implement every of the required virtual functions.
Loaders defined in modules get automatically referenced with the reflection system.
TODO: this might become manual in the future for practical reasons.


How assets are loaded
----------------------

When a file has to be loaded as an asset, the following steps are executed:

1) On indexing phase, the database iterates over all loaders.
   If one match, it is choosed to create the instance for later loading.
   If two loaders match, isDirect() is called to check if it has priority over the other.
   Otherwise, the loading process is ambiguous and the file is not loaded.


2) On loading phase, the database calls the loadFromStream() method on the appropriate loader.
   It's important to note assets are not created here, they are always and only updated.


Hot-reload
-----------

Hot-reloading is achieved by design.
Every asset the application references has an existing instance in the AssetDatabase.
Only a subset of them may be loaded at a time.
If assets need to be reloaded, they are not destroyed, only updated:
the loader gets called a second time on the same asset, and here you go :)
