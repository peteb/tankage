/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BULLET_H_1LH5R0L7
#define BULLET_H_1LH5R0L7

#include <boost/shared_ptr.hpp>
#include "object.h"
#include "reference_frame2.h"
#include "graphics/sprite.h"
#include "ref.h"

//namespace Graphics {class Sprite; }
namespace Physics {class Body; class Geom; }

class World;

class Bullet : public Object, public ReferenceFrame2, public Graphics::SpriteEventHandler {
public:
   // ReferenceFrame2 ------------------------------------------------
   void setPosition(const vec2 & pos);
   vec2 getPosition() const;
   void setOrientation(const mat2 & orient);
   mat2 getOrientation() const;
   // ----------------------------------------------------------------
   
   void leftView();
   
   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Body> body;
   Ref<Physics::Geom> geom;
};

#endif /* end of include guard: BULLET_H_1LH5R0L7 */
