/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */
 
#include <sstream>
#include "vec2.h"

const vec2 vec2::Zero(0.0f, 0.0f);
 
vec2::vec2(float x, float y) 
   : x(x), y(y)
{
}

vec2 vec2::operator + (const vec2 & rh) const {
   return vec2(x + rh.x, y + rh.y);
}

vec2 vec2::operator - (const vec2 & rh) const {
   return vec2(x - rh.x, y - rh.y);
}

vec2 vec2::operator - () const {
   return vec2(-x, -y);
}

vec2 vec2::operator * (float scalar) const {
   return vec2(x * scalar, y * scalar);
}

vec2 & vec2::operator += (const vec2 & rh) {
   x += rh.x;
   y += rh.y;
   return *this;
}
vec2 & vec2::operator -= (const vec2 & rh) {
   x -= rh.x;
   y -= rh.y;
   return *this;
}


vec2::operator std::string() const {
   std::stringstream ss;
   ss << x << ", " << y;
   return ss.str();
}