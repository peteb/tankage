/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "particle_emitter.h"

void Graphics::ParticleEmitter::setParticleSystem(const Ref<ParticleSystem>::WeakPtr &psys) {
   particleSystem = psys;
}

void Graphics::ParticleEmitter::update(float dt) {
   
}
