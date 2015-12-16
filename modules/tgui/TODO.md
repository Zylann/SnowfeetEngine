TGUI - TODO list
===============

A bit of refacto regarding bounds
---------------------------------

A control has a box layout:
outer bounds, base bounds and inner bounds.
outerBounds is relative to the parent and includes margins.
baseBounds is relative to the parent and is not computed, it's the base rectangle. It's where the hitbox starts.
innerBounds is relative to baseBounds and includes padding. It is used for child positionning.
Relevant getters and setters should be created and old ones refactored.


Orientation support for ListLayout
----------------------------------

ListLayout is currently hardcoded to be vertical from top to bottom. We should be able to set a direction.


Minimal sizes
--------------

Controls should have minimal sizes, and layouts should respect them so the user can't resize them to a point it makes no sense.


Delegate support for compound controls
---------------------------------------

Controls such as TabControl and Slider are actually made of several controls. Those should be customizable, on theme-level or on instance level.
For example, a TabControl could have a tab delegate which creates tabs with text and two buttons for pin and close, with a little script and all.
For a slider, it could be the thumb or even the background.
The technical solution for such delegates is the prefab system built in the engine: we specify a sub-scene which will be instantiated each time we need a tab. (The root would be a Tab)
The same approach could be used with item controls such as listviews (another task for this?).


Rewrite theming system
----------------------

The current theming system involves code repetition from the writing of theme files to the implementations of onDraw in C++.
There should be a theming system similar to what did Qt with CSS. Each control has a box design, so why not borrow some kind of stylesheet?
The idea is also to get the onDraw-related methods out of Controls so it only bothers about logic, and similar code can be factored inside a style renderer. We give it controls and it draws what it can: background, states, animations, text.


Stacking
--------

Controls are primarily Z-ordered with their order inside their parent entity. it's fine for elements in a panel, however, clicking on a window should bring it up. Same for popups.


DockWindows
-----------

One big step to build the editor is to have a working dock system. SplitLayout, TabControl and Docker allows us to build one, but it is static. DockWindows links them together so the user can modify its workspace at will, like Unity3D.
The first iteration on this task doesn't covers multi-window yet, so no native pop-out.
