/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "snail.h"
#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"
#include "world.h"
#include "player_entity.h"
#include "bullet.h"
#include "helmet.h"

#include <iostream>

Snail::Snail() {
   health = 100;
}

// TODO: fix event system.

void Snail::setTransform(const CoordSystemData2 & cs) {
   physBody->setTransform(cs);
}

CoordSystemData2 Snail::getTransform() const {
   return physBody->getTransform();
}


void Snail::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   // TODO: this is ugly
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
	  if (Ref<Bullet>::SharedPtr bulletOwner = Cast<Bullet>(lockedOwner)) {
		 if (static_cast<void *>(bulletOwner->shooter) != static_cast<void *>(this)) { // hit by a bullet
			bulletOwner->kill();
			increaseHealth(-5);
            float slump = static_cast<float>(2 - rand() % 4) / 2.0f;
            const vec2 delta = getTransform().position - bulletOwner->getTransform().position;
			physBody->addImpulse(delta * 8.0f); //vec2(300.0f, slump * 100.0f));
		 }
	  }
   }
   
}

void Snail::setEventHandler(const Ref<SnailEventHandler> & newHandler) {
   eventHandler = newHandler;
}

void Snail::increaseHealth(int add) {
   health += add;

   if (Ref<SnailEventHandler>::SharedPtr lockedHandler = eventHandler.lock())
	  lockedHandler->onHealthChange(static_cast<float>(health));
   
   std::cout << "decreased health to " << health << std::endl;

   if (health <= 0.0f) {
	  std::cout << "snail died" << std::endl;
	  kill();
      if (Ref<Helmet>::SharedPtr lockedHelmet = helmet.lock())
         lockedHelmet->kill();
   }

}
