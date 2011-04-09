#include "module.h"
#include "portal.h"
#include <iostream>

class WM : public ModuleInterface {
public:
  WM() : ModuleInterface("WindowManager") {}

  virtual void doStuff() {std::cout << "werked!" << std::endl; }
};

class GLFW : public WM {
public:
  void doStuff() {std::cout << "moo" << std::endl; }
};

int main() {
  Portal mainPortal;
  GLFW mod;
  
  mainPortal.registerInterface(mod);

  WM *wm_if = mainPortal.requestInterface<WM>();

  wm_if->doStuff();
  
}
