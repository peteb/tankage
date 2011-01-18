/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */
 
#include "vec.h"
#include <sstream>


vec2 vec2::Zero() {
  static vec2 zero(0.0f, 0.0f);
  return zero;
}

vec2 vec2::Identity() {
  static vec2 identity(0.0f, 0.0f);
  return identity;
}

vec2::operator std::string() const {
  std::stringstream ss;
  ss << x << ", " << y;
  return ss.str();
}
