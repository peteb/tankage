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

#include <utils/log.h>

#include <cstdlib>
#include <iostream>
#include <ctime>


int app_main(Portal &interfaces, const std::vector<char *> &args) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  
  wm->createWindow(800, 600);
  
  SystemContext systems;
  
  // Register the subsystems
  Config *config = systems.registerSystem<Config>();
  GameClient *gameclient = systems.registerSystem<GameClient>();
  Actors *actors = systems.registerSystem<Actors>();
  systems.registerSystem<Players>();
  Control *control = systems.registerSystem<Control>();
  Projectiles *projectiles = systems.registerSystem<Projectiles>();
  Background *background = systems.registerSystem<Background>();
  systems.registerSystem<Particles>();
  systems.registerSystem<TextureLoader>();
  
  systems.init(interfaces);
  config->parse(args);
  systems.start();
  
  double lastTick = wm->timeSeconds();
  bool running = true;
  const int escape = input->keycode("escape");
  
  while (running) {
    double thisTime = wm->timeSeconds();
    const rect wndSize = wm->size();
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);

    control->update();
    background->render();
    gameclient->update();

    gameclient->tick(thisTime - lastTick);
    lastTick = thisTime;
    
    //   particles.render();
    actors->render();
    projectiles->update();
    projectiles->render();
    
    wm->swapBuffers();

    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }


  gameclient->disconnectGently();

  return EXIT_SUCCESS;
}
