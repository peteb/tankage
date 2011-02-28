#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <game/common/system.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/common/replicated_system.h>

#include <string>
#include <map>

class Control : public ReplicatedSystem {
public:
  Control();
  
  void init(const class Portal &interfaces);
  void update();
  void onReceive(NetPacketType type, const class Packet &packet);
  const Tank::Input *lastInput(ActorId actor) const;
  
private:  
  void onTick(class Client *client);
  Tank::Input currentState() const;
  
  class WindowManager *wm;
  double lastTick;
  int keyUp, keyDown, keyLeft, keyRight, keyShoot;
  class Input *input;
  std::string snail;

  std::map<ActorId, Tank::Input> states;
  // data that will be replicated
  double inputBegan;
  Tank::Input state;
};

#endif // !GAME_CONTROL_H
