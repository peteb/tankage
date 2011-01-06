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

namespace Physics {class Body; }


class RocketAmmo : public Object, public Graphics::SpriteEventHandler, public Physics::GeomEventHandler {
public:
  
   void leftView();
   void collided(const Ref<Physics::Geom>::SharedPtr & with);
   
   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Body> body;
   Ref<Physics::Geom> geom;
};

#endif // !ROCKET_AMMO_H
