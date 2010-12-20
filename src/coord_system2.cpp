//===- coord_system2.cpp - Implements coordinate systems --------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file implements the data class and leaf class for 2D coordinate systems.
//
//===----------------------------------------------------------------------===//

#include "coord_system2.h"

CoordSystemData2 CoordSystemData2::Identity(position_type::Identity,
                                            orientation_type::Identity);

CoordSystemData2::CoordSystemData2(const position_type &pos,
                                   const orientation_type &orient)
   : position(pos)
   , orientation(orient)
{

}

CoordSystemData2 CoordSystemData2::transform(const CoordSystemData2 &other) const {
   CoordSystemData2 ret = *this;
   ret.position += other.position;
   // TODO: orientation. do it now.
   return ret;
}

CoordSystemData2 CoordSystemData2::inverse() const {
   return CoordSystemData2(-position, orientation);
}


CoordSystemLeaf2::CoordSystemLeaf2(const data_type::position_type &pos,
                                   const data_type::orientation_type &orient)
   : coordSystem(CoordSystemData2(pos, orient))
{
}

CoordSystemLeaf2::CoordSystemLeaf2(const data_type &coordSystem)
   : coordSystem(coordSystem)
{
}
   
void CoordSystemLeaf2::setTransform(const data_type &cs) {
   coordSystem = cs;
}

CoordSystemLeaf2::data_type CoordSystemLeaf2::getTransform() const {
   return coordSystem;
}

