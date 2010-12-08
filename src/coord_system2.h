//===- coord_system2.h - Coordinate system class definitions ----*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the CoordSystemData2 class and CoordSystem2 interface.
//
//===----------------------------------------------------------------------===//

#ifndef COORDSYSTEM2_H
#define COORDSYSTEM2_H

#include "math/vec2.h"
#include "math/mat2.h"

/// CoordSysData2 describes a coordinate system in two dimensions.
class CoordSystemData2 {
public:
   CoordSystemData2(const vec2 & pos, const mat2 & orient) : position(pos), orientation(orient) {}
   
   vec2 position;
   mat2 orientation;
};

/// Coordinate system interface.
class CoordSystem2 {
public:
   virtual ~CoordSystem2() {}

   virtual void setTransform(const CoordSystemData2 & cs) =0;
   virtual CoordSystemData2 getTransform() const =0;
};

#endif /* end of include guard: COORDSYSTEM2_H */
