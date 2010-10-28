/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object.h"
#include "object_list.h"
#include <stdexcept>

Object::Object() {
   owner = NULL;
}

void Object::addedOwner(ObjectList * owner) {
   if (this->owner)
      throw std::runtime_error("addedOwner: object is already owned!");
      
   this->owner = owner;
}

void Object::kill() {
   if (owner)
      owner->enqueueRemoval(this);
}

// Called by the cache or something
void Object::activate() {

}

// Also called by the cache or owner
void Object::deactivate() {

}
