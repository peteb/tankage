/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/sprite.h"

using Graphics::Sprite;
 
Sprite::Sprite() 
   : position(vec2::Zero) 
{
   
}

std::vector<vec2> Sprite::constructVertices() const {
   std::vector<vec2> vertices;
   vertices.reserve(4);

   const float halfWidth = 32.0f / 2.0f;
   const float halfHeight = 32.0f / 2.0f;

   vertices.push_back(vec2(-halfWidth, -halfHeight));
   vertices.push_back(vec2( halfWidth, -halfHeight));
   vertices.push_back(vec2( halfWidth,  halfHeight));
   vertices.push_back(vec2(-halfWidth,  halfHeight));

   return vertices;
}

void Sprite::setPosition(const vec2 & newPos) {
   this->position = newPos;
}

vec2 Sprite::getPosition() const {
   return position;
}