#ifndef GAME_GAMESERVER_H
#define GAME_GAMESERVER_H

#include <game/common/system.h>
#include <game/common/net_protocol.h>
#include <vector>
#include <map>


class GameServer : public System {
public:
  GameServer();
  ~GameServer();
  
  void init(const class Portal &interfaces);
  void start();
  
  void update();
  void registerSystem(class ReplicatedSystem *system);
  void tick(double dt);
  class ClientSession *session(class Client *client) const;
  
private:
  typedef std::map<class Client *, class ClientSession *> SessionMap;
  
  void onConnect(class Client *client);
  void onDisconnect(class Client *client);
  void onReceive(class Packet *packet);
  
  // net protocol
  void onIdent(const struct NetIdentifyMsg *ident, class Packet *packet);
  
  std::vector<class ReplicatedSystem *> _systems;
  SessionMap _sessions;
  class Host *_host;
  class Logging *_log;
  class Network *_net;
};


#endif // !GAME_GAMESERVER_H
