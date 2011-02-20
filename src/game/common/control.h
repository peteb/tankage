#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <game/common/system.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/common/replicated_system.h>

#include <string>

class Control : public ReplicatedSystem {
public:
  Control();
  
  void init(const class Portal &interfaces);
  void update();
  
private:
  void triggerState(int keycode, Tank::State state, Tank *target);
  void onTick(class Client *client);
  
  class WindowManager *wm;
  double lastTick;
  int keyUp, keyDown, keyLeft, keyRight, keyShoot;
  class Input *input;
  std::string snail;
};

#endif // !GAME_CONTROL_H
