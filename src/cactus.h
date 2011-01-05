
/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef CACTUS_H
#define CACTUS_H

#include "object.h"
#include "coord_system2.h"
#include "graphics/sprite.h"
#include "ref.h"
#include "physics/geom.h"

class Geom;
class Body;
class World;

class Cactus : public Object, public CoordSystem2, public SpriteEventHandler, public GeomEventHandler {
public:
   Cactus();
   
   // CoordSystem2 ------------------------------------------------
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
   
   void collided(const Ref<Geom>::SharedPtr & with);
   void leftView();

   Ref<Sprite> sprite;
   Ref<Body> body;
   Ref<Geom> geom;

private:
   void increaseHealth(float by);
   
   float health;
};

#endif /* end of include guard: CACTUS_H */
