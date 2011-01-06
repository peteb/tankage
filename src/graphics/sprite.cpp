/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "graphics/render_list.h"
#include "graphics/mesh.h"
#include <cmath>
#include <iostream>

using Graphics::Renderer;

Sprite::Sprite(const Ref<Renderer>::SharedPtr & renderer, const rect & size) 
  : orientation(mat2::Identity())
  , renderer(renderer)
  , size(size)
  , originalSize(size)
{
  columns = rows = 1;
  cellX = cellY = 0;
}

std::vector<Vertex2T2> Sprite::constructVertices() const {
  std::vector<Vertex2T2> vertices;
  vertices.reserve(4);
   
  rect offsetRect = size;
  vec2 ul, lr;
  offsetRect.getCoords(ul, lr);

  // TODO: clean this up with a CoordSystemData
  ul = ul;
  lr = lr;
//   ul += position;
//   lr += position;
   
  float cw = 1.0f / static_cast<float>(columns);
  float ch = 1.0f / static_cast<float>(rows);

  float t1x = cw * static_cast<float>(cellX);
  float t1y = ch * static_cast<float>(cellY);
  float t2x = t1x + cw;
  float t2y = t1y + ch;
   
  vertices.push_back(Vertex2T2(ul, vec2(t1x, t1y)));
  vertices.push_back(Vertex2T2(vec2(lr.x, ul.y), vec2(t2x, t1y)));
  vertices.push_back(Vertex2T2(lr, vec2(t2x, t2y)));
  vertices.push_back(Vertex2T2(vec2(ul.x, lr.y), vec2(t1x, t2y)));

  for (size_t i = 0; i < vertices.size(); ++i) {
    vertices[i].pos = position + orientation * vertices[i].pos;
  }
   
  return vertices;
}

rect Sprite::getBoundingBox() const {
  rect ret = getSize();
  ret.origin = position;
  return ret;
}

void Sprite::setTransform(const CoordSystemData2 & cs) {
  position = cs.position;
  orientation = cs.orientation;
   
  if (Ref<CoordSystem2>::SharedPtr lockedDelegate = delegate.lock())
    lockedDelegate->setTransform(cs);
}

CoordSystemData2 Sprite::getTransform() const {
  return CoordSystemData2(position, orientation);
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

void Sprite::setDelegate(const Ref<CoordSystem2> & delegate) {
  this->delegate = delegate;
}

void Sprite::setGrid(int columns, int rows) {
  this->columns = columns;
  this->rows = rows;
  this->size = rect((size.halfSize.x * 2.0f) / static_cast<float>(columns),
                    (size.halfSize.y * 2.0f) / static_cast<float>(rows));
}

void Sprite::setCell(int x, int y) {
  this->cellX = x;
  this->cellY = y;
}


void Sprite::enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
  Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh);
  mesh->vertices = constructVertices();
   
  renderList->insert(renderer, mesh);
}

