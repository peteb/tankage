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
#include "updatable.h"

class Snail;

namespace Graphics {class Sprite; }
namespace Physics {class Geom; class Body; }

class Missile : public Object, public CoordSystem2, public Graphics::SpriteEventHandler, public Updatable {
public:
   Missile();
   
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;

   void update(float dt);   
   void leftView();
   void setFuel(float fuel);
   
   Ref<Snail>::WeakPtr shooter;
   Ref<Snail>::WeakPtr target;
   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Geom> geom;
   Ref<Physics::Body> body;

private:
   float timeSinceUpdate, fuel;
};

#endif // !MISSILE_H
