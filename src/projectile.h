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

class Geom;
class Body;
class World;
class Snail;

class Projectile : public Object, public CoordSystem2, public Graphics::SpriteEventHandler {
public:
   virtual ~Projectile() {}
   
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
      
   void leftView();

   Ref<Snail>::WeakPtr shooter;
   Ref<Graphics::Sprite> sprite;
   Ref<Body> body;
   Ref<Geom> geom;
};


#endif // !PROJECTILE_H
