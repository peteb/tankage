#ifndef GAME_GAMESERVER_H
#define GAME_GAMESERVER_H

#include <game/common/system.h>
#include <game/common/net_protocol.h>
#include <game/server/entity.h>

#include <vector>
#include <map>


class GameServer : public System {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_GAMESERVER;
  }
  
  GameServer();
  ~GameServer();
  
  void run();
  void init(const class Portal &interfaces);
  void start();
  
  class ClientSession *session(class Client *client) const;
  unsigned int gameTick() const;
  double tickDuration() const;
  
private:
  typedef std::map<class Client *, class ClientSession *> SessionMap;
  
  void onConnect(class Client *client);
  void onDisconnect(class Client *client);
  void onReceive(class Packet *packet);
  void updateNet(int timeout = 0);
  void onTick();
  class Tank *spawnTank();
  
  // net protocol
  void onIdent(const struct NetIdentifyMsg *ident, class Packet *packet);
  
  std::vector<Entity *> _entities;
  SessionMap _sessions;
  class Host *_host;
  class Logging *_log;
  class Network *_net;
  class WindowManager *_wm;
  unsigned int _tick;
};


#endif // !GAME_GAMESERVER_H
