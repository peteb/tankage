#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

class Input;

void control_RegisterVariables(class Config &config);

class Control {
public:  
  struct Input {
    unsigned char buttons;
    
    Control::Input &write(class Packer &msg);
    Control::Input &read(class Unpacker &msg);

    enum {
      FORWARD = 0x01
    };
  };

  Control(const class Portal &interfaces);

  void start();
};

#endif // !GAME_CONTROL_H
