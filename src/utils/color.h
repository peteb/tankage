/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef UTILS_COLOR_H
#define UTILS_COLOR_H

class color4 {
public:
  color4(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a)
  {
  }
 
  static color4 White();
  
  float r, g, b, a;
};
   
#endif // !UTILS_COLOR_H
