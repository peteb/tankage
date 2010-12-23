/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "particle_system.h"
#include "graphics/render_list.h"
#include "graphics/mesh.h"
#include "graphics/color.h"

#include <algorithm>

void Graphics::ParticleSystem::setRenderer(const Ref<Graphics::Renderer>::SharedPtr &renderer) {
  this->renderer = renderer;
}

void Graphics::ParticleSystem::addParticle(const vec2 &pos, const vec2 &velocity) {
  particles.push_back((Graphics::Particle){pos, velocity, 1.0f});
}

void Graphics::ParticleSystem::enqueueVertices(const Ref<RenderList>::SharedPtr &renderList, float dt) {
  std::vector<Vertex2T2> vertices;
  const vec2 halfSize(6.0f, 6.0f);
   
  size_t e = particles.size();
  
  for (size_t i = 0; i < e; ) {
    Graphics::Particle &particle = particles[i];

    if (particle.ttd <= 0.0f) {
      particles[i] = particles[std::max(1U, e) - 1UL];
      --e;
      continue;
    }
      
    const vec2 &pos = particle.pos;
    Graphics::Color color = Graphics::Color::White;
    color.a = (particle.ttd - 0.4f) / 1.0f;
    float scale = 0.8f + (1.2f - particle.ttd) * 1.5f;
    
    vertices.push_back(Vertex2T2(pos + halfSize * vec2(-1.0f, -1.0f) * scale, vec2(0.0f, 0.0f), color));
    vertices.push_back(Vertex2T2(pos + halfSize * vec2( 1.0f, -1.0f) * scale, vec2(1.0f, 0.0f), color));
    vertices.push_back(Vertex2T2(pos + halfSize * vec2( 1.0f,  1.0f) * scale, vec2(1.0f, 1.0f), color));
    vertices.push_back(Vertex2T2(pos + halfSize * vec2(-1.0f,  1.0f) * scale, vec2(0.0f, 1.0f), color));

    particle.pos += particle.vel * dt;
    particle.ttd -= dt;
    ++i;
  }

  particles.erase(particles.begin() + e, particles.end());
  
  Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh);
  mesh->vertices = vertices;
  renderList->insert(renderer, mesh);
}

