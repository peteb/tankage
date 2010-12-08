//===- CoordSysTransformer.h ------------------------------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the CoordSysTransformer class.
//
//===----------------------------------------------------------------------===//

#ifndef COORDSYS_TRANSFORMER_H
#define COORDSYS_TRANSFORMER_H

#include "coord_system2.h"
#include "ref.h"

/// This class provides a converter for describing a parent-child relationship
/// between two coordinate systems.
class ReframeTransformer : public CoordSystem2 {
public:

   /// CoordSysTransformer ctor - Binds the transformer to the child and sets
   /// the transformation to make.
   ReframeTransformer(const Ref<CoordSystem2> & delegate,
                      const vec2 & coord,
                      const mat2 & orient = mat2::Identity);
   
   void setTransform(const CoordSystemData2 & cs);
   CoordSystemData2 getTransform() const;


private:
   Ref<CoordSystem2> delegate;
   vec2 coord;
   mat2 orient;
};

#endif // !COORDSYS_TRANSFORMER_H
