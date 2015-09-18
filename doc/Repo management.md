Repo management
=================

Repositories
--------------

The core and its official modules currently form the "engine", in the same Git repository.
In the far future, sub-repos might be created, however I prefer creating branches at the moment.


Branches
---------

The master branch is dedicated to the core and is somewhat the most "stable".
Fixes to the core or various modules can be made here, but nothing should break.
There is no RC or stable yet because the engine has not reached this stage of distribution.

For each experiment, medium/big features or important modules, a branch should be created for them.
If a fix must be done in the core, it should be made on master, unless it's a breaking change.
The workflow for a core fix is:
* checkout master
* perform the fix
* push master so other can benefit your fix
* checkout back to your branch
* merge master to have your fix in your branch
