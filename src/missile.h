/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef MISSILE_H
#define MISSILE_H

#include "ref.h"
#include "updatable.h"
#include "projectile.h"

class Snail;

namespace Graphics {
class ParticleEmitter;
}

class Missile : public Projectile, public Updatable {
public:
  Missile();
   
  void update(float dt);   
  void setFuel(float fuel);
  
  Ref<Snail>::WeakPtr target;
  Ref<Graphics::ParticleEmitter> smokeEmitter;
  
private:
  float timeSinceUpdate, fuel;
};

#endif // !MISSILE_H
