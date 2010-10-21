/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */
 
#include <sstream>
#include <cmath>
#include "vec2.h"

const vec2 vec2::Zero(0.0f, 0.0f);
 
vec2::vec2() {
   x = y = 0.0f;
}

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

float vec2::getMagnitude() const {
   return sqrt(x * x + y * y);
}

vec2 & vec2::normalize() {
   float mag = getMagnitude();
   
   if (mag > 0.0f) {
      x /= mag;
      y /= mag;
   }
   
   return *this;
}


vec2::operator std::string() const {
   std::stringstream ss;
   ss << x << ", " << y;
   return ss.str();
}