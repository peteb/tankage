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
    PlayerInput delta;
    TankState absolute;
  };

  typedef ring_buffer<Move> MoveBuffer;
  typedef std::pair<MoveBuffer::iterator, MoveBuffer::iterator> MoveRange;
  
  Control();
  
  void init(const class Portal &interfaces);
  void start();
  
  void update();
  void onReceive(NetPacketType type, const class Packet &packet);
  const PlayerInput *lastInput(ActorId actor) const;
  MoveRange history(float time);
  void removeHistory(const MoveBuffer::iterator &first);
  
private:  
  void reloadKeycodes();
  
  void onTick(class Client *client);
  PlayerInput currentState() const;
  
  class WindowManager *wm;
  double lastTick;
  int keyUp, keyDown, keyLeft, keyRight, keyShoot;
  class Input *input;
  class Config *config;
  MoveBuffer moves;
  std::vector<PlayerInput> toSend;
  std::map<ActorId, PlayerInput> states;
  PlayerInput state; // FIXME: prevstate
  
};

#endif // !GAME_CONTROL_H
