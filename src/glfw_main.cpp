/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include <map>

#include "glfw.h"
#include "game.h"

static void main_loop(Game & game);
static Game * gameInstance = 0;

static void GLFWCALL WindowResize(int width, int height) {
   if (gameInstance)
      gameInstance->windowChangedSize(width, height);
}

static void GLFWCALL KeyStateChange(int key, int state) {
   if (gameInstance) {
      typedef std::map<int, std::string> KeyMap;
      KeyMap boundKeys;
      
      boundKeys[GLFW_KEY_UP] = "player1_up";
      boundKeys[GLFW_KEY_DOWN] = "player1_down";
      boundKeys[GLFW_KEY_LEFT] = "player1_left";
      boundKeys[GLFW_KEY_RIGHT] = "player1_right";
      boundKeys[GLFW_KEY_SPACE] = "player1_shoot";

	  boundKeys['W'] = "player2_up";
	  boundKeys['S'] = "player2_down";
	  boundKeys['D'] = "player2_right";
	  boundKeys['A'] = "player2_left";
	  boundKeys['Z'] = "player2_shoot";
	  
      std::string keyName = "unknown";
      KeyMap::const_iterator iter = boundKeys.find(key);
      if (iter != boundKeys.end())
         keyName = iter->second;
      
      gameInstance->keyStateChanged(keyName, (state == GLFW_PRESS ? 1 : 0));
      
      
   }
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
   glfwSetKeyCallback(KeyStateChange);
   glfwSetWindowTitle("Snail Wail");
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
