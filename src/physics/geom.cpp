/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "geom.h"
#include "body.h"
#include "graphics/vertex.h"
#include "graphics/render_list.h"
#include "graphics/mesh.h"
#include <vector>

Geom::Geom(const rect & size)
  : size(size)
  , collisionMask(0xFFFFFFFFU)
{
  collisionId = 0;
  priority = 0;
}

void Geom::setBody(const Ref<Body> & body) {
  this->linkedBody = body;
}

void Geom::setRefFrame(const Ref<CoordSystem2> & refFrame) {
  this->refFrame = refFrame;
}

rect Geom::getSize() const {
  return size;
}

void Geom::setOffset(const vec2 & offset) {
  size.origin = offset;
}


// mat2 Geom::getOrientation() const {
//    if (Ref<Body>::SharedPtr lockedBody = linkedBody.lock())
// 	  return lockedBody->getOrientation();
   
//    return mat2::Identity;
// }

void Geom::setTransform(const CoordSystemData2 & cs) {
  position = cs.position;
}

CoordSystemData2 Geom::getTransform() const {
  return CoordSystemData2(position, mat2::Identity());
}

void Geom::setCollisionId(unsigned int collisionId) {
  this->collisionId = collisionId;
}

void Geom::setCollisionMask(const std::bitset<32> & mask) {
  this->collisionMask = mask;
}

void Geom::setEventHandler(const Ref<GeomEventHandler> & eventHandler) {
  this->eventHandler = eventHandler;
}

Ref<Object>::WeakPtr Geom::getOwner() const {
  if (Ref<Body>::SharedPtr lockedPtr = linkedBody.lock())
    return lockedPtr->getOwner();

  throw std::runtime_error("no body linked to geom");
}

void Geom::collided(const Ref<Geom>::SharedPtr & with) {
  if (Ref<GeomEventHandler>::SharedPtr lockedPtr = eventHandler.lock())
    lockedPtr->collided(with);
}

void Geom::enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
  vec2 position;
  if (Ref<Body>::SharedPtr lockedBody = linkedBody.lock())
    position = lockedBody->getTransform().position;
  else if (Ref<CoordSystem2>::SharedPtr lockedRef = refFrame.lock())
    position = lockedRef->getTransform().position;

  position += size.origin;
   
  // TODO: get vertices from size instead
  std::vector<Vertex2T2> vertices;
  vertices.reserve(4);
  vertices.push_back(Vertex2T2(size.halfSize * vec2(-1.0f, -1.0f) + position, vec2(0.0f, 0.0f)));
  vertices.push_back(Vertex2T2(size.halfSize * vec2(1.0f, -1.0f) + position, vec2(1.0f, 0.0f)));
  vertices.push_back(Vertex2T2(size.halfSize * vec2(1.0f, 1.0f) + position, vec2(1.0f, 1.0f)));
  vertices.push_back(Vertex2T2(size.halfSize * vec2(-1.0f, 1.0f) + position, vec2(0.0f, 1.0f)));
		 
  Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh); // TODO: not good to allocate every time
  mesh->vertices = vertices;
  renderList->insert(Ref<Graphics::Renderer>::SharedPtr(), mesh);
}

void Geom::setPriority(int prio) {
  this->priority = prio;
}

int Geom::getPriority() const {
  return priority;
}
