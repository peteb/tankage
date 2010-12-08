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

/// CoordSystemData2 describes a coordinate system in two dimensions.
class CoordSystemData2 {
public:
   typedef vec2 position_type; ///< translation/position type
   typedef mat2 orientation_type; ///< rotation/orientation type

   static CoordSystemData2 Identity; ///< the identity coordinate system,
                                     ///applying a transform using the identity
                                     ///will result in no change.

   CoordSystemData2(const position_type & pos, const orientation_type & orient);
   
   /// Transforms the coordinate system through translation and matrix
   /// multiplication. 
   CoordSystemData2 & transform(const CoordSystemData2 & other);

   /// Calculates the inverse coordinate system.
   CoordSystemData2 getInverse() const;

   
   position_type position;
   orientation_type orientation;
};

/// Interface for modifying and retrieving a coordinate system.
class CoordSystem2 {
public:
   typedef CoordSystemData2 data_type; ///< the concrete data type of the
                                       ///interface; the kind of data that it
                                       ///works on.

   virtual ~CoordSystem2() {}

   virtual void setTransform(const data_type & cs) =0;
   virtual data_type getTransform() const =0;
};

#endif /* end of include guard: COORDSYSTEM2_H */
