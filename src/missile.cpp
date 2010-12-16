/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "missile.h"
#include "physics/body.h"
#include "snail.h"

Missile::Missile()
   : timeSinceUpdate(0.0f)
   , fuel(2.0f)
{
}

void Missile::setTransform(const CoordSystemData2 & cs) {
   body->setTransform(cs);
}

CoordSystemData2 Missile::getTransform() const {
   return body->getTransform();
}

void Missile::leftView() {
   kill();
}

void Missile::update(float dt) {
   timeSinceUpdate += dt;
   fuel -= dt;
   
   if (timeSinceUpdate >= 0.1f && fuel > 0.0f) {
      // TODO: fix this ugly way of handling target seeking!
      
      if (Ref<Snail>::SharedPtr lockedTarget = target.lock()) {
         vec2 vel = body->getVelocity();
         vec2 uvel = vel;
         uvel.normalize();
         vec2 diff = lockedTarget->getTransform().position - body->getTransform().position;
         vec2 udiff = diff;
         udiff.normalize();

         vec2 impulse = (udiff - uvel * 0.9f) * 200.0f + uvel * 30.0f;
         
         body->addImpulse(impulse); //300.0f);
         vel = body->getVelocity();
         vel.normalize();
         
         mat2 spriteOrient(vel, vec2(-vel.y, vel.x));
         
         body->setTransform(CoordSystemData2(body->getTransform().position, spriteOrient));
      }
      
      timeSinceUpdate = 0.0f;
   }
}

