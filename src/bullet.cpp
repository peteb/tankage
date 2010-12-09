/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "bullet.h"
//#include "world.h"
#include "physics/body.h"

void Bullet::setTransform(const CoordSystemData2 & cs) {
   body->setTransform(cs);
}

CoordSystemData2 Bullet::getTransform() const {
   return body->getTransform();
}
void Bullet::leftView() {
   kill();
}
