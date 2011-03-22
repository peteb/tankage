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
 // Players *players = systems.registerSystem<Players>();
 // Control *control = systems.registerSystem<Control>();
 // Projectiles *projectiles = systems.registerSystem<Projectiles>();
 // systems.registerSystem<Particles>();
 // systems.registerSystem<TextureLoader>();
  
  systems.init(interfaces);
  config->parse(args);
  systems.start();
  
  /*
    TODO:
      * new config should be saved
      * unittest packer
      * think about how removing objects will be handled (code and net)
        * hint the client that the object is removed (send ACTORS_REMOVED packet or similar)
        * client can remove object if not updated within a set time
        * if a hint was wrong (started to receive updates), recreate the object
      * start using packer instead of structs
      * start using new log
      * 
   
   */
  const rect wndSize = wm->size();
  gfx->setViewport(wndSize);
  gfx->setOrtho(wndSize);
  wm->swapBuffers(); 
  
  server->run();
    
  return EXIT_SUCCESS;
}

