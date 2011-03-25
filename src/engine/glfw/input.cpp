#include <engine/glfw/input.h>
#include <stdexcept>
#include <iostream>
#include "GL/glfw.h"
#include <utils/key.h>
#include <algorithm>

// Called by glfw/wm.cpp
namespace Glfw {
void GLFWCALL KeyStateChange(int key, int state) {
  Glfw::Input::instance().onKeyStateChange(key, state);
}
}


bool Glfw::Input::keyPressed(int key) {
  // return glfwGetKey(key) == GLFW_PRESS;
  KeyStateRange keyRange = findKeyStates(key);
  if (keyRange.first == keyRange.second) {
    // Key has not received any updates
    return false;
  }

  return keyRange.first->second & KEY_STATE_PRESSED;
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
  else if (key.size() == 1) // handle single-letter names (ASCII)
    return key[0];
  else if (key == "mouse1")
    return GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LEFT;
  
  throw std::runtime_error("glfw: no keycode for '" + key + "'");
}


void Glfw::Input::onKeyStateChange(int key, int state) {
  KeyStateRange stateElements = findKeyStates(key);

  if (stateElements.first != stateElements.second) {
    unsigned char &foundState = stateElements.first->second;
  
    if (state) {
      foundState = KEY_STATE_PRESSED;
    }
    else {
      foundState |= KEY_STATE_RELEASED;
    }
  }
  else {
    // This key has not received state updates earlier, add it.
    unsigned char initialState = (state ? KEY_STATE_PRESSED : 0);
    
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

  unsigned char &state = keyRange.first->second;
  const bool wasPressed = state & KEY_STATE_PRESSED;

  if (state & KEY_STATE_RELEASED)
    state = 0;

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

void Glfw::Input::mousePos(int &x, int &y) {
  glfwGetMousePos(&x, &y);
}
