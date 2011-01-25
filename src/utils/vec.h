/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef VEC2_H_8BJJJ5TN
#define VEC2_H_8BJJJ5TN

#include <string>
#include <cmath>
#include <utils/value.h>

/// A two-dimensional vector.
class vec2 {
public:
  /// Default constructor doesn't initialize anything.
  vec2() {}
  vec2(float x, float y) : x(x), y(y) {}
  
  vec2 operator - () const {return vec2(-x, -y); }


  vec2 &operator *= (const vec2 &rhs) {x *= rhs.x; y *= rhs.y; return *this; }
  vec2 &operator *= (float scalar) {x *= scalar; y *= scalar; return *this; }
  vec2 &operator /= (float scalar) {x /= scalar; y /= scalar; return *this; }
  
  template<typename OtherT>
  vec2 operator * (const OtherT &rhs) const {return vec2(*this) *= rhs; }
  vec2 operator / (float scalar) {return vec2(*this) /= scalar; }
  vec2 &operator += (const vec2 &rh) {x += rh.x; y += rh.y; return *this; }
  vec2 &operator -= (const vec2 &rh) {x -= rh.x; y -= rh.y; return *this; }
  vec2 operator + (const vec2 &rh) const {return vec2(*this) += rh; }
  vec2 operator - (const vec2 &rh) const {return vec2(*this) -= rh; }
  
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

inline vec2 closest_point(const vec2 &start, const vec2 &end, const vec2 &pos) {
  vec2 ap = pos - start;
  vec2 ab = end - start;
  float ab2 = dot(ab, ab);
  float apab = dot(ap, ab);
  float t = apab / ab2;

  return ab * clamp(t, 0.0f, 1.0f) + start;
}

#endif /* end of include guard: VEC2_H_8BJJJ5TN */
