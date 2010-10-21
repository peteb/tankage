/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <boost/shared_ptr.hpp>

#include "hi_controller.h"
#include "math/vec2.h"
#include "graphics/sprite.h"
#include "triggerable.h"

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
   
   if (Ref<ReferenceFrame2>::SharedPtr acquiredTarget = reframeDelegate.lock()) {
      for (unsigned i = 0; i < activeDirs.size(); ++i) {
         if (activeDirs[i])
            acquiredTarget->setPosition(
               acquiredTarget->getPosition() + dirToVecMap[i] * speed * dt
            );
      }
   }
}

void HiController::setRefFrameDelegate(
   const Ref<ReferenceFrame2> & newTarget) 
{
   this->reframeDelegate = newTarget;
}

void HiController::setActionDelegate(const Ref<Triggerable> & newTarget) {
   this->actionDelegate = newTarget;
}


void HiController::toggle(const std::string & key, int state) {
   int dir = -1;
   
   if (key == "up_arrow")
      dir = 0;
   else if (key == "right_arrow")
      dir = 1;
   else if (key == "down_arrow")
      dir = 2;
   else if (key == "left_arrow")
      dir = 3;
      
   if (dir > -1) {
      if (state)
         startDirection(dir);
      else
         stopDirection(dir);
   }

   if (key == "shoot") {
      if (Ref<Triggerable>::SharedPtr acquiredTarget = actionDelegate.lock()) {
         acquiredTarget->trigger("shoot", state);
      }      
   }
}