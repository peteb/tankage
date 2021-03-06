#ifndef PLATFORM_INPUT_H
#define PLATFORM_INPUT_H

#include <platform/interface.h>

class Input : public Interface {
public:
  static std::string id() {
    return "input";
  }

  virtual int keycode(const std::string &key) =0;
  virtual bool keyPressed(int key) =0;

  /// Returns true if the key has been pressed since the last invocation.
  virtual bool keyWasPressed(int key) =0;

  /// Returns true if the key has been released since the last invocation.
  virtual bool keyWasReleased(int key) =0;

  virtual void mousePos(int &x, int &y) =0;
};

#endif // !PLATFORM_WINDOW_MANAGER_H
