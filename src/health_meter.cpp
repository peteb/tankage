/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "health_meter.h"
#include "graphics/texture.h"
#include "graphics/render_list.h"
#include "graphics/vertex.h"
#include "graphics/renderer.h"
#include "graphics/mesh.h"

void HealthMeter::setValue(float value) {
   this->value = value;
}

void HealthMeter::enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
   std::vector<Vertex2T2> vertices;
   vertices.reserve(10 * 4);
   
   rect size = renderer->getNativeSize();
   size.halfSize = size.halfSize;

   float x = 0.0f;
   float valuesLeft = value;
   
   for (int i = 0; i < 10; ++i) {
	  vec2 texPos = vec2::Zero;
	  vec2 texCellSize = vec2(1.0f/2.0f, 1.0f/2.0f);
	  
	  if (valuesLeft >= 10.0f)
		 texPos = vec2::Zero;
	  else if (valuesLeft >= 5.0f) {
		 if (dir >= 0.0f)
			texPos = vec2(1.0f/2.0f, 0.0f);
		 else
			texPos = vec2(1.0f/2.0f, 1.0f/2.0f);
	  }
	  else
		 texPos = vec2(0.0f, 1.0f/2.0f);

	  const vec2 heartOrigin = origin + vec2(x, 0.0f);
	  
	  vertices.push_back(Vertex2T2(size.halfSize * vec2(-1.0f, -1.0f) + heartOrigin, texPos));
	  vertices.push_back(Vertex2T2(size.halfSize * vec2(1.0f, -1.0f) + heartOrigin, texPos + texCellSize * vec2(1.0f, 0.0f)));
	  vertices.push_back(Vertex2T2(size.halfSize * vec2(1.0f, 1.0f) + heartOrigin, texPos + texCellSize));
	  vertices.push_back(Vertex2T2(size.halfSize * vec2(-1.0f, 1.0f) + heartOrigin, texPos + texCellSize * vec2(0.0f, 1.0f)));
	  
	  x += 17.0f * dir;
	  valuesLeft -= 10.0f;
   }
   
   Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh); // TODO: this is really bad
   mesh->vertices = vertices;

   renderList->insert(renderer, mesh);
}

void HealthMeter::setPosition(const vec2 & origin) {
   this->origin = origin;
}

void HealthMeter::setRenderer(const Ref<Graphics::Renderer>::SharedPtr & newRenderer) {
   this->renderer = newRenderer;
}

void HealthMeter::setDir(float dir) {
   this->dir = dir;
}
