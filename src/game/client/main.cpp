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

#include <utils/ring_buffer.h>

#include <cstdlib>
#include <iostream>

int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  
  wm->createWindow(800, 600);
  
  SystemContext systems;

  // Register the subsystems
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

  systems.init(interfaces);

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

//    if (thisTime - lastTick >= 1.0/100.0) {
      gameclient.tick(thisTime - lastTick);
      lastTick = thisTime;
      //  }
    
    //   particles.render();
    actors.render();
    projectiles.update();
    projectiles.render();
    
    wm->swapBuffers();

    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }


  gameclient.disconnectGently();

  
  /*bool running = true;
  double lastTick = wm->timeSeconds();

  SystemContext systems;

  // Register the subsystems
  Background bkg;
  Snails snails;
  Control control;
  Items items;
  Particles particles;
  TextureLoader texLoader;
  
  systems.set(SystemContext::SYSTEM_SNAILS, &snails);
  systems.set(SystemContext::SYSTEM_BACKGROUND, &bkg);
  systems.set(SystemContext::SYSTEM_CONTROL, &control);
  systems.set(SystemContext::SYSTEM_ITEMS, &items);
  systems.set(SystemContext::SYSTEM_PARTICLES, &particles);
  systems.set(SystemContext::SYSTEM_TEXTURE_LOADER, &texLoader);
  
  systems.init(interfaces);
  
  while (running) {          
    const rect wndSize = wm->size();
    
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);

    control.update();
    bkg.render();
    particles.render();
    snails.render();
    items.update();
    items.render();
    
    wm->swapBuffers();

    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
      }*/

  return EXIT_SUCCESS;
}
