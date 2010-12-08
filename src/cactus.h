
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

//namespace Graphics {class Sprite; }
namespace Physics {class Body; class Geom; }

class World;

class Cactus : public Object, public CoordSystem2, public Graphics::SpriteEventHandler, public Physics::GeomEventHandler {
public:
   // CoordSystem2 ------------------------------------------------
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
   
   void collided(const Ref<Physics::Geom>::SharedPtr & with);
   void leftView();

   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Body> body;
   Ref<Physics::Geom> geom;
};

#endif /* end of include guard: CACTUS_H */
