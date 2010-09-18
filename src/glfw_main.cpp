/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include "glfw.h"
#include "game.h"

static void main_loop(Game & game);
static Game * gameInstance = 0;

static void GLFWCALL WindowResize(int width, int height) {
   if (gameInstance)
      gameInstance->windowChangedSize(width, height);
}

int main(int argc, char ** argv) {
   if (!glfwInit()) {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return EXIT_FAILURE;
   }
   
   const int windowWidth = 800, windowHeight = 600;
   
   if (!glfwOpenWindow(windowWidth, windowHeight, 0, 0, 0, 0, 24, 0, GLFW_WINDOW)) {
      glfwTerminate();
      std::cerr << "Failed to open GLFW window" << std::endl;
      return EXIT_FAILURE;
   }
   
   glfwSetWindowSizeCallback(WindowResize);
   glfwSetWindowTitle("Neon Afterlife");
   std::cout << "Initialized glfw" << std::endl;
   
   Game game;
   gameInstance = &game;
   game.windowChangedSize(windowWidth, windowHeight);
   main_loop(game);
   gameInstance = 0;
   glfwTerminate();
}

void main_loop(Game & game) {
   bool running = true;
   float last_tick = glfwGetTime();
   int lastWidth = -1, lastHeight = -1;
   
   while (running) {      
      float this_tick = glfwGetTime();
      float dt = this_tick - last_tick;
      
      game.tick(dt);
      glfwSwapBuffers();
      
      last_tick = this_tick;
      running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
   }
}
