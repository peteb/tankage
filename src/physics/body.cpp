/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "body.h"

Physics::Body::Body(PhysSubsystem &subsystem)
   : subsystem(subsystem)
   , position(vec2::Zero())
   , velocity(vec2::Zero())
   , orientation(mat2::Identity())
{
   
}

void Physics::Body::setDelegate(const Ref<CoordSystem2> & newTarget) {
   delegate = newTarget;
}

void Physics::Body::setOwner(const Ref<Object>::WeakPtr & owner) {
   this->owner = owner;
}

Ref<Object>::WeakPtr Physics::Body::getOwner() const {
   return owner;
}

void Physics::Body::update(float dt) {
   if (Ref<CoordSystem2>::SharedPtr target = delegate.lock()) {
      position += velocity * dt;
      target->setTransform(CoordSystemData2(position, orientation));  
   }
}

void Physics::Body::setTransform(const CoordSystemData2 & cs) {
   orientation = cs.orientation;
   position = cs.position;
}

CoordSystemData2 Physics::Body::getTransform() const {
   return CoordSystemData2(position, orientation);
}


// void Physics::Body::setVelocity(const vec2 & vel) {
//    velocity = vel;
// }
void Physics::Body::addImpulse(const vec2 & vel) {
	velocity += vel;
}

vec2 Physics::Body::getVelocity() const {
	return velocity;
}
