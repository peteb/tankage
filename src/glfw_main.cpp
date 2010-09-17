/*
 * $Id$
 * neon afterlife
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include "glfw.h"
#include "game.h"

static void main_loop(Game & game);

int main(int argc, char ** argv) {
   if (!glfwInit()) {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return EXIT_FAILURE;
   }
   
   if (!glfwOpenWindow(800, 600, 0, 0, 0, 0, 24, 0, GLFW_WINDOW)) {
      glfwTerminate();
      std::cerr << "Failed to open GLFW window" << std::endl;
      return EXIT_FAILURE;
   }
   
   glfwSetWindowTitle("Neon Afterlife");
   std::cout << "Initialized glfw" << std::endl;
   
   Game game;
   main_loop(game);
   
   glfwTerminate();
}

void main_loop(Game & game) {
   bool running = true;
   float last_tick = glfwGetTime();
   
   while (running) {
      float this_tick = glfwGetTime();
      float dt = this_tick - last_tick;
      
      game.tick(dt);
      glfwSwapBuffers();
      
      last_tick = this_tick;
      running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
   }
}
