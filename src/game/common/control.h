#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <game/common/system.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/common/replicated_system.h>

#include <utils/ring_buffer.h>

#include <string>
#include <map>
#include <vector>
#include <utility>

class Control : public ReplicatedSystem {
public:
  struct Move {
    Tank::Input delta;
    Tank::State absolute;
  };

  typedef ring_buffer<Move> MoveBuffer;
  typedef std::pair<MoveBuffer::iterator, MoveBuffer::iterator> MoveRange;
  
  Control();
  
  void init(const class Portal &interfaces);
  void update();
  void onReceive(NetPacketType type, const class Packet &packet);
  const Tank::Input *lastInput(ActorId actor) const;
  MoveRange history(float time);
  void removeHistory(const MoveBuffer::iterator &first);
  
private:  
  void onTick(class Client *client);
  Tank::Input currentState() const;
  
  class WindowManager *wm;
  double lastTick;
  int keyUp, keyDown, keyLeft, keyRight, keyShoot;
  class Input *input;
  class Config *config;
  MoveBuffer moves;
  std::vector<Tank::Input> toSend;
  std::map<ActorId, Tank::Input> states;
  
  
  // data that will be replicated
  double inputBegan;
  Tank::Input state;
};

#endif // !GAME_CONTROL_H
