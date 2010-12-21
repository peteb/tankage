/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GAME_H_RGVIQMD1
#define GAME_H_RGVIQMD1

#include <boost/shared_ptr.hpp>

#include "graphics/gfx_subsystem.h"
#include "physics/phys_subsystem.h"
#include "hi_controller.h"
#include "player_entity.h"
#include "object_creator.h"
#include "world.h"
#include "ref.h"
#include "health_meter.h"
#include "graphics/particle_system.h"
#include "graphics/particle_emitter.h"

class CactusGenerator;
class Snail;
 
class Game {
public:
  Game();
  ~Game();
   
  void tick(float dt);
  void windowChangedSize(int width, int height);
  void keyStateChanged(const std::string & key, int state);
   
private:
  Ref<Snail> firstSnail;
  Ref<Snail> secondSnail;
  Ref<CactusGenerator> cactusGenerator;
  Ref<Graphics::Renderer> defaultRenderer;
  HiController playerInput1, playerInput2;
  Ref<HealthMeter> snailHealth1, snailHealth2;

  Ref<Graphics::ParticleSystem> particles;
  Ref<Graphics::ParticleEmitter> emitter;
  
  bool drawGeoms;
  World world;
  ObjectCreator creator;
};

#endif /* end of include guard: GAME_H_RGVIQMD1 */
