#include <netinet/in.h>
#include <game/common/control.h>
#include <game/common/players.h>
#include <game/common/config.h>

#include <game/server/client_session.h>
#include <game/server/gameserver.h>
#include <game/client/gameclient.h>

#include <engine/input.h>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/network.h>

#include <utils/vec.h>
#include <utils/packer.h>

#include <arpa/inet.h>
#include <iostream>

Variable<std::string> control_keyUp("W");
Variable<std::string> control_keyDown("S");
Variable<std::string> control_keyLeft("A");
Variable<std::string> control_keyRight("D");
Variable<std::string> control_keyShoot("mouse1");

void control_RegisterVariables(class Config &config) {
  config.registerVariable("control", "key_up", &control_keyUp);
  config.registerVariable("control", "key_down", &control_keyDown);
  config.registerVariable("control", "key_left", &control_keyLeft);
  config.registerVariable("control", "key_right", &control_keyRight);
  config.registerVariable("control", "key_shoot", &control_keyShoot);    
}

Control::Control(const Portal &interfaces) {
  _input = interfaces.requestInterface< ::Input>();
  _wm = interfaces.requestInterface<WindowManager>();

  reloadKeycodes();
}

void Control::reloadKeycodes() {
  keyUp = _input->keycode(*control_keyUp);
  keyDown = _input->keycode(*control_keyDown);
  keyLeft = _input->keycode(*control_keyLeft);
  keyRight = _input->keycode(*control_keyRight);
  keyShoot = _input->keycode(*control_keyShoot);
}

/* <--- begin input state ---> */
Control::Input &Control::Input::write(class Packer &msg) {
  msg.writeShort(buttons); // fixme: writeByte
  return *this;
}

Control::Input &Control::Input::read(class Unpacker &msg) {
  buttons = msg.readShort();
  return *this;
}
/* <--- end input state ---> */


Control::Input Control::currentInput() const {
  Input ret;
  ret.buttons = 0;
/*  ret.aim_x = 0;
  ret.aim_y = 0;
  */
  struct {Input::Commands state; int keycode; } stateMap[] = {
    {Input::FORWARD, keyUp},
    {Input::BACKWARD, keyDown},
    {Input::TURN_RIGHT, keyRight},
    {Input::TURN_LEFT, keyLeft}/*,
    {Input::STATE_SHOOTING, keyShoot}*/};

  /*
    SHOOTING is not really a move. It should not have an impact on
    prediction.
    Also, don't create projectiles on client. Only work on server data.
    
   */
  
  // Map keycodes to bits
  for (size_t i = 0; i < 5; ++i) {
    const int keycode = stateMap[i].keycode;
    const Input::Commands state = stateMap[i].state;
      
    if (_input->keyPressed(keycode)) {
      ret.buttons |= state;
    }
  }
    
  //input->mousePos(ret.aim_x, ret.aim_y);

  return ret;
}

