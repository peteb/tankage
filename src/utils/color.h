/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef UTILS_COLOR_H
#define UTILS_COLOR_H

class color4 {
public:
  color4() {}
  
  color4(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a)
  {
  }
 
  color4 operator *(float scalar) const {
    color4 ret = color4(r * scalar, g * scalar, b * scalar, a * scalar);
    return ret;
  }
  
  color4 operator -(const color4 &rhs) const {
    color4 ret = color4(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
    return ret;
  }

  color4 operator +(const color4 &rhs) const {
    color4 ret = color4(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
    return ret;
  }
  
  static color4 White();
  
  float r, g, b, a;
};
   
#endif // !UTILS_COLOR_H
