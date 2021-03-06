#include <platform/portal.h>
#include <platform/window_manager.h>
#include <platform/input.h>
#include <platform/graphics.h>

#include <game/client/gameclient.h>
#include <game/common/config.h>
#include <game/common/control.h>

#include <utils/log.h>

#include <cstdlib>


int app_main(Portal &interfaces, const std::vector<char *> &args) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  wm->createWindow(800, 600);

  std::string wnd_caption = "Tankage (" BUILD_VERSION ")";
  wm->setWindowTitle(wnd_caption);
  
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
