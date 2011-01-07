/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_PARTICLE_SYSTEM_H
#define GRAPHICS_PARTICLE_SYSTEM_H

#include <utils/vec.h>
#include "ref.h"

#include <vector>

namespace Graphics {

class RenderList;
class Renderer;
   
struct Particle {
  vec2 pos;
  vec2 vel;
  float ttd;
};
   
class ParticleSystem {
public:
  void setRenderer(const Ref<Graphics::Renderer>::SharedPtr &renderer);
  void addParticle(const vec2 &pos, const vec2 &velocity);
  void enqueueVertices(const Ref<RenderList>::SharedPtr &renderList, float dt);

private:
  std::vector<Particle> particles;
  Ref<Graphics::Renderer>::SharedPtr renderer;
};



}

#endif // !GRAPHICS_PARTICLE_SYSTEM_H
