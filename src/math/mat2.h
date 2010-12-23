//===- mat2.h - Two-dimensional matrix --------------------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the 2D matrix class.
//
//===----------------------------------------------------------------------===//

#ifndef MAT2_H_LF0Q48ED
#define MAT2_H_LF0Q48ED

#include "vec2.h"

class mat2 {
public:
  mat2(const vec2 &x, const vec2 &y);
  mat2(float e11, float e12, float e21, float e22);
  
  vec2 operator * (const vec2 &v) const;
  mat2 operator * (const mat2 &m) const;

  // FIXME: inverse
  vec2 getX() const;
  vec2 getY() const;
  
  static mat2 Identity;
  static mat2 Zero;
  
  float elements[2][2];
};


#endif /* end of include guard: MAT2_H_LF0Q48ED */
