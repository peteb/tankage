/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef RECT_H_Y6VDFOHK
#define RECT_H_Y6VDFOHK

#include <utils/vec.h>

class rect {
public:
  rect(const vec2 & origin = vec2::Zero(), const vec2 & halfSize = vec2::Zero());
  rect(const vec2 & origin, float h_width, float h_height);
  rect(float width, float height);
  
  void getCoords(vec2 & upperLeft, vec2 & lowerRight) const;
  float width() const {
    return halfSize.x * 2.0f;
  }

  float height() const {
    return halfSize.y * 2.0f;
  }
  
  static bool intersect(const rect & r1, const rect & r2);
  
  vec2 origin, halfSize;
};


#endif /* end of include guard: RECT_H_Y6VDFOHK */
