#include <iostream>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <game/background.h>

int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  
  wm->createWindow(800, 600);

  const int escape = input->keycode("escape");
  bool running = true;
  double lastTick = wm->timeSeconds();

  Background bkg(interfaces);
  
  while (running) {      
    double thisTick = wm->timeSeconds();
    float dt = std::max(thisTick - lastTick, 0.00001);
    
    //  game.tick(dt);
    bkg.render();
    wm->swapBuffers();
    lastTick = thisTick;
    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }

  return EXIT_SUCCESS;
}
