/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef RECT_H_Y6VDFOHK
#define RECT_H_Y6VDFOHK

#include "vec2.h"

class rect {
public:
   rect(const vec2 & origin = vec2::Zero, const vec2 & halfSize = vec2::Zero);
   rect(const vec2 & origin, float width, float height);
   rect(float width, float height);
   
   void getCoords(vec2 & upperLeft, vec2 & lowerRight) const;
   
   static bool intersect(const rect & r1, const rect & r2);
   
   vec2 origin, halfSize;
};


#endif /* end of include guard: RECT_H_Y6VDFOHK */
