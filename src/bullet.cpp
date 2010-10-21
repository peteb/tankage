/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "bullet.h"
#include "world.h"
#include "physics/body.h"

void Bullet::submitComponents(World & world) {
   world.graphics.addSpriteToScene(sprite);
   world.physics.addBody(body);
}

void Bullet::setPosition(const vec2 & pos) {
   body->setPosition(pos);
}

vec2 Bullet::getPosition() const {
   return body->getPosition();
}

void Bullet::setOrientation(const mat2 & orient) {
   body->setOrientation(orient);
}

mat2 Bullet::getOrientation() const {
   return body->getOrientation();
}


void Bullet::leftView() {
   kill();
}