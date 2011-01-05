/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef HELMET_H
#define HELMET_H

#include "object.h"
#include "physics/geom.h"
#include "ref.h"
#include "coord_system2.h"

class Geom;
class Body;

namespace Graphics {class Sprite; }

class Helmet : public Object, public CoordSystem2, public GeomEventHandler {
public:
   Helmet();
   
   void collided(const Ref<Geom>::SharedPtr & with);

   // CoordSystem2
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
   
   Ref<Graphics::Sprite> sprite;
   Ref<Geom> geom;
   Ref<Body> body;
   Ref<Body> snailBody;
   
private:
   int health;
};

#endif // !HELMET_H

