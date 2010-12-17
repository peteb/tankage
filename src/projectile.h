/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "object.h"
#include "coord_system2.h"
#include "graphics/sprite.h"
#include "ref.h"

//namespace Graphics {class Sprite; }
namespace Physics {class Body; class Geom; }

class World;
class Snail;

class Projectile : public Object, public CoordSystem2, public Graphics::SpriteEventHandler {
public:
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
      
   void leftView();

   Ref<Snail>::WeakPtr shooter;
   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Body> body;
   Ref<Physics::Geom> geom;
};


#endif // !PROJECTILE_H
