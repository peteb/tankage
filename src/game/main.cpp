#include <iostream>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <game/background.h>
#include <engine/graphics.h>

#include <cstdlib>

int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  wm->createWindow(800, 600);

  const int escape = input->keycode("escape");
  bool running = true;
  double lastTick = wm->timeSeconds();

  Background bkg(interfaces);
  
  while (running) {      
    double thisTick = wm->timeSeconds();
    float dt = std::max(thisTick - lastTick, 0.00001);

    const rect wndSize = wm->size();
    
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);
    
    bkg.render();

    wm->swapBuffers();
    lastTick = thisTick;
    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }

  return EXIT_SUCCESS;
}
