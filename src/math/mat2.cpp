/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "mat2.h"

mat2 mat2::Identity(1.0f, 0.0f, 0.0f, 1.0f);
mat2 mat2::Zero(0.0f, 0.0f, 0.0f, 0.0f);


mat2::mat2(const vec2 &x, const vec2 &y) {
   elements[0][0] = x.x;
   elements[0][1] = x.y;
   elements[1][0] = y.x;
   elements[1][1] = y.y;
}

mat2::mat2(float e11, float e12, float e21, float e22) {
   elements[0][0] = e11;
   elements[0][1] = e12;
   elements[1][0] = e21;
   elements[1][1] = e22;
}

vec2 mat2::getX() const {
   return vec2(elements[0][0], elements[0][1]);
}

vec2 mat2::getY() const {
   return vec2(elements[1][0], elements[1][1]);
}

vec2 mat2::operator * (const vec2 &v) const {
   float x = v.x * elements[0][0] + v.y * elements[1][0];
   float y = v.x * elements[0][1] + v.y * elements[1][1];
   
   return vec2(x, y);
}

mat2 mat2::operator * (const mat2 &m) const {
   return mat2::Zero;
}

