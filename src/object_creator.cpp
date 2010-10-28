/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object_creator.h"
#include "bullet.h"
#include "snail.h"
#include <boost/make_shared.hpp>
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

boost::shared_ptr<Bullet> ObjectCreator::createBullet() {
   boost::shared_ptr<Bullet> newBullet = boost::make_shared<Bullet>();

   newBullet->sprite = Owning(world.graphics.createSprite("../data/bullet.png"));
   newBullet->sprite->setEventHandler(newBullet);

   newBullet->body = Owning(world.physics.createBody());
   newBullet->body->setDelegate(newBullet->sprite);
   
   newBullet->geom = Owning(world.physics.createRectGeom(newBullet->sprite->getSize()));
   newBullet->geom->setBody(newBullet->body);
   newBullet->geom->setCollisionId(3);
   newBullet->geom->setCollisionMask(0x0u);
   
   return newBullet;
}

boost::shared_ptr<Snail> ObjectCreator::createSnail(int team, ObjectCreator & creator,
   World & world) 
{
   float xPos = 0.0f;
   if (team == 0)
      xPos = 100.0f;
   else
      xPos = 600.0f;
   
   boost::shared_ptr<Snail> newSnail = boost::make_shared<Snail>();
   
   if (team == 0)
      newSnail->sprite = Owning(world.graphics.createSprite("../data/snail_r.png"));
   else
      newSnail->sprite = Owning(world.graphics.createSprite("../data/snail_l.png"));

   newSnail->physBody = Owning(world.physics.createBody());
   newSnail->physBody->setDelegate(Observing(newSnail->sprite.lock()));
   
   newSnail->logic = Owning(new PlayerEntity(xPos, creator, world));
   newSnail->logic->setTarget(Observing(newSnail->physBody.lock()));
   
   newSnail->physGeom = Owning(world.physics.createRectGeom(newSnail->sprite->getSize()));
   newSnail->physGeom->setRefFrame(Observing(newSnail->sprite.lock()));
   newSnail->physGeom->setEventHandler(Observing(newSnail));
   
   if (team == 0)
      newSnail->physGeom->setCollisionId(1);
   else
      newSnail->physGeom->setCollisionId(0);
   
   newSnail->physGeom->setCollisionMask(0x8u);
   
   return newSnail;
}

boost::shared_ptr<Object> ObjectCreator::createObject(const std::string & type, ObjectCreator & creator) {
	boost::shared_ptr<Object> retval;

	if (type == "snail1") {




	}
	else if (type == "bullet") {


	}
	
	return retval;
}
