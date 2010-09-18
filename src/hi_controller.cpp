/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "hi_controller.h"
#include "vec2.h"
#include "graphics/sprite.h"

static vec2 dirToVecMap[4] = {
   vec2(0.0f, 1.0f), 
   vec2(1.0f, 0.0f), 
   vec2(0.0f, -1.0f), 
   vec2(-1.0f, 0.0f)
};
 
HiController::HiController()
   : activeDirs(4, false)
{   
   target = NULL;
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
   if (target) {
      for (unsigned i = 0; i < activeDirs.size(); ++i) {
         if (activeDirs[i])
            target->position += dirToVecMap[i] * dt;
      }
   }
}

void HiController::setTarget(Graphics::Sprite * target) {
   this->target = target;
}