/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "helmet.h"
#include "physics/body.h"
#include "projectile.h"
#include "missile.h"

Helmet::Helmet() {
   health = 100;
}

void Helmet::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
      if (Ref<Missile>::SharedPtr lockedMissile = Cast<Missile>(with->getOwner().lock())) {
         health -= 20;

         if (Ref<Physics::Body>::SharedPtr lockedBody = snailBody.lock()) {
            //   lockedBody->addImpulse(with->getTransform().orientation * vec2(300.0f, 50.0f));
            const vec2 delta = getTransform().position - lockedBody->getTransform().position;
			lockedBody->addImpulse(delta * -5.0f);
         }
         
         if (Ref<Physics::Body>::SharedPtr lockedBody = with->linkedBody.lock()) {
            lockedBody->addImpulse(lockedBody->getVelocity() * vec2(-0.3f, -1.4f));

            vec2 vel = lockedBody->getVelocity();
            vel.normalize();
            mat2 spriteOrient(vel, vec2(-vel.y, vel.x));
            lockedBody->setTransform(CoordSystemData2(lockedBody->getTransform().position, spriteOrient));
         }

         lockedMissile->setFuel(0.001f);
      } else if (Cast<Projectile>(with->getOwner().lock())) {
         lockedOwner->kill();
         health -= 20;
         
         if (Ref<Physics::Body>::SharedPtr lockedBody = snailBody.lock())
            lockedBody->addImpulse(with->getTransform().orientation * vec2(100.0f, 50.0f));
         
      }

      
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


