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
   rect(const vec2 & origin, const vec2 & size);
   
   vec2 origin, size;
};


#endif /* end of include guard: RECT_H_Y6VDFOHK */
