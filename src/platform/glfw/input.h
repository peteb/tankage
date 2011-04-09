#ifndef PLATFORM_GLFW_INPUT_H
#define PLATFORM_GLFW_INPUT_H

#include <platform/input.h>
#include <utils/singleton.h>
#include <vector>
#include <utility>

namespace Glfw {
// This module is dependant on glfw/wm and glfw/starter
class Input : public ::Input, public Singleton<Input> {
public:
  bool keyPressed(int key);
  int keycode(const std::string &key);

  bool keyWasPressed(int key);
  bool keyWasReleased(int key);

  void onKeyStateChange(int key, int state);
  void mousePos(int &x, int &y);  

private:
  enum {
    KEY_STATE_PRESSED  = 0x01,
    KEY_STATE_RELEASED = 0x02
  };
    
  typedef std::vector<std::pair<int, unsigned char> > KeyStateMap;
  typedef std::pair<KeyStateMap::iterator, KeyStateMap::iterator> KeyStateRange;
  
  KeyStateMap keyStates;

  KeyStateRange findKeyStates(int key);
};

}

#endif // !PLATFORM_GLFW_INPUT_H
