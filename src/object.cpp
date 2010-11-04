/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object.h"
#include "object_list.h"
#include <stdexcept>
#include <iostream>

Object::Object() {
   owner = NULL;
}

void Object::addedOwner(ObjectList * owner) {
   if (this->owner)
      throw std::runtime_error("addedOwner: object is already owned!");
      
   this->owner = owner;
}

void Object::kill() {
   if (owner) {
      owner->enqueueRemoval(this);
	  owner = NULL; // TODO: check if enqueueRemoval succeeds, and then remove owner?
	  
	  if (Ref<ObjectEventListener>::SharedPtr lockedListener = eventListener.lock())
		 lockedListener->onObjectDeath(*this); // TODO: in object dtor instead? but maybe not, what about caches
   }
   else {
	  std::cout << "object::kill: no owner" << std::endl;
   }
}

void Object::setEventListener(const Ref<ObjectEventListener> & newListener) {
   this->eventListener = newListener;
}

// Called by the cache or something
void Object::activate() {

}

// Also called by the cache or owner
void Object::deactivate() {

}
