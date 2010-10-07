/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "bullet.h"
#include "world.h"

void Bullet::submitComponents(World & world) {
   world.graphics.addSpriteToScene(sprite);
}
