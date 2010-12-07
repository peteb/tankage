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
         lockedBody->addImpulse(with->getOrientation() * vec2(100.0f, 50.0f));

      if (health <= 0) {
         kill();
      }
   }
}

vec2 Helmet::getPosition() const {
   return body->getPosition();
}

void Helmet::setPosition(const vec2 & newPos) {
   body->setPosition(newPos);
}

mat2 Helmet::getOrientation() const {
   return body->getOrientation();
}

void Helmet::setOrientation(const mat2 & newOrientation) {

}

