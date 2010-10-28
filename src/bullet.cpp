/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "bullet.h"
#include "world.h"
#include "physics/body.h"

void Bullet::setPosition(const vec2 & pos) {
   body.lock()->setPosition(pos);
}

vec2 Bullet::getPosition() const {
   return body.lock()->getPosition();
}

void Bullet::setOrientation(const mat2 & orient) {
   body.lock()->setOrientation(orient);
}

mat2 Bullet::getOrientation() const {
   return body.lock()->getOrientation();
}


void Bullet::leftView() {
   kill();
}
