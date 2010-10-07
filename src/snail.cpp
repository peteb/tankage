/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "snail.h"
#include "graphics/gfx_subsystem.h"
#include "world.h"

void Snail::submitComponents(World & world) {
   world.graphics.addSpriteToScene(sprite);
   
}