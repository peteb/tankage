/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "projectile.h"
#include "physics/body.h"

void Projectile::setTransform(const CoordSystemData2 & cs) {
   body->setTransform(cs);
}

CoordSystemData2 Projectile::getTransform() const {
   return body->getTransform();
}
void Projectile::leftView() {
   kill();
}
