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
   , orientation(mat2::Identity)
   , texture(texture)
   , sceneNode(sceneNode)
{
   size = texture->getSize();
}

std::vector<Vertex2T2> Sprite::constructVertices() const {
   std::vector<Vertex2T2> vertices;
   vertices.reserve(4);

   const float halfWidth = size.halfSize.x;
   const float halfHeight = size.halfSize.y;

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

void Sprite::setOrientation(const mat2 & orient) {
   this->orientation = orient;
}

mat2 Sprite::getOrientation() const {
   return orientation;
}

rect Sprite::getSize() const {
   return size;
}

boost::shared_ptr<Texture> Sprite::getTexture() const {
   return texture;
}

void Sprite::enteredView() {
   if (boost::shared_ptr<SpriteEventHandler> locked = eventHandler.lock())
      locked->enteredView();
}

void Sprite::leftView() {
   if (boost::shared_ptr<SpriteEventHandler> locked = eventHandler.lock())
      locked->leftView();
}

void Sprite::setEventHandler(const boost::weak_ptr<SpriteEventHandler> & eventHandler) {
   this->eventHandler = eventHandler;
}
