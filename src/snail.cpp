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

Snail::Snail() {
   health = 100;
}

void Snail::setPosition(const vec2 & pos) {
   physBody.lock()->setPosition(pos);   
}

#include <iostream>
void Snail::collided(const Ref<Physics::Geom>::SharedPtr & with) {
   // TODO: this is ugly
   std::cout << "snail collided" << std::endl;
   if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
	  if (Ref<Bullet>::SharedPtr bulletOwner = Cast<Bullet>(lockedOwner)) {
		 if (static_cast<void *>(bulletOwner->shooter) != static_cast<void *>(this)) { // hit by a bullet
			bulletOwner->kill();
			increaseHealth(-5);
			physBody->addImpulse(with->getOrientation() * vec2(300.0f, 0.0f));
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
   }

}
