#include <engine/portal.h>
#include <engine/network.h>

#include <game/server/gameserver.h>
#include <game/common/system.h>

#include <cstdlib>
#include <memory>


int app_main(Portal &interfaces) {
  SystemContext systems;

  // Register the subsystems
  GameServer server;

  systems.set(SystemContext::SYSTEM_GAMESERVER, &server);

  systems.init(interfaces);
  
  while (1) {
    server.update();
    
     
  }
  
  return EXIT_SUCCESS;
}

