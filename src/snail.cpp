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

void Snail::submitComponents(World & world) {
   world.graphics.addSpriteToScene(sprite);
   world.scheduler.subscribe(0.0f, logic);
}

void Snail::setPosition(const vec2 & pos) {
   sprite->setPosition(pos);   
}
