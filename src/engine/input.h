#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <engine/interface.h>

class Input : public Interface {
public:
  static std::string id() {
    return "input";
  }

  virtual int keycode(const std::string &key) =0;
  virtual bool keyPressed(int key) =0;
};

#endif // !ENGINE_WINDOW_MANAGER_H
