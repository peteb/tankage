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
#include "ref.h"

ObjectCreator::ObjectCreator(World & world)
   : world(world)
{
}

boost::shared_ptr<Bullet> ObjectCreator::createBullet() {
   boost::shared_ptr<Bullet> newBullet = boost::make_shared<Bullet>();
   newBullet->sprite = world.graphics.createSprite("../data/snail2.png");
   newBullet->sprite->setEventHandler(newBullet);
   newBullet->body = world.physics.createBody();
   newBullet->body->setDelegate(newBullet->sprite);
   
   return newBullet;
}

boost::shared_ptr<Snail> ObjectCreator::createSnail(float x, ObjectCreator & creator, World & world) {
   boost::shared_ptr<Snail> newSnail = boost::make_shared<Snail>();
   newSnail->sprite = world.graphics.createSprite("../data/snail2.png");
   newSnail->logic = boost::shared_ptr<PlayerEntity>(new PlayerEntity(x, creator, world));
   newSnail->logic->setTarget(Sp2Wr(newSnail->sprite));
   
   return newSnail;
}