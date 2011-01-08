#include <engine/glfw/input.h>
#include <stdexcept>
#include "GL/glfw.h"

bool Glfw::Input::keyPressed(int key) {
  return glfwGetKey(key) == GLFW_PRESS;
}

int Glfw::Input::keycode(const std::string &key) {
  if (key == "escape")
    return GLFW_KEY_ESC;

  throw std::runtime_error("glfw: no keycode for '" + key + "'");
}

