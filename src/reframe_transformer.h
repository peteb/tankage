/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef REFRAME_TRANSFORMER_H
#define REFRAME_TRANSFORMER_H

#include "reference_frame2.h"
#include "ref.h"

class ReframeTransformer : public ReferenceFrame2 {
public:
   ReframeTransformer(const Ref<ReferenceFrame2> & delegate, const vec2 & coord, const mat2 & orient = mat2::Identity);
   
   vec2 getPosition() const;
   void setPosition(const vec2 & newPos);
   mat2 getOrientation() const;
   void setOrientation(const mat2 & newOrientation);


private:
   Ref<ReferenceFrame2> delegate;
   vec2 coord;
   mat2 orient;
};

#endif // !REFRAME_TRANSFORMER_H
