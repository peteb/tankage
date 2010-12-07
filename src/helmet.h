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
#include "reference_frame2.h"

namespace Physics {class Geom; class Body; }
namespace Graphics {class Sprite; }

class Helmet : public Object, public ReferenceFrame2, public Physics::GeomEventHandler {
public:
   Helmet();
   
   void collided(const Ref<Physics::Geom>::SharedPtr & with);

   // ReferenceFrame2
   vec2 getPosition() const;
   void setPosition(const vec2 & newPos);
   mat2 getOrientation() const;
   void setOrientation(const mat2 & newOrientation);
   // !ReferenceFrame2
   
   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Geom> geom;
   Ref<Physics::Body> body;

private:
   int health;
};

#endif // !HELMET_H

