/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PLAYER_ENTITY_H_5TG24XG5
#define PLAYER_ENTITY_H_5TG24XG5

#include <boost/weak_ptr.hpp>
#include "reference_frame.h"
#include "triggerable.h"

class PlayerEntity : public ReferenceFrame2D, public Triggerable {
public:
   PlayerEntity(float x);
   
   void setTarget(const boost::weak_ptr<ReferenceFrame2D> & newTarget);
   void update(float dt);
   
   void setPosition(const vec2 & newPos);
   vec2 getPosition() const;
   
   void trigger(const std::string & action, int state);
   
private:
   int shooting;
   float tshot;
   
   float xPos;
   boost::weak_ptr<ReferenceFrame2D> target;
};

#endif /* end of include guard: PLAYER_ENTITY_H_5TG24XG5 */
