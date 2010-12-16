/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "missile.h"
#include "physics/body.h"

void Missile::setTransform(const CoordSystemData2 & cs) {
   body->setTransform(cs);
}

CoordSystemData2 Missile::getTransform() const {
   return body->getTransform();
}

void Missile::leftView() {
   kill();
}
