

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
       
### Cons

* Indirections and pointer locking when a property changes,
  ref.lock() -> Adapter.setPosition -> ref.lock() -> delegate.setPosition.
  Suggestions; a functor that takes a RefFrame, has some state (coord, orient)
  and returns a new reframe.
  A way of cutting down ref.locks and indirections would be to change setPosition/
  setOrientation into setReferenceFrame. setRefFrame/getRefFrame.
* Object bloat, the converter probably has some state (coord, orient) and a ref
  delegate. The delegating object has a ref delegate. Each ref is at least a shared_ptr
  and a weak_ptr, and maybe even a T*.
   
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
