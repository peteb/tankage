/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <boost/shared_ptr.hpp>

#include "hi_controller.h"
#include "vec2.h"
#include "graphics/sprite.h"

static vec2 dirToVecMap[4] = {
   vec2(0.0f, -1.0f), 
   vec2(1.0f, 0.0f), 
   vec2(0.0f, 1.0f), 
   vec2(-1.0f, 0.0f)
};
 
HiController::HiController()
   : activeDirs(4, false)
{   
}

void HiController::startDirection(int dir) {
   if (dir >= 0 && dir < activeDirs.size())
      activeDirs[dir] = true;
}

void HiController::stopDirection(int dir) {
   if (dir >= 0 && dir < activeDirs.size())
      activeDirs[dir] = false;   
}

void HiController::update(float dt) {
   static const float speed = 300.0f;
   
   if (boost::shared_ptr<Graphics::Sprite> acquiredTarget = target.lock()) {
      for (unsigned i = 0; i < activeDirs.size(); ++i) {
         if (activeDirs[i])
            acquiredTarget->position += dirToVecMap[i] * speed * dt;
      }
   }
}

void HiController::setTarget(boost::shared_ptr<Graphics::Sprite> & newTarget) {
   this->target = newTarget;
}
