#include <engine/portal.h>
#include <engine/network.h>
#include <engine/window_manager.h>
#include <engine/graphics.h>

#include <game/server/gameserver.h>
#include <game/common/system.h>
#include <game/common/actors.h>
#include <game/common/players.h>
#include <game/common/tank.h>
#include <game/common/control.h>
#include <game/common/projectiles.h>
#include <game/common/texture_loader.h>
#include <game/common/config.h>
#include <game/client/particles.h>

#include <cstdlib>
#include <memory>


int app_main(Portal &interfaces, const std::vector<char *> &args) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  wm->createWindow(800, 600);

  
  SystemContext systems;
  
  // Register the subsystems
  Config *config = systems.registerSystem<Config>();
  GameServer *server = systems.registerSystem<GameServer>();
  Actors *actors = systems.registerSystem<Actors>();
  Players *players = systems.registerSystem<Players>();
  Control *control = systems.registerSystem<Control>();
  Projectiles *projectiles = systems.registerSystem<Projectiles>();
  systems.registerSystem<Particles>();
  systems.registerSystem<TextureLoader>();
  
  // FIXME: requestSystem, use it everywhere
  // FIXME: baseclass for game[client|server]
  // FIXME: context->systemReplicator = game[client|server]
  // FIXME: actors, players, control, projectiles should register themselves
  // FIXME: same for server/main.cpp
  // FIXME: delete systems in SystemContext dtor
  // FIXME: system ctor should take portal and context
  
  systems.init(interfaces);
  config->parse(args);
  systems.start();
  
  double lastTick = wm->timeSeconds();
  
  while (1) {
    double thisTime = wm->timeSeconds();
    
    const rect wndSize = wm->size();
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);

    
    server->update();
    actors->render();
    projectiles->update();
    projectiles->render();
    
    if (thisTime - lastTick >= 1.0/20.0) { // Tickrate
      server->tick(thisTime - lastTick);
      lastTick = thisTime;
    }
    
    wm->swapBuffers(); 
  }
  
  return EXIT_SUCCESS;
}

