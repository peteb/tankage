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

Physics::Geom::Geom(const rect & size)
   : size(size)
   , collisionMask(0xFFFFFFFFu)
{
   collisionId = 0;
}

void Physics::Geom::setBody(const Ref<Physics::Body> & body) {
   this->linkedBody = body;
}

void Physics::Geom::setRefFrame(const Ref<ReferenceFrame2> & refFrame) {
   this->refFrame = refFrame;
}

rect Physics::Geom::getSize() const {
   return size;
}

void Physics::Geom::setPosition(const vec2 & newPos) {
   this->position = newPos;
}

vec2 Physics::Geom::getPosition() const {
   return position;
}

void Physics::Geom::setOrientation(const mat2 & orient) {
   
}

mat2 Physics::Geom::getOrientation() const {
   if (Ref<Physics::Body>::SharedPtr lockedBody = linkedBody.lock())
	  return lockedBody->getOrientation();
   
   return mat2::Identity;
}

void Physics::Geom::setCollisionId(unsigned int collisionId) {
   this->collisionId = collisionId;
}

void Physics::Geom::setCollisionMask(const std::bitset<32> & mask) {
   this->collisionMask = mask;
}

void Physics::Geom::setEventHandler(const Ref<GeomEventHandler> & eventHandler) {
	this->eventHandler = eventHandler;
}

Ref<Object>::WeakPtr Physics::Geom::getOwner() const {
   if (Ref<Physics::Body>::SharedPtr lockedPtr = linkedBody.lock())
	  return lockedPtr->getOwner();

   throw std::runtime_error("no body linked to geom");
}

void Physics::Geom::collided(const Ref<Geom>::SharedPtr & with) {
	if (Ref<GeomEventHandler>::SharedPtr lockedPtr = eventHandler.lock())
		lockedPtr->collided(with);
}

void Physics::Geom::enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
   vec2 position;
   if (Ref<Physics::Body>::SharedPtr lockedBody = linkedBody.lock())
	  position = lockedBody->getPosition();
   else if (Ref<ReferenceFrame2>::SharedPtr lockedRef = refFrame.lock())
	  position = lockedRef->getPosition();


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
