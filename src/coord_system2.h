//===- coord_system2.h - Coordinate system class definitions ----*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the CoordSystemData2 class and CoordSystem2
// interface. These classes are the basis for managing two-dimensional
// coordinate systems in the engine.
//
//===----------------------------------------------------------------------===//

#ifndef COORDSYSTEM2_H
#define COORDSYSTEM2_H

#include "math/vec2.h"
#include "math/mat2.h"

// FIXME: this needs testing, preferably sooner than later

/// CoordSystemData2 describes a coordinate system in two dimensions containing
/// a position/translation and orientation/rotation.
class CoordSystemData2 {
public:
  typedef vec2 position_type;
  typedef mat2 orientation_type;
  
  CoordSystemData2(const position_type &pos, const orientation_type &orient);
  
  CoordSystemData2 transform(const CoordSystemData2 &other) const;
  CoordSystemData2 inverse() const;
  
  static CoordSystemData2 Identity;
  
  position_type position;
  orientation_type orientation;
};

/// Interface for modifying and retrieving a coordinate system.
class CoordSystem2 {
public:
  typedef CoordSystemData2 data_type;
  
  virtual ~CoordSystem2() {}
  
  /// Updates the coordinate system with the new one. A successive call to
  /// getTransform should return the same value as was given to setTransform
  /// unless it's a read-only coordinate system.
  virtual void setTransform(const data_type &cs) =0;

  virtual data_type getTransform() const =0;
};

/// A leaf/final implementation of the CoordSystem2 interface, returns the given
/// coordinate system data.
class CoordSystemLeaf2 : public CoordSystem2 {
public:
  CoordSystemLeaf2(const data_type::position_type &pos,
                   const data_type::orientation_type &orient);   
  CoordSystemLeaf2(const data_type &coordSystem);
  
  void setTransform(const data_type &cs);
  data_type getTransform() const;
  
private:
  data_type coordSystem;
};

#endif /* end of include guard: COORDSYSTEM2_H */
