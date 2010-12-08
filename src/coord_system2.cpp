//===- coord_system2.cpp - Implement coordinate systems ---------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file implements the CoordSystemData2 class.
//
//===----------------------------------------------------------------------===//

#include "coord_system2.h"

CoordSystemData2 CoordSystemData2::Identity(position_type::Identity,
                                            orientation_type::Identity);

CoordSystemData2::CoordSystemData2(const position_type & pos,
                                   const orientation_type & orient)
   : position(pos)
   , orientation(orient)
{

}

CoordSystemData2 & CoordSystemData2::transform(const CoordSystemData2 & other) {
   position += other.position;
   // TODO: orientation
   return *this;
}

CoordSystemData2 CoordSystemData2::getInverse() const {
   return CoordSystemData2(-position, orientation);
}
