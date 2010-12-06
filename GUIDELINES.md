

Design Guidelines
=================
This document describes the metrics and guidelines followed mostly in the Engine,
but not necessarily in the game itself.
It's a pretty vague document, but it's a work in progress.

Data
----
Data should flow, not be pushed/pulled! Chain consumers using delegates. Remember
that this is what the Ref class is meant for; to plug in adapters and converters
between two objects.


### Example
    sprite->setDelegate(Owning(new Animator(otherSprite)))
       
   
Classes
-------
A class is broken if it fullfills any of the points below:

* It's got a non-descriptive name. It should be easy to tell what a class does
  based on its name.
* It's got more than one major responsibility, more than one reason to change.
* It's got many public member functions--It exposes too much.
* It's got many private member functions--It probably does too much.
* It's dependant on many other classes to work; using it outside the project would
be hard.

Note that "many" is a subjective term and not defined here. It might get defined
later. An exempt from the "many" rules might be member functions that implement
a common interface.

Modules
-------
Don't create a lot of namespaces; their main use is to logically lump implementation
details together.
