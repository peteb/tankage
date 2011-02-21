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

#include <cstdlib>
#include <memory>


int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  wm->createWindow(800, 600);

  
  SystemContext systems;

  // Register the subsystems
  GameServer server;
  Actors actors;
  Players players;
  Control control;
  
  server.registerSystem(&actors);
  server.registerSystem(&players);
  server.registerSystem(&control);
  
  systems.set(SystemContext::SYSTEM_ACTORS, &actors);
  systems.set(SystemContext::SYSTEM_GAMESERVER, &server);
  systems.set(SystemContext::SYSTEM_PLAYERS, &players);
  systems.set(SystemContext::SYSTEM_CONTROL, &control);
  
  systems.init(interfaces);

  double lastTurn = wm->timeSeconds();
  double lastTick = wm->timeSeconds();
  int upOrDown = 0;
  
  while (1) {
    double thisTime = wm->timeSeconds();
    
    const rect wndSize = wm->size();
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);

    
    server.update();
    actors.render();

    if (thisTime - lastTick >= 1.0/25.0) { // Tickrate
      server.tick(thisTime - lastTick);
      lastTick = thisTime;
    }
    
    wm->swapBuffers(); 
  }
  
  return EXIT_SUCCESS;
}

