/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SNAIL_H_7SGQH1KL
#define SNAIL_H_7SGQH1KL

#include <boost/shared_ptr.hpp>
#include "object.h"
#include "ref.h"

namespace Graphics {class Sprite; class Subsystem; }
namespace Physics {class Geom; }

class PlayerEntity;
class World;
class vec2;

class Snail : public Object {
public:
   void submitComponents(World & world);
   void setPosition(const vec2 & pos);
   
   Ref<Graphics::Sprite> sprite;
   Ref<PlayerEntity> logic;
   Ref<Physics::Geom> physGeom;
};

#endif /* end of include guard: SNAIL_H_7SGQH1KL */
