/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SNAIL_H_7SGQH1KL
#define SNAIL_H_7SGQH1KL

#include "object.h"
#include "ref.h"
#include "physics/geom.h"
#include "physics/body.h"
#include "coord_system2.h"

namespace Graphics {class Sprite; class Subsystem; }
namespace Physics {class Geom; }

class PlayerEntity;
class World;
class vec2;
class Helmet;

class SnailEventHandler {
public:
   virtual ~SnailEventHandler() {}
   
   virtual void onHealthChange(float newValue) =0;
};

class Snail : public Object, public CoordSystem2, public Physics::GeomEventHandler {
public:
   Snail();

   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;
   
   void collided(const Ref<Physics::Geom>::SharedPtr & with);
   
   void setEventHandler(const Ref<SnailEventHandler> & newHandler);
   void increaseHealth(int add);

   Ref<SnailEventHandler> eventHandler;
   Ref<Graphics::Sprite> sprite;
   Ref<Helmet> helmet;
   Ref<PlayerEntity> logic;
   Ref<Physics::Geom> physGeom;
   Ref<Physics::Body> physBody;
   
private:
   int health;
};

#endif /* end of include guard: SNAIL_H_7SGQH1KL */
