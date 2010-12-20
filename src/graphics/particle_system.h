/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_PARTICLE_SYSTEM_H
#define GRAPHICS_PARTICLE_SYSTEM_H

#include "math/vec2.h"
#include "ref.h"

#include <vector>

namespace Graphics {

class RenderList;
class Renderer;
   
struct Particle {
   vec2 pos;
   float ttd;
};
   
class ParticleSystem {
public:
   void setRenderer(const Ref<Graphics::Renderer>::SharedPtr &renderer);
   void addParticle(const vec2 &pos);
   void enqueueVertices(const Ref<RenderList>::SharedPtr &renderList, float dt);

private:
   std::vector<Particle> particles;
   Ref<Graphics::Renderer>::SharedPtr renderer;
};



}

#endif // !GRAPHICS_PARTICLE_SYSTEM_H
