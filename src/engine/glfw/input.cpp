#include <engine/glfw/input.h>
#include <stdexcept>
#include <iostream>
#include "GL/glfw.h"
#include <utils/key.h>
#include <algorithm>

// Called by glfw/wm.cpp
namespace Glfw {
void KeyStateChange(int key, int state) {
  Glfw::Input::instance().onKeyStateChange(key, state);
}
}


bool Glfw::Input::keyPressed(int key) {
  return glfwGetKey(key) == GLFW_PRESS;
}

int Glfw::Input::keycode(const std::string &key) {
  // FIXME: a better mapping for these, possibly generated?
  if (key == "escape")
    return GLFW_KEY_ESC;
  else if (key == "up")
    return GLFW_KEY_UP;
  else if (key == "down")
    return GLFW_KEY_DOWN;
  else if (key == "right")
    return GLFW_KEY_RIGHT;
  else if (key == "left")
    return GLFW_KEY_LEFT;
  else if (key == "space")
    return GLFW_KEY_SPACE;
  
  throw std::runtime_error("glfw: no keycode for '" + key + "'");
}


void Glfw::Input::onKeyStateChange(int key, int state) {
  KeyStateRange stateElements = findKeyStates(key);

  if (stateElements.first != stateElements.second) {
    KeyState &foundState = stateElements.first->second;
    foundState |= (state ? KEY_STATE_PRESSED : KEY_STATE_RELEASED);
  }
  else {
    // This key has not received state updates earlier, add it.
    KeyState initialState = (state ? KEY_STATE_PRESSED : KEY_STATE_RELEASED);
    
    keyStates.insert(stateElements.first, std::make_pair(key, initialState));
    std::sort(keyStates.begin(), keyStates.end(), key_cmp());
  }
}

bool Glfw::Input::keyWasPressed(int key) {
  KeyStateRange keyRange = findKeyStates(key);
  if (keyRange.first == keyRange.second) {
    // Key has not received any updates
    return false;
  }
  
  const bool wasPressed = keyRange.first->second & KEY_STATE_PRESSED;
  keyRange.first->second &= ~KEY_STATE_PRESSED;
  return wasPressed;
}

bool Glfw::Input::keyWasReleased(int key) {
  KeyStateRange keyRange = findKeyStates(key);
  if (keyRange.first == keyRange.second) {
    // Key has not received any updates
    return false;
  }
  
  const bool wasReleased = keyRange.first->second & KEY_STATE_RELEASED;
  keyRange.first->second &= ~KEY_STATE_RELEASED;
  return wasReleased;
}

Glfw::Input::KeyStateRange Glfw::Input::findKeyStates(int key) {
  return std::equal_range(keyStates.begin(),
                          keyStates.end(),
                          key,
                          key_cmp()
    );
}
