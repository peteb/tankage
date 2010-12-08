/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "helmet.h"
#include "physics/body.h"
#include "bullet.h"

Helmet::Helmet() {
   health = 100;
}

void Helmet::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
      lockedOwner->kill();
      health -= 20;

      if (Ref<Physics::Body>::SharedPtr lockedBody = snailBody.lock())
         lockedBody->addImpulse(with->getTransform().orientation * vec2(100.0f, 50.0f));

      if (health <= 0) {
         kill();
      }
   }
}

void Helmet::setTransform(const CoordSystemData2 & cs) {
   body->setTransform(cs);
}

CoordSystemData2 Helmet::getTransform() const {
   return body->getTransform();
}


