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
#include "reference_frame2.h"

namespace Graphics {class Sprite; class Subsystem; }
namespace Physics {class Geom; }

class PlayerEntity;
class World;
class vec2;

class SnailEventHandler {
public:
   virtual ~SnailEventHandler() {}
   
   virtual void onHealthChange(float newValue) =0;
};

class Snail : public Object, public ReferenceFrame2, public Physics::GeomEventHandler {
public:
   Snail();
   
   void setPosition(const vec2 & pos);
   vec2 getPosition() const;
   mat2 getOrientation() const;
   void setOrientation(const mat2 & newOrientation);

   void collided(const Ref<Physics::Geom>::SharedPtr & with);
   
   void setEventHandler(const Ref<SnailEventHandler> & newHandler);
   void increaseHealth(int add);

   Ref<SnailEventHandler> eventHandler;
   Ref<Graphics::Sprite> sprite;
   Ref<Graphics::Sprite> helmet;
   Ref<PlayerEntity> logic;
   Ref<Physics::Geom> physGeom;
   Ref<Physics::Body> physBody;
   
private:
   void updateHelmet();
   
   int health;
};

#endif /* end of include guard: SNAIL_H_7SGQH1KL */
