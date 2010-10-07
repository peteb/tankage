/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BULLET_H_1LH5R0L7
#define BULLET_H_1LH5R0L7

#include <boost/shared_ptr.hpp>
#include "object.h"

namespace Graphics {class Sprite; }

class World;

class Bullet : public Object {
public:
   void submitComponents(World & world);

   boost::shared_ptr<Graphics::Sprite> sprite;
};

#endif /* end of include guard: BULLET_H_1LH5R0L7 */
