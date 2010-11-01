/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "body.h"

Physics::Body::Body(Physics::Subsystem & subsystem)
   : subsystem(subsystem)
   , position(vec2::Zero)
   , velocity(vec2::Zero)
{
   
}

void Physics::Body::setDelegate(const Ref<ReferenceFrame2> & newTarget) {
   delegate = newTarget;
}

void Physics::Body::setOwner(const boost::weak_ptr<Object> & owner) {
   this->owner = owner;
}

boost::weak_ptr<Object> Physics::Body::getOwner() const {
   return owner;
}

void Physics::Body::update(float dt) {
   if (Ref<ReferenceFrame2>::SharedPtr target = delegate.lock()) {
      position += velocity * dt;
      target->setPosition(position);  
   }
}

void Physics::Body::setPosition(const vec2 & pos) {
   position = pos;
}

vec2 Physics::Body::getPosition() const {
   return position;
}

void Physics::Body::setOrientation(const mat2 & orient) {
   
}

mat2 Physics::Body::getOrientation() const {
   return mat2::Identity;
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
