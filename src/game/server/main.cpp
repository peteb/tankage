#include <engine/portal.h>
#include <engine/network.h>
#include <engine/window_manager.h>
#include <engine/graphics.h>

#include <game/server/gameserver.h>
#include <game/common/system.h>
#include <game/common/snails.h>
#include <game/common/players.h>

#include <cstdlib>
#include <memory>


int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  wm->createWindow(800, 600);

  
  SystemContext systems;

  // Register the subsystems
  GameServer server;
  Snails snails;
  Players players;
  
  server.registerSystem(&snails);
  server.registerSystem(&players);
  
  systems.set(SystemContext::SYSTEM_SNAILS, &snails);
  systems.set(SystemContext::SYSTEM_GAMESERVER, &server);
  systems.set(SystemContext::SYSTEM_PLAYERS, &players);
  
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
    snails.render();

    if (thisTime - lastTurn > 2.0) {
      if (upOrDown == 0) {
        snails.snail(Snails::SNAIL_RIGHT)->stopState(Snail::STATE_MOVE_DOWN);
        snails.snail(Snails::SNAIL_RIGHT)->startState(Snail::STATE_MOVE_UP);
        upOrDown = 1;
      }
      else {
        snails.snail(Snails::SNAIL_RIGHT)->stopState(Snail::STATE_MOVE_UP);
        snails.snail(Snails::SNAIL_RIGHT)->startState(Snail::STATE_MOVE_DOWN);
        upOrDown = 0;
      }
      lastTurn = thisTime;

    }

    if (thisTime - lastTick >= 1.0/25.0) { // Tickrate
      server.tick(thisTime - lastTick);
      lastTick = thisTime;
    }
    
    wm->swapBuffers(); 
  }
  
  return EXIT_SUCCESS;
}

