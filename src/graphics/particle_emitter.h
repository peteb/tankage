/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_PARTICLE_EMITTER_H
#define GRAPHICS_PARTICLE_EMITTER_H

#include "updatable.h"
#include "ref.h"

namespace Graphics {

class ParticleSystem;
   
class ParticleEmitter : public Updatable {
public:
   void setParticleSystem(const Ref<ParticleSystem>::WeakPtr &psys);
   
   void update(float dt);

private:
   Ref<ParticleSystem>::WeakPtr particleSystem;
};

}

#endif // !GRAPHICS_PARTICLE_EMITTERH
