/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef REFERENCE_FRAME_H_434BR1U2
#define REFERENCE_FRAME_H_434BR1U2

#include "vec2.h"

class ReferenceFrame2D {
public:
   virtual ~ReferenceFrame2D() {}
   
   virtual vec2 getPosition() const =0;
   virtual void setPosition(const vec2 & newPos) =0;
};


#endif /* end of include guard: REFERENCE_FRAME_H_434BR1U2 */
