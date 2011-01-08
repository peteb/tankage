#include <iostream>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>

int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  
  wm->createWindow(800, 600);

  const int escape = input->keycode("escape");
  bool running = true;
  double lastTick = wm->timeSeconds();

  while (running) {      
    double thisTick = wm->timeSeconds();
    float dt = std::max(this_tick - last_tick, 0.00001);
    
    //  game.tick(dt);

    wm->swapBuffers();
    lastTick = thisTick;
    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }

  return EXIT_SUCCESS;
}
