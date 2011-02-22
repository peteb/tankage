#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/common/system.h>
#include <game/common/net_protocol.h>
#include <vector>

class GameClient : public System {
public:
  enum ClientState {
    STATE_DISCONNECTED = 0,
    STATE_CONNECTED,
    STATE_IDENTIFIED
  };
  
  GameClient();
  ~GameClient();

  void init(const class Portal &interfaces);
  void update();
  void disconnectGently();
  void registerSystem(class ReplicatedSystem *system);
  void tick(double dt);
  
private:
  void onConnect();
  void onDisconnect();
  void onReceive(class Packet *packet);

  // net protocol
  void onError(const struct NetErrorMsg *error, class Packet *packet);
  void onSystemUpdate(const struct NetSystemMsg *msg, class Packet *packet);
  
  std::vector<class ReplicatedSystem *> _systems;
  class Config *_config;
  class Logging *_log;
  class Network *_net;
  class Client *_client;
  ClientState _state;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
