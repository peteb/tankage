/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "graphics/render_list.h"
#include "graphics/mesh.h"

using Graphics::Sprite;
using Graphics::Renderer;
using Graphics::SpriteEventHandler;

Sprite::Sprite(const Ref<Renderer>::SharedPtr & renderer, const rect & size) 
   : orientation(mat2::Identity)
   , renderer(renderer)
   , size(size)
{
}

std::vector<Vertex2T2> Sprite::constructVertices() const {
   std::vector<Vertex2T2> vertices;
   vertices.reserve(4);

   const float halfWidth = size.halfSize.x;
   const float halfHeight = size.halfSize.y;

   vertices.push_back(Vertex2T2(vec2(-halfWidth, -halfHeight) + position, vec2(0.0f, 0.0f)));
   vertices.push_back(Vertex2T2(vec2( halfWidth, -halfHeight) + position, vec2(1.0f, 0.0f)));
   vertices.push_back(Vertex2T2(vec2( halfWidth,  halfHeight) + position, vec2(1.0f, 1.0f)));
   vertices.push_back(Vertex2T2(vec2(-halfWidth,  halfHeight) + position, vec2(0.0f, 1.0f)));

   return vertices;
}

rect Sprite::getBoundingBox() const {
   rect ret = getSize();
   ret.origin = position;
   return ret;
}

void Sprite::setPosition(const vec2 & newPos) {
   this->position = newPos;
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

void Sprite::enteredView() {
   if (Ref<SpriteEventHandler>::SharedPtr locked = eventHandler.lock())
      locked->enteredView();
}

void Sprite::leftView() {
   if (Ref<SpriteEventHandler>::SharedPtr locked = eventHandler.lock())
      locked->leftView();
}

void Sprite::setEventHandler(const Ref<SpriteEventHandler>::WeakPtr & eventHandler) {
   this->eventHandler = eventHandler;
}

void Sprite::enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
   Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh);
   mesh->vertices = constructVertices();
   
   renderList->insert(renderer, mesh);
}

// TODO: this shouldn't compile, we're not using enqueueRender anywhere. fix
