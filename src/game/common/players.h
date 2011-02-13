#ifndef GAME_COMMON_PLAYERS_H
#define GAME_COMMON_PLAYERS_H

#include <game/common/replicated_system.h>

class Players : public ReplicatedSystem {
public:
  void init(const class Portal &interfaces);

  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  void onIdent(class Client *client);
  
private:
};

#endif // !GAME_COMMON_PLAYERS_H
