/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef VEC2_H_8BJJJ5TN
#define VEC2_H_8BJJJ5TN

#include <string>

class vec2 {
public:
   vec2();
   vec2(float x, float y);
   
   vec2 operator + (const vec2 & rh) const;
   vec2 operator - (const vec2 & rh) const;
   vec2 operator - () const;
   vec2 operator * (float scalar) const;
   vec2 operator * (const vec2 & rh) const;
   vec2 & operator += (const vec2 & rh);
   vec2 & operator -= (const vec2 & rh);
   
   operator std::string() const;
   
   float getMagnitude() const;
   vec2 & normalize();
   
   static const vec2 Zero;
   
   float x, y;
};


#endif /* end of include guard: VEC2_H_8BJJJ5TN */
