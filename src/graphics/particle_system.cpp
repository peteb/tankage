/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "particle_system.h"
#include "graphics/render_list.h"
#include "graphics/mesh.h"
#include <algorithm>

void Graphics::ParticleSystem::setRenderer(const Ref<Graphics::Renderer>::SharedPtr &renderer) {
   this->renderer = renderer;
}

void Graphics::ParticleSystem::addParticle(const vec2 &pos) {
   particles.push_back((Graphics::Particle){pos, 2.0f});
}

void Graphics::ParticleSystem::enqueueVertices(const Ref<RenderList>::SharedPtr &renderList, float dt) {
   std::vector<Vertex2T2> vertices;
   const vec2 halfSize(8.0f, 8.0f);
   
   for (size_t i = 0, e = particles.size(); i < e; ) {
      Graphics::Particle &particle = particles[i];

      if (particle.ttd <= 0.0f) {
         particles[i] = particles[std::max(1UL, e) - 1UL];
         --e;
         continue;
      }
      
      const vec2 &pos = particle.pos;
      vertices.push_back(Vertex2T2(pos + halfSize * vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f)));
      vertices.push_back(Vertex2T2(pos + halfSize * vec2( 1.0f, -1.0f), vec2(1.0f, 0.0f)));
      vertices.push_back(Vertex2T2(pos + halfSize * vec2( 1.0f,  1.0f), vec2(1.0f, 1.0f)));
      vertices.push_back(Vertex2T2(pos + halfSize * vec2(-1.0f,  1.0f), vec2(0.0f, 1.0f)));

      particle.pos += vec2(200.0f, 200.0f) * dt;
      particle.ttd -= dt;
      ++i;
   }

   Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh);
   mesh->vertices = vertices;
   renderList->insert(renderer, mesh);
}

