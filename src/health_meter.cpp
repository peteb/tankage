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
#include <cmath>

HealthMeter::HealthMeter()
   : t(0.0f)
{
}

void HealthMeter::setValue(float value) {
   if (this->value >= 50.0f && value < 50.0f)
      t = 0.0f;
   
   this->value = value;
}

void HealthMeter::enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList, float dt) {
   std::vector<Vertex2T2> vertices;
   vertices.reserve(10 * 4);
   
   rect size = renderer->getNativeSize();
   size.halfSize = size.halfSize;

   float x = 0.0f;
   float valuesLeft = value;
   
   for (int i = 0; i < 10; ++i) {
	  vec2 texPos = vec2::Zero;
	  vec2 texCellSize = vec2(1.0f/2.0f, 1.0f/2.0f);
	  bool ghostHeart = false;

      // calculate texture coordinates
	  if (valuesLeft >= 10.0f) {
		 texPos = vec2::Zero;
      }
	  else if (valuesLeft >= 5.0f) {
		 if (dir >= 0.0f)
			texPos = vec2(1.0f/2.0f, 0.0f);
		 else
			texPos = vec2(1.0f/2.0f, 1.0f/2.0f);
	  }
	  else {
         ghostHeart = true;
		 texPos = vec2(0.0f, 1.0f/2.0f);
      }
      
	  const vec2 heartOrigin = origin + vec2(x, 0.0f);

      // calculate a scalar if we're low on health
      float scale = 1.0f;
      if (!ghostHeart && i < 4 && value <= 40.0f) {
         const float scaleSpeed = (60.0f - value) / 1.5f;
         scale = 1.25f + 0.25f * sin((1.0 + t) * scaleSpeed);
      }

      // create the mesh data
	  vertices.push_back(
         Vertex2T2(
            size.halfSize * vec2(-1.0f, -1.0f) * scale + heartOrigin,
            texPos
            )
         );
	  vertices.push_back(
         Vertex2T2(
            size.halfSize * vec2(1.0f, -1.0f) * scale + heartOrigin,
            texPos + texCellSize * vec2(1.0f, 0.0f)
            )
         );
	  vertices.push_back(
         Vertex2T2(
            size.halfSize * vec2(1.0f, 1.0f) * scale + heartOrigin,
            texPos + texCellSize
            )
         );
	  vertices.push_back(
         Vertex2T2(
            size.halfSize * vec2(-1.0f, 1.0f) * scale + heartOrigin,
            texPos + texCellSize * vec2(0.0f, 1.0f)
            )
         );
	  
	  x += 17.0f * dir;
	  valuesLeft -= 10.0f;
   }
   
   Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh);
   // TODO: this is really bad
   mesh->vertices = vertices;

   renderList->insert(renderer, mesh);
   t += dt;
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

