#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/network.h>

#include <game/client/background.h>
#include <game/client/particles.h>
#include <game/client/gameclient.h>
#include <game/common/actors.h>
#include <game/common/control.h>
#include <game/common/system.h>
#include <game/common/projectiles.h>
#include <game/common/texture_loader.h>
#include <game/common/players.h>
#include <game/common/config.h>

#include <utils/tanklog.h>

#include <cstdlib>
#include <iostream>
#include <ctime>


struct client_log_consumer { 
  void operator()(tankage::tanklog::severity_t severity, const std::string &line) {
    if (severity > tankage::tanklog::severity_info) {
	  std::cout << line << std::endl;
    }
  }
};

int app_main(Portal &interfaces, const std::vector<char *> &args) {
  // create and register extra client consumer
  tankage::tanklog::register_consumer(client_log_consumer());
  tanklog(info) << "Starting-up the client =]";

  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  
  wm->createWindow(800, 600);
  
  SystemContext systems;

  /*
    Proposal:
   systems.registerSystem<Config>();
   systems.registerSystem<GameClient>();
   systems.registerSystem<Actors>();
   systems.registerSystem<Players>();
   systems.registerSystem<Background>();
   systems.registerSystem<Control>();
   systems.registerSystem<Projectiles>();
   systems.registerSystem<Particles>();
   systems.registerSystem<TextureLoader>();

   Actors::init can register in gameclient
   
   GameClient &client = systems.system<GameClient>();
   
   */
  
  
  
  // Register the subsystems
  Config config;
  GameClient gameclient;
  Actors actors;
  Players players;
  Background background;
  Control control;
  Projectiles projectiles;
  Particles particles;
  TextureLoader texLoader;
  
  gameclient.registerSystem(&actors);
  gameclient.registerSystem(&players);
  gameclient.registerSystem(&control);
  gameclient.registerSystem(&projectiles);
  
  // TODO: this is fugly, registering like this. maybe it should be done
  //       like gameClient above
  
  systems.set(SystemContext::SYSTEM_BACKGROUND, &background);
  systems.set(SystemContext::SYSTEM_ACTORS, &actors);
  systems.set(SystemContext::SYSTEM_PROJECTILES, &projectiles);
  systems.set(SystemContext::SYSTEM_GAMECLIENT, &gameclient);
  systems.set(SystemContext::SYSTEM_PLAYERS, &players);
  systems.set(SystemContext::SYSTEM_CONTROL, &control);
  systems.set(SystemContext::SYSTEM_PARTICLES, &particles);
  systems.set(SystemContext::SYSTEM_TEXTURE_LOADER, &texLoader);
  systems.set(SystemContext::SYSTEM_CONFIG, &config);
  
  systems.init(interfaces);
  config.parse(args);
  systems.start();
  
  double lastTick = wm->timeSeconds();
  bool running = true;
  const int escape = input->keycode("escape");
  
  while (running) {
    double thisTime = wm->timeSeconds();
    const rect wndSize = wm->size();
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);

    control.update();
    background.render();
    gameclient.update();

    gameclient.tick(thisTime - lastTick);
    lastTick = thisTime;
    
    //   particles.render();
    actors.render();
    projectiles.update();
    projectiles.render();
    
    wm->swapBuffers();

    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }


  gameclient.disconnectGently();

  return EXIT_SUCCESS;
}
