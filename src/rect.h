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
   rect(const vec2 & origin, const vec2 & halfSize);

   void getCoords(vec2 & upperLeft, vec2 & lowerRight) const;
   
   static bool intersect(const rect & r1, const rect & r2);
   
   vec2 origin, halfSize;
};


#endif /* end of include guard: RECT_H_Y6VDFOHK */
