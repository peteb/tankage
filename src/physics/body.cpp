/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "body.h"

Body::Body(PhysSubsystem &subsystem)
  : subsystem(subsystem)
  , position(vec2::Zero())
  , velocity(vec2::Zero())
  , orientation(mat2::Identity())
{
   
}

void Body::setDelegate(const Ref<CoordSystem2> & newTarget) {
  delegate = newTarget;
}

void Body::setOwner(const Ref<Object>::WeakPtr & owner) {
  this->owner = owner;
}

Ref<Object>::WeakPtr Body::getOwner() const {
  return owner;
}

void Body::update(float dt) {
  if (Ref<CoordSystem2>::SharedPtr target = delegate.lock()) {
    position += velocity * dt;
    target->setTransform(CoordSystemData2(position, orientation));  
  }
}

void Body::setTransform(const CoordSystemData2 & cs) {
  orientation = cs.orientation;
  position = cs.position;
}

CoordSystemData2 Body::getTransform() const {
  return CoordSystemData2(position, orientation);
}


// void Body::setVelocity(const vec2 & vel) {
//    velocity = vel;
// }
void Body::addImpulse(const vec2 & vel) {
  velocity += vel;
}

vec2 Body::getVelocity() const {
  return velocity;
}
