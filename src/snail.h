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

class Snail : public Object {
public:
   void submitComponents(World & world);
   
   boost::shared_ptr<Graphics::Sprite> sprite;
   boost::shared_ptr<PlayerEntity> logic;
};

#endif /* end of include guard: SNAIL_H_7SGQH1KL */
