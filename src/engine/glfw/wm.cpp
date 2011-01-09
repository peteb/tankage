#include <engine/glfw/wm.h>
#include <iostream>
#include "GL/glfw.h"

#include <cstdlib>

/// GLFW callbacks
namespace {
void GLFWCALL WindowResize(int w, int h) {
  Glfw::WindowManager::instance()._size = rect(w, h);
}

void GLFWCALL KeyStateChange(int key, int state) {
}
}

Glfw::WindowManager::WindowManager() {
}

rect Glfw::WindowManager::size() const {
  return _size;
}

void Glfw::WindowManager::createWindow(int width, int height) {
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
  
  if (!glfwOpenWindow(width, height, 0, 0, 0,
                      0, 24, 0, GLFW_WINDOW)) {
    glfwTerminate();
    std::cerr << "glfw: failed to open window" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  glfwSetWindowSizeCallback(WindowResize);
  glfwSetKeyCallback(KeyStateChange);
  glfwSetWindowTitle("Snail Wail");
  std::cout << "glfw: window opened" << std::endl;
}

double Glfw::WindowManager::timeSeconds() {
  return glfwGetTime();
}

void Glfw::WindowManager::swapBuffers() {
  glfwSwapBuffers();
}

bool Glfw::WindowManager::windowState(WindowState state) {
  int glfwParam;
  switch (state) {
  case Glfw::WindowManager::OPENED:
    glfwParam = GLFW_OPENED;
    break;
    
  case Glfw::WindowManager::ACTIVE:
    glfwParam = GLFW_ACTIVE;
    break;
    
  case Glfw::WindowManager::ICONIFIED:
    glfwParam = GLFW_ICONIFIED;
    break;
    
  default:
    return false;
  }
  
  return glfwGetWindowParam(glfwParam);
}

