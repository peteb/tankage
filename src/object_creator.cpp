/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object_creator.h"
#include "bullet.h"
#include "snail.h"
#include "cactus.h"

#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"
#include "player_entity.h"
#include "world.h"
#include "physics/body.h"
#include "physics/geom.h"
#include "ref.h"

ObjectCreator::ObjectCreator(World & world)
   : world(world)
{
}

Ref<Bullet>::SharedPtr ObjectCreator::createBullet() {
   Ref<Bullet>::SharedPtr newBullet(new Bullet);

   newBullet->sprite = Owning(world.graphics.createSprite("../data/bullet.png"));
   newBullet->sprite->setEventHandler(newBullet);

   newBullet->body = Owning(world.physics.createBody());
   newBullet->body->setDelegate(newBullet->sprite);
   newBullet->body->setOwner(newBullet);

   const rect bulletSize(32, 8);
   newBullet->geom = Owning(world.physics.createRectGeom(bulletSize));
   newBullet->geom->setBody(newBullet->body);
   newBullet->geom->setCollisionId(3);
   newBullet->geom->setCollisionMask(0x0u);
   
   return newBullet;
}

Ref<Snail>::SharedPtr ObjectCreator::createSnail(int team, ObjectCreator & creator) {
   float xPos = 0.0f;
   if (team == 0)
      xPos = 100.0f;
   else
      xPos = 700.0f;
   
   Ref<Snail>::SharedPtr newSnail(new Snail);
   
   if (team == 0)
      newSnail->sprite = Owning(world.graphics.createSprite("../data/snail_l.png"));
   else
      newSnail->sprite = Owning(world.graphics.createSprite("../data/snail_r.png"));

   newSnail->physBody = Owning(world.physics.createBody());
   newSnail->physBody->setDelegate(Observing(newSnail->sprite.lock()));
   
   newSnail->logic = Owning(new PlayerEntity(xPos, newSnail.get(), creator, world));
   newSnail->logic->setTarget(Observing(newSnail->physBody.lock()));

   newSnail->setEventHandler(Observing(newSnail->logic));
   
   world.scheduler.subscribe(0.0f, newSnail->logic);
   
   const rect snailSize(56, 39);
   newSnail->physGeom = Owning(world.physics.createRectGeom(snailSize));
   newSnail->physGeom->setRefFrame(Observing(newSnail->sprite.lock()));
   newSnail->physGeom->setEventHandler(Observing(newSnail));


   // TODO: this is ugly
   if (team == 1) {
	  newSnail->physGeom->setOffset(vec2(2.0f, -4.0f));
	  newSnail->logic->weaponDir = vec2(-1.0f, 0.0f);
	  newSnail->logic->weaponPos = vec2(-50.0f, 1.0f);
   }
   else { // first snail. not obvious.
	  newSnail->physGeom->setOffset(vec2(2.0f, -5.0f));
	  newSnail->logic->weaponDir = vec2(1.0f, 0.0f);
	  newSnail->logic->weaponPos = vec2(50.0f, 1.0f);
   }
   
   if (team == 0)
      newSnail->physGeom->setCollisionId(1);
   else
      newSnail->physGeom->setCollisionId(0);
   
   newSnail->physGeom->setCollisionMask(0x8u);

   return newSnail;
}

Ref<Object>::SharedPtr ObjectCreator::createObject(const std::string & type, ObjectCreator & creator) {
   Ref<Object>::SharedPtr retval;
   
   if (type == "snail1") {
	  return createSnail(0, creator);
   }
   else if (type == "snail2") {
	  return createSnail(1, creator);
   }
   else if (type == "bullet") {
	  return createBullet();
   }
   else if (type == "cactus") {
	  Ref<Cactus>::SharedPtr newCactus(new Cactus);
	   
	  newCactus->sprite = Owning(world.graphics.createSprite("../data/cactus1.png"));
	  newCactus->sprite->setEventHandler(newCactus);

	  newCactus->body = Owning(world.physics.createBody());
	  newCactus->body->setDelegate(newCactus->sprite);
	  newCactus->body->setOwner(newCactus);

	  const rect cactusSize(18, 34);
	  newCactus->geom = Owning(world.physics.createRectGeom(cactusSize));
	  newCactus->geom->setBody(newCactus->body);
	  newCactus->geom->setCollisionId(4);
	  newCactus->geom->setCollisionMask(0x8u);
	  newCactus->geom->setEventHandler(Observing(newCactus));
	  newCactus->geom->setOffset(vec2(-1.0f, -2.0f));
	  return newCactus;
   }
	

   throw std::runtime_error("failed to create object of type '" + type + "', I don't know what it is!");
}
