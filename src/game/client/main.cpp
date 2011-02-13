#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/network.h>

#include <game/client/background.h>
#include <game/client/particles.h>
#include <game/client/gameclient.h>
#include <game/common/snails.h>
#include <game/common/control.h>
#include <game/common/system.h>
#include <game/common/items.h>
#include <game/common/texture_loader.h>
#include <game/common/players.h>

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
  Snails snails;
  Players players;
  
  gameclient.registerSystem(&snails);
  gameclient.registerSystem(&players);
  
  systems.set(SystemContext::SYSTEM_SNAILS, &snails);
  systems.set(SystemContext::SYSTEM_GAMECLIENT, &gameclient);
  systems.set(SystemContext::SYSTEM_PLAYERS, &players);
  
  systems.init(interfaces);

  bool running = true;
  const int escape = input->keycode("escape");
  
  while (running) {          
    gameclient.update();
    
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
