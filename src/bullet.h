/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BULLET_H_1LH5R0L7
#define BULLET_H_1LH5R0L7

#include <boost/shared_ptr.hpp>
#include "object.h"
#include "reference_frame.h"
#include "graphics/sprite.h"

//namespace Graphics {class Sprite; }
namespace Physics {class Body; }

class World;

class Bullet : public Object, public ReferenceFrame2D, public Graphics::SpriteEventHandler {
public:
   void submitComponents(World & world);

   void setPosition(const vec2 & pos);
   vec2 getPosition() const;

   void leftView();
   
   boost::shared_ptr<Graphics::Sprite> sprite;
   boost::shared_ptr<Physics::Body> body;
};

#endif /* end of include guard: BULLET_H_1LH5R0L7 */
