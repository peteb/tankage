/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef ROCKET_AMMO_H
#define ROCKET_AMMO_H

#include "ref.h"
#include "object.h"
#include "graphics/sprite.h"
#include "physics/geom.h"

class Body;

class RocketAmmo : public Object, public SpriteEventHandler, public GeomEventHandler {
public:
  
   void leftView();
   void collided(const Ref<Geom>::SharedPtr & with);
   
   Ref<Sprite> sprite;
   Ref<Body> body;
   Ref<Geom> geom;
};

#endif // !ROCKET_AMMO_H
