/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef POWERUP_H
#define POWERUP_H

#include "ref.h"
#include "object.h"
#include "graphics/sprite.h"
#include "physics/geom.h"

class Body;

// TODO: mixins for leftView? Ie, DestroyOnLeftView

class PowerUp : public Object, public Graphics::SpriteEventHandler, public GeomEventHandler {
public:
  
   void leftView();
   void collided(const Ref<Geom>::SharedPtr & with);
   
   Ref<Graphics::Sprite> sprite;
   Ref<Body> body;
   Ref<Geom> geom;
};

#endif // !POWERUP_H
