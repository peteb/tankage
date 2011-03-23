#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <game/common/system.h>

class Input;

class Control : public System {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_CONTROL;
  }  
  
  struct Input {
    unsigned char buttons;
    
    Control::Input &write(class Packer &msg);
    Control::Input &read(class Unpacker &msg);

    enum {
      FORWARD = 0x01
    };
  };

  Control();
  
  void init(const class Portal &interfaces);
  void start();
};

#endif // !GAME_CONTROL_H
