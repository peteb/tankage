#include <iostream>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <engine/graphics.h>

#include <game/background.h>
#include <game/snails.h>
#include <game/control.h>
#include <game/system.h>
#include <game/items.h>
#include <game/projectiles.h>

int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  wm->createWindow(800, 600);

  const int escape = input->keycode("escape");
  bool running = true;
  double lastTick = wm->timeSeconds();

  SystemContext systems;

  Background bkg(interfaces, &systems);
  Snails snails(interfaces, &systems);
  Control control(interfaces, &systems);
  Items items(interfaces, &systems);
  Projectiles projectiles(interfaces, &systems);
  
  systems.set(&snails);
  systems.set(&bkg);
  systems.set(&control);
  systems.set(&items);
  systems.set(&projectiles);
  systems.init();
  
  while (running) {      
    double thisTick = wm->timeSeconds();
    float dt = std::max(thisTick - lastTick, 0.00001);

    const rect wndSize = wm->size();
    
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);

    control.update();
    bkg.render();
    snails.render();
    items.update();
    items.render();
    projectiles.update();
    
    wm->swapBuffers();
    lastTick = thisTick;
    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }

  return EXIT_SUCCESS;
}
