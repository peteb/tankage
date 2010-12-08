//===- coordsystem_transformer.h --------------------------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the CoordSystemTransformer class.
//
//===----------------------------------------------------------------------===//

#ifndef COORDSYSTEM_TRANSFORMER_H
#define COORDSYSTEM_TRANSFORMER_H

#include "coord_system2.h"
#include "ref.h"

/*
ReframeTransformer::ReframeTransformer(const Ref<CoordSystem2> & delegate, const vec2 & coord, const mat2 & orient)
   : delegate(delegate)
   , coord(coord)
   , orient(orient)
{
}

void ReframeTransformer::setTransform(const CoordSystemData2 & cs) {
   if (Ref<CoordSystem2>::SharedPtr lockedDelegate = delegate.lock()) {
      lockedDelegate->setTransform(CoordSystemData2(cs.position + coord, orient));
   }
}

CoordSystemData2 ReframeTransformer::getTransform() const {
   return CoordSystemData2(delegate->getTransform().position - coord, orient);  
}
*/



/// This class provides a converter that describes a parent-child relationship
/// between two coordinate systems.
template<typename CoordSysT>
class CoordSystemTransformer : public CoordSysT {
public:

   /// CoordSysTransformer ctor - Binds the transformer to the child and sets
   /// the transformation to make.
   CoordSystemTransformer(const Ref<CoordSysT> & delegate,
                          const typename CoordSysT::data_type & frame)
      : delegate(delegate)
      , frame(frame)
   {
   }

   
   void setTransform(const typename CoordSysT::data_type & cs) {
      typedef typename CoordSysT::data_type CsFrameT;
      
      if (typename Ref<CoordSysT>::SharedPtr lockedDelegate = delegate.lock()) {
         CsFrameT newFrame = CsFrameT(cs).transform(frame);
         lockedDelegate->setTransform(newFrame);
      }
   }
   
   typename CoordSysT::data_type getTransform() const {
      typedef typename CoordSysT::data_type CsFrameT;

      if (typename Ref<CoordSysT>::SharedPtr lockedDelegate = delegate.lock()) {
         CsFrameT delegateFrame = lockedDelegate->getTransform();
         return CsFrameT(delegateFrame).transform(frame.getInverse());
      }
      
      return  CoordSysT::data_type::Identity;
   }


private:
   Ref<CoordSystem2> delegate;
   typename CoordSysT::data_type frame;
};

#endif // !COORDSYS_TRANSFORMER_H
