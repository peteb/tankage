/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "particle_emitter.h"
#include "particle_system.h"
#include "coord_system2.h"

Graphics::ParticleEmitter::ParticleEmitter()
  : emitInterval(0.1f)
  , secondsSinceEmit(emitInterval)
{
}

void Graphics::ParticleEmitter::setParticleSystem(const Ref<ParticleSystem>::WeakPtr &psys) {
  particleSystem = psys;
}

void Graphics::ParticleEmitter::setCoordSystem(const Ref<CoordSystem2> &newCs) {
  origin = newCs;
}

void Graphics::ParticleEmitter::update(float dt) {
  if (secondsSinceEmit >= emitInterval) {
    emit();
    secondsSinceEmit = 0.0f;
  }
  else {
    secondsSinceEmit += dt;
  }
  
}

void Graphics::ParticleEmitter::emit() {
  if (Ref<ParticleSystem>::SharedPtr lockedParticles = particleSystem.lock()) {
    if (Ref<CoordSystem2>::SharedPtr lockedOrigin = origin.lock()) {
      const CoordSystem2::data_type &originTransform = origin->getTransform();
      lockedParticles->addParticle(originTransform.position, originTransform.orientation.getX() * 100.0f);
    }
  }
}
