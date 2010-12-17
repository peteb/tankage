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
   , fuel(1.4f)
{
}

void Missile::setFuel(float fuel) {
   this->fuel = fuel;
}

void Missile::update(float dt) {
   timeSinceUpdate += dt;
   fuel -= dt;
   
   if (timeSinceUpdate >= 0.01f && fuel > 0.0f) {
      // TODO: fix this ugly way of handling target seeking!
      
      if (Ref<Snail>::SharedPtr lockedTarget = target.lock()) {
         vec2 vel = body->getVelocity();
         vec2 uvel = vel;
         uvel.normalize();
         vec2 diff = lockedTarget->getTransform().position - body->getTransform().position;
         vec2 udiff = diff;
         udiff.normalize();

         vec2 impulse = (udiff - uvel * 1.0f) * 200.0f + uvel * 40.0f;
         
         body->addImpulse(impulse * 200.0f * dt); //300.0f);
         vel = body->getVelocity();
         vel.normalize();
         
         mat2 spriteOrient(vel, vec2(-vel.y, vel.x));
         body->setTransform(CoordSystemData2(body->getTransform().position, spriteOrient));
      }
      
      timeSinceUpdate = 0.0f;
   }
}

