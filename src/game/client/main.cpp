
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <engine/graphics.h>
#include <engine/network.h>

#include <game/client/background.h>
#include <game/client/particles.h>
#include <game/common/snails.h>
#include <game/common/control.h>
#include <game/common/system.h>
#include <game/common/items.h>
#include <game/common/texture_loader.h>

#include <cstdlib>
#include <iostream>

int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  Network *net = interfaces.requestInterface<Network>();
  
  wm->createWindow(800, 600);

  bool connected = false;
  Client *client = net->connect("127.0.0.1:12345");

  const int escape = input->keycode("escape");
  bool running = true;
  
  while (running) {
    client->receive();
    if (connected != client->isConnected()) {
      connected = client->isConnected();
      std::cout << "client state changed: " << connected << std::endl;

      if (connected) {
        client->send("hej!", 5, Client::PACKET_RELIABLE, 0);
      }
    }

    wm->swapBuffers();
    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }

  // let the client down gently
  client->disconnect();
  while (client->isConnected()) {
    client->receive();
  }
  
  delete client;
  
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
