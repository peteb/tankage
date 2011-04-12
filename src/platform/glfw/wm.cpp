#include <platform/glfw/wm.h>
#include <platform/config.h>
#include <iostream>
#include <cstdlib>

#include "GL/glfw.h"

#ifdef PLATFORM_COCOA
#include <platform/cocoa/dialogs.h>
#endif

namespace Glfw {
void KeyStateChange(int key, int state);
}

/// GLFW callbacks
namespace {
void GLFWCALL WindowResize(int w, int h) {
  Glfw::WindowManager::instance()._size = rect(w, h);
}

void GLFWCALL KeyStateChange(int key, int state) {
  Glfw::KeyStateChange(key, state);
}

void GLFWCALL MouseButtonChange(int key, int state) {
  Glfw::KeyStateChange(GLFW_KEY_LAST + key, state);
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
  glfwSetMouseButtonCallback(MouseButtonChange);

  #ifdef DEV
  glfwSetWindowTitle("Snail Wail [DEVELOPER VERSION]");
  #else
  glfwSetWindowTitle("The Age of Tanks");
  #endif

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

void Glfw::WindowManager::displayError(
  const char *title, const char *description) {
  #ifdef PLATFORM_COCOA
  cocoa_error_window(title, description);  
  #else
  std::cerr << "glfw: ERROR (" << title << ") " << description << std::endl;
  #endif
}
