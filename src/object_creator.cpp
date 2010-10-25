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
   
   return newBullet;
}

boost::shared_ptr<Snail> ObjectCreator::createSnail(float x, ObjectCreator & creator,
   World & world) 
{
   boost::shared_ptr<Snail> newSnail = boost::make_shared<Snail>();
   
   newSnail->sprite = Owning(world.graphics.createSprite("../data/snail_r.png"));

   newSnail->logic = Owning(new PlayerEntity(x, creator, world));
   newSnail->logic->setTarget(Observing(newSnail->sprite.lock()));
   
   newSnail->physGeom = Owning(world.physics.createRectGeom(newSnail->sprite->getSize()));
   newSnail->physGeom->setRefFrame(Observing(newSnail->sprite.lock()));
   
   return newSnail;
}