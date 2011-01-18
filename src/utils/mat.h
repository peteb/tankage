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

#include <utils/vec.h>

class mat2 {
public:
  /// Default constructor, doesn't initialize any members
  mat2() {}

  mat2(const vec2 &x, const vec2 &y) {
    elements[0][0] = x.x;
    elements[0][1] = x.y;
    elements[1][0] = y.x;
    elements[1][1] = y.y;
  }
  
  mat2(float e11, float e12, float e21, float e22) {
    elements[0][0] = e11;
    elements[0][1] = e12;
    elements[1][0] = e21;
    elements[1][1] = e22;
  }
  
  vec2 operator * (const vec2 &v) const {
    return vec2(v.x * elements[0][0] + v.y * elements[1][0],
                v.x * elements[0][1] + v.y * elements[1][1]);
  }
  
  mat2 operator * (const mat2 &m) const;
  mat2 &operator *= (const mat2 &m) {*this = *this * m; return *this; }
  
  // FIXME: inverse
  vec2 x() const {return vec2(elements[0][0], elements[0][1]); }
  vec2 y() const {return vec2(elements[1][0], elements[1][1]); }
  
  static mat2 Identity();
  static mat2 Zero();
  
  float elements[2][2];
};


#endif /* end of include guard: MAT2_H_LF0Q48ED */
