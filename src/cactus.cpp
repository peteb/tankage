/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "cactus.h"
#include "world.h"
#include "physics/body.h"
#include "bullet.h"

void Cactus::setTransform(const CoordSystemData2 & cs) {
   body->setTransform(cs);
}

CoordSystemData2 Cactus::getTransform() const {
   return body->getTransform();
}


void Cactus::leftView() {
   kill();
}

void Cactus::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
	  if (Ref<Bullet>::SharedPtr bulletOwner = Cast<Bullet>(lockedOwner)) {
		 bulletOwner->kill();
	  }
   }
}

