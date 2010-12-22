/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_PARTICLE_EMITTER_H
#define GRAPHICS_PARTICLE_EMITTER_H

#include "updatable.h"
#include "ref.h"

class CoordSystem2;

namespace Graphics {

class ParticleSystem;
   
class ParticleEmitter : public Updatable {
public:
  ParticleEmitter();
  virtual ~ParticleEmitter() {}
  
  void setParticleSystem(const Ref<ParticleSystem>::WeakPtr &psys);
  void setCoordSystem(const Ref<CoordSystem2> &newCs);
  
  void update(float dt);

protected:
  virtual void emit();
  
  Ref<ParticleSystem>::WeakPtr particleSystem;
  Ref<CoordSystem2> origin;

  float emitInterval;
  float secondsSinceEmit;
};

}

#endif // !GRAPHICS_PARTICLE_EMITTERH
