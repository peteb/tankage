
/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef CACTUS_H
#define CACTUS_H

#include <boost/shared_ptr.hpp>
#include "object.h"
#include "reference_frame2.h"
#include "graphics/sprite.h"
#include "ref.h"
#include "physics/geom.h"

//namespace Graphics {class Sprite; }
namespace Physics {class Body; class Geom; }

class World;

class Cactus : public Object, public ReferenceFrame2, public Graphics::SpriteEventHandler, public Physics::GeomEventHandler {
public:
   // ReferenceFrame2 ------------------------------------------------
   void setPosition(const vec2 & pos);
   vec2 getPosition() const;
   void setOrientation(const mat2 & orient);
   mat2 getOrientation() const;
   // ----------------------------------------------------------------

   void collided(const boost::shared_ptr<Physics::Geom> & with);
   void leftView();

   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Body> body;
   Ref<Physics::Geom> geom;
};

#endif /* end of include guard: CACTUS_H */
