#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <string>
#include <game/common/system.h>
#include <game/common/snails.h>

class Control : public System {
public:
  void init(const class Portal &interfaces);
  void update();
  
private:
  void triggerState(int keycode, Snail::SnailState state);
  
  int keyUp, keyDown, keyLeft, keyRight, keyShoot;
  class Input *input;
  class Cfg *cfg;
  std::string snail;
};

#endif // !GAME_CONTROL_H
