#ifndef GAME_COMMON_PLAYERS_H
#define GAME_COMMON_PLAYERS_H

#include <game/common/system.h>
#include <game/common/replicated_system.h>

class Players : public System {
public:
  void init(const class Portal &interfaces);

  void onTick(class Client *client);
  void onReceive(NetPacketType type, const void *data, size_t size);
  
private:
};

#endif // !GAME_COMMON_PLAYERS_H
