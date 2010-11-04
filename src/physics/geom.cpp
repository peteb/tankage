/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "geom.h"
#include "body.h"
Physics::Geom::Geom(const rect & size)
   : size(size)
   , collisionMask(0xFFFFFFFFu)
{
   collisionId = 0;
}

void Physics::Geom::setBody(const Ref<Physics::Body> & body) {
   this->linkedBody = body;
}

void Physics::Geom::setRefFrame(const Ref<ReferenceFrame2> & refFrame) {
   this->refFrame = refFrame;
}

rect Physics::Geom::getSize() const {
   return size;
}

void Physics::Geom::setPosition(const vec2 & newPos) {
   this->position = newPos;
}

vec2 Physics::Geom::getPosition() const {
   return position;
}

void Physics::Geom::setOrientation(const mat2 & orient) {
   
}

mat2 Physics::Geom::getOrientation() const {
   if (Ref<Physics::Body>::SharedPtr lockedBody = linkedBody.lock())
	  return lockedBody->getOrientation();
   
   return mat2::Identity;
}

void Physics::Geom::setCollisionId(unsigned int collisionId) {
   this->collisionId = collisionId;
}

void Physics::Geom::setCollisionMask(const std::bitset<32> & mask) {
   this->collisionMask = mask;
}

void Physics::Geom::setEventHandler(const Ref<GeomEventHandler> & eventHandler) {
	this->eventHandler = eventHandler;
}

boost::weak_ptr<Object> Physics::Geom::getOwner() const {
   if (Ref<Physics::Body>::SharedPtr lockedPtr = linkedBody.lock())
	  return lockedPtr->getOwner();

   throw std::runtime_error("no body linked to geom");
}

void Physics::Geom::collided(const boost::shared_ptr<Geom> & with) {
	if (Ref<GeomEventHandler>::SharedPtr lockedPtr = eventHandler.lock())
		lockedPtr->collided(with);
}

