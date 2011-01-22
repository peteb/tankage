Design Guidelines
=================
This document describes the metrics and guidelines followed mostly in the Engine,
but not necessarily in the game itself.
It's a pretty vague document, but it's a work in progress.

File structure
----------
/utils/ -- for utility classes, like vectors, matrices, interpolation functions, ...
/engine/ -- contains the different modules and classes that make up the engine. a libengine is compiled for the current platform, which contains the required modules
/game/ -- uses the libengine and contains the game itself (entry point is app_main).
   
Commenting
----------
Files that are part of the core/the engine should be fully commented with
file header, class header, members, etc.


Classes
-------
A class should strive to be self-sufficient; it shouldn't rely on external
procedures to get created. The constructor should be enough. Additional data
should be supplied using member functions.

A class is broken if it fullfills any of the points below:

* It's got a non-descriptive name. It should be easy to tell what a class does
  based on its name.
* It's got more than one major responsibility, more than one reason to change.
* It's got many public member functions--It exposes too much.
* It's dependant on many other classes to work; using it outside the project would
be hard.

Note that "many" is a subjective term and not defined here. It might get defined
later. An exempt from the "many" rules might be member functions that implement
a common interface.

Modules
-------
Don't create a lot of namespaces; their main use is to logically lump implementation
details together.
