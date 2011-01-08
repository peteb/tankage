/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include <map>
#include "GL/glfw.h"
#include <cstdlib>

#include <engine/window_manager.h>
#include <engine/portal.h>

//static void main_loop(Game & game);
//static Game * gameInstance = 0;

class Glfw : public WindowManager {
public:
  static Glfw &instance() {
    static Glfw inst;
    return inst;
  }
  
  rect size() const {
    return _size;
  }

  static void GLFWCALL WindowResize(int w, int h) {
    instance()._size = rect(w, h);
  }

  static void GLFWCALL KeyStateChange(int key, int state) {
  }
  
private:
  Glfw() {}

  rect _size;
}; 


int main(int argc, char **argv) {
  Portal interfaces;

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return EXIT_FAILURE;
  }

  interfaces.registerInterface<Glfw>();
  
  const int windowWidth = 800, windowHeight = 600;
  
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
  
  if (!glfwOpenWindow(windowWidth, windowHeight, 0, 0, 0, 0, 24, 0, GLFW_WINDOW)) {
    glfwTerminate();
    std::cerr << "Failed to open GLFW window" << std::endl;
    return EXIT_FAILURE;
  }
  
  
  glfwSetWindowSizeCallback(Glfw::WindowResize);
  glfwSetKeyCallback(Glfw::KeyStateChange);
  glfwSetWindowTitle("Snail Wail");
  std::cout << "Initialized glfw" << std::endl;

  std::cout << std::string(interfaces.requestInterface<WindowManager>()->size().origin) << std::endl;
  
//   Game game;
//   gameInstance = &game;
//   game.windowChangedSize(windowWidth, windowHeight);
//   main_loop(game);
//  gameInstance = 0;
  glfwTerminate();
}

// void main_loop(Game & game) {
//   bool running = true;
//   float last_tick = static_cast<float>(glfwGetTime());
//   int lastWidth = -1, lastHeight = -1;
//   
//   while (running) {      
//     float this_tick = glfwGetTime();
//     float dt = this_tick - last_tick;
//     
//     //  game.tick(dt);
//     glfwSwapBuffers();
//     
//     last_tick = this_tick;
//     running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
//   }
// }
