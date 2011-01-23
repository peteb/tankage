#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <string>
#include <game/system.h>

class Control : public System {
public:
  void init(const class Portal &interfaces);
  void update();
  
private:
  int keyUp, keyDown, keyShoot;
  class Input *input;
  std::string snail;
};

#endif // !GAME_CONTROL_H
