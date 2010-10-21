/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef REFERENCE_FRAME2_H_434BR1U2
#define REFERENCE_FRAME2_H_434BR1U2

#include "math/vec2.h"
#include "math/mat2.h"

class ReferenceFrame2 {
public:
   virtual ~ReferenceFrame2() {}
   
   virtual vec2 getPosition() const =0;
   virtual void setPosition(const vec2 & newPos) =0;
   
   virtual mat2 getOrientation() const =0;
   virtual void setOrientation(const mat2 & newOrientation) =0;
};


#endif /* end of include guard: REFERENCE_FRAME2_H_434BR1U2 */
