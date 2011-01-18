#ifndef ENGINE_GLFW_INPUT_H
#define ENGINE_GLFW_INPUT_H

#include <engine/input.h>
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
  
private:
  enum {
    KEY_STATE_PRESSED  = 0x01,
    KEY_STATE_RELEASED = 0x02
  };
    
  typedef unsigned char KeyState;
  typedef std::vector<std::pair<int, KeyState> > KeyStateMap;
  typedef std::pair<KeyStateMap::iterator, KeyStateMap::iterator> KeyStateRange;
  
  KeyStateMap keyStates;

  KeyStateRange findKeyStates(int key);
};

}

#endif // !ENGINE_GLFW_INPUT_H
