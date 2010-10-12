/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SNAIL_H_7SGQH1KL
#define SNAIL_H_7SGQH1KL

#include <boost/shared_ptr.hpp>
#include "object.h"

namespace Graphics {class Sprite; class Subsystem; }

class PlayerEntity;
class World;
class vec2;

class Snail : public Object {
public:
   void submitComponents(World & world);
   
   void setPosition(const vec2 & pos);
   
   boost::shared_ptr<Graphics::Sprite> sprite;
   boost::shared_ptr<PlayerEntity> logic;
};

#endif /* end of include guard: SNAIL_H_7SGQH1KL */
