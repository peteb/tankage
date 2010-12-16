/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef MISSILE_H
#define MISSILE_H

#include "ref.h"
#include "object.h"
#include "coord_system2.h"
#include "graphics/sprite.h"

class Snail;

namespace Graphics {class Sprite; }
namespace Physics {class Geom; class Body; }

class Missile : public Object, public CoordSystem2, public Graphics::SpriteEventHandler {
public:
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
      
   void leftView();

   Ref<Snail>::WeakPtr shooter;
   Ref<Snail> target;
   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Geom> geom;
   Ref<Physics::Body> body;
};

#endif // !MISSILE_H
