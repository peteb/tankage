#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <game/common/system.h>
#include <game/common/snails.h>
#include <game/common/tank.h>

#include <string>

class Control : public System {
public:
  void init(const class Portal &interfaces);
  void update();
  
private:
  void triggerState(int keycode, Snail::SnailState state);
  
  int keyUp, keyDown, keyLeft, keyRight, keyShoot;
  class Input *input;
  std::string snail;
};

#endif // !GAME_CONTROL_H
