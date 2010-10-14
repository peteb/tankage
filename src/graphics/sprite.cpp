/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "graphics/gfx_subsystem.h"

using Graphics::Sprite;
 
Sprite::Sprite(const boost::shared_ptr<Texture> & texture, BoundedSprite * sceneNode) 
   : position(vec2::Zero) 
   , texture(texture)
   , sceneNode(sceneNode)
{
   
}

std::vector<Vertex2T2> Sprite::constructVertices() const {
   std::vector<Vertex2T2> vertices;
   vertices.reserve(4);

   const float halfWidth = 64.0f / 2.0f;
   const float halfHeight = 64.0f / 2.0f;

   vertices.push_back(Vertex2T2(vec2(-halfWidth, -halfHeight), vec2(0.0f, 0.0f)));
   vertices.push_back(Vertex2T2(vec2( halfWidth, -halfHeight), vec2(1.0f, 0.0f)));
   vertices.push_back(Vertex2T2(vec2( halfWidth,  halfHeight), vec2(1.0f, 1.0f)));
   vertices.push_back(Vertex2T2(vec2(-halfWidth,  halfHeight), vec2(0.0f, 1.0f)));

   return vertices;
}

void Sprite::setPosition(const vec2 & newPos) {
   this->position = newPos;
   if (sceneNode)
      sceneNode->boundingArea.origin = newPos;
}

vec2 Sprite::getPosition() const {
   return position;
}

rect Sprite::getSize() const {
   return texture->getSize();
}