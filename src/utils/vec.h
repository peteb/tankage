/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef VEC2_H_8BJJJ5TN
#define VEC2_H_8BJJJ5TN

#include <string>
#include <cmath>

/// A two-dimensional vector.
class vec2 {
public:
  /// Default constructor doesn't initialize anything.
  vec2() {}
  vec2(float x, float y) : x(x), y(y) {}
  
  vec2 operator - () const {return vec2(-x, -y); }
  vec2 operator * (float scalar) const {return vec2(x * scalar, y * scalar); }
  vec2 operator * (const vec2 & rh) const {return vec2(x * rh.x, y * rh.y); }
  vec2 & operator += (const vec2 & rh) {x += rh.x; y += rh.y; return *this; }
  vec2 & operator -= (const vec2 & rh) {x -= rh.x; y -= rh.y; return *this; }
  vec2 operator + (const vec2 & rh) const {return vec2(*this) += rh; }
  vec2 operator - (const vec2 & rh) const {return vec2(*this) -= rh; }
  
  float magnitude() const {return sqrt(x * x + y * y); }

  vec2 &normalize() {
    float mag = magnitude();
    if (mag > 0.0f) {
      x /= mag;
      y /= mag;
    }

    return *this;
  }

  /// Textual representation of a vector for easy outputting
  operator std::string() const;
  
  static vec2 Zero();
  static vec2 Identity();
  
  float x;
  float y;
};

inline float dot(const vec2 &v1, const vec2 &v2) {
  return v1.x * v2.x + v1.y * v2.y;
}


#endif /* end of include guard: VEC2_H_8BJJJ5TN */
