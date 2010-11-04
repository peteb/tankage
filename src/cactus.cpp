/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "cactus.h"
#include "world.h"
#include "physics/body.h"
#include "bullet.h"

void Cactus::setPosition(const vec2 & pos) {
   body.lock()->setPosition(pos);
}

vec2 Cactus::getPosition() const {
   return body.lock()->getPosition();
}

void Cactus::setOrientation(const mat2 & orient) {
   body.lock()->setOrientation(orient);
}

mat2 Cactus::getOrientation() const {
   return body.lock()->getOrientation();
}

void Cactus::leftView() {
   kill();
}

void Cactus::collided(const boost::shared_ptr<Physics::Geom> & with) {
   if (boost::shared_ptr<Object> lockedOwner = with->getOwner().lock()) {
	  if (boost::shared_ptr<Bullet> bulletOwner = boost::dynamic_pointer_cast<Bullet>(lockedOwner)) {
		 bulletOwner->kill();
	  }
   }
}

