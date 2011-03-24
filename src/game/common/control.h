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

    enum Commands {
      FORWARD =     0x01,
      BACKWARD =    0x02,
      TURN_RIGHT =  0x04,
      TURN_LEFT =   0x08
    };
  };

  Control(const class Portal &interfaces);

  Input currentInput() const;
  
private:
  void reloadKeycodes();
  
  class WindowManager *_wm;
  class ::Input *_input;
  
  int keyUp;
  int keyDown;
  int keyLeft;
  int keyRight;
  int keyShoot;
};

#endif // !GAME_CONTROL_H
