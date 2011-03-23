#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/network.h>

#include <game/client/tank_renderer.h>
#include <game/client/particles.h>
#include <game/client/gameclient.h>
//#include <game/common/control.h>
//#include <game/common/projectiles.h>
#include <game/common/texture_loader.h>
#include <game/common/players.h>
#include <game/common/config.h>
#include <game/common/control.h>

#include <utils/log.h>

#include <cstdlib>
#include <iostream>
#include <ctime>


int app_main(Portal &interfaces, const std::vector<char *> &args) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  
  wm->createWindow(800, 600);
  
  Config config(interfaces);
  client_RegisterVariables(config);
  control_RegisterVariables(config);
  config.parse(args); // read and set variables from cmdline
  
  GameClient client(interfaces);
  
  bool running = true;
  const int escape = input->keycode("escape");
  
  while (running) {
    const rect wndSize = wm->size();
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);
    
    client.update();
    wm->swapBuffers();
    
    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }
  
  client.disconnectGently();

  return EXIT_SUCCESS;
}
