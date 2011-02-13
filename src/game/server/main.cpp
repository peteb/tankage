#include <engine/portal.h>
#include <engine/network.h>
#include <engine/window_manager.h>

#include <game/server/gameserver.h>
#include <game/common/system.h>
#include <game/common/snails.h>
#include <game/common/players.h>

#include <cstdlib>
#include <memory>


int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  
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
  
  while (1) {
    server.update();
    
     
  }
  
  return EXIT_SUCCESS;
}

