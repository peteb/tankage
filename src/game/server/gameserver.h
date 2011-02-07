#ifndef GAME_GAMESERVER_H
#define GAME_GAMESERVER_H

#include <game/common/system.h>
#include <game/common/net_protocol.h>

class GameServer : public System {
public:
  GameServer();
  ~GameServer();
  
  void init(const class Portal &interfaces);
  void update();
  
private:
  void onConnect(class Client *client);
  void onDisconnect(class Client *client);
  void onReceive(class Packet *packet);

  // net protocol
  void onIdent(const struct NetIdentifyMsg *ident, class Packet *packet);
  
  class Host *_host;
  class ReplicatedSystem *_systems[NET_SYSTEM_MAX];
};


#endif // !GAME_GAMESERVER_H
