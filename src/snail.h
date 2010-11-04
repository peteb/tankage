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
#include "physics/geom.h"
#include "physics/body.h"

namespace Graphics {class Sprite; class Subsystem; }
namespace Physics {class Geom; }

class PlayerEntity;
class World;
class vec2;

class Snail : public Object, public Physics::GeomEventHandler {
public:
   Snail();
   
   void setPosition(const vec2 & pos);
   void collided(const boost::shared_ptr<Physics::Geom> & with);
   
   
   
   Ref<Graphics::Sprite> sprite;
   Ref<PlayerEntity> logic;
   Ref<Physics::Geom> physGeom;
   Ref<Physics::Body> physBody;
   int health;
};

#endif /* end of include guard: SNAIL_H_7SGQH1KL */
