/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "player_entity.h"
#include <algorithm>

void PlayerEntity::setTarget(const boost::weak_ptr<ReferenceFrame2D> & newTarget) {
   this->target = newTarget;
}

void PlayerEntity::update(float dt) {
   if (boost::shared_ptr<ReferenceFrame2D> acquiredTarget = target.lock()) {
      vec2 pos = acquiredTarget->getPosition();
      pos.x = std::max(pos.x, -10.0f);
      pos.x = std::min(pos.x, 890.0f);
      pos.y = std::max(pos.y, 10.0f);
      pos.y = std::min(pos.y, 600.0f - 10.0f);


      vec2 delta = vec2(500.0f, pos.y) - pos;

      if (delta.getMagnitude() > 50.0f) {
         delta.normalize();
         delta = delta * 50.0f;
      }
         
      acquiredTarget->setPosition(pos + delta * 5.0f * dt);
   }   
}

void PlayerEntity::setPosition(const vec2 & newPos) {
   if (boost::shared_ptr<ReferenceFrame2D> acquiredTarget = target.lock()) {
      acquiredTarget->setPosition(newPos);
   }
}

vec2 PlayerEntity::getPosition() const {
   if (boost::shared_ptr<ReferenceFrame2D> acquiredTarget = target.lock()) {
      return acquiredTarget->getPosition();
   }

   return vec2::Zero;
}
