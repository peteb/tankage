#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/common/system.h>
#include <game/common/net_protocol.h>
#include <game/common/peer.h>
#include <vector>

class GameClient : public Peer {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_GAMECLIENT;
  }
  
  enum ClientState {
    STATE_DISCONNECTED = 0,
    STATE_CONNECTED,
    STATE_IDENTIFIED
  };
  
  GameClient();
  ~GameClient();

  void init(const class Portal &interfaces);
  void start();
  void update();
  void disconnectGently();
  void tick(double dt);
  double localTime() const;

  bool predictLocal() const;
  
private:
  void onConnect();
  void onDisconnect();
  void onReceive(class Packet *packet);

  // net protocol
  void onError(const struct NetErrorMsg *error, class Packet *packet);
  void onSystemUpdate(const struct NetSystemMsg *msg, class Packet *packet);
  
  class Logging *_log;
  class Network *_net;
  class Client *_client;
  ClientState _state;

  bool _predict_local;
  double _time;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
