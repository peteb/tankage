/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef POWERUP_H
#define POWERUP_H

#include "ref.h"
#include "object.h"

namespace Graphics {class Sprite; }
namespace Physics {class Body; class Geom; }

class PowerUp : public Object {
public:
   
   Ref<Graphics::Sprite> sprite;
   Ref<Physics::Body> body;
   Ref<Physics::Geom> geom;
};

#endif // !POWERUP_H
