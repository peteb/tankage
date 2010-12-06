/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "reframe_transformer.h"

ReframeTransformer::ReframeTransformer(const Ref<ReferenceFrame2> & delegate, const vec2 & coord, const mat2 & orient)
   : delegate(delegate)
   , coord(coord)
   , orient(orient)
{
}

vec2 ReframeTransformer::getPosition() const {
   if (Ref<ReferenceFrame2>::SharedPtr lockedDelegate = delegate.lock())
      return lockedDelegate->getPosition() - coord;

   return coord;
}

void ReframeTransformer::setPosition(const vec2 & newPos) {
   if (Ref<ReferenceFrame2>::SharedPtr lockedDelegate = delegate.lock())
      lockedDelegate->setPosition(newPos + coord);  
}

mat2 ReframeTransformer::getOrientation() const {
   if (Ref<ReferenceFrame2>::SharedPtr lockedDelegate = delegate.lock())
      return lockedDelegate->getOrientation(); // TODO 

   return orient;
}

void ReframeTransformer::setOrientation(const mat2 & newOrientation) {
   if (Ref<ReferenceFrame2>::SharedPtr lockedDelegate = delegate.lock())
      lockedDelegate->setOrientation(newOrientation); // TODO  

}


