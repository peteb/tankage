#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/common/system.h>
#include <game/common/net_protocol.h>

#include <game/client/tank_renderer.h>

#include <vector>

class GameClient : public System {
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
  double localTime() const;

  bool predictLocal() const;
  
private:
  void onConnect();
  void onDisconnect();
  void onReceive(class Packet *packet);

  void updateNet();
  
  // net protocol
  void onError(const struct NetErrorMsg *error, class Packet *packet);
  void onSystemUpdate(const struct NetSystemMsg *msg, class Packet *packet);
  
  friend class TankRenderer;
  
  
  class Logging *_log;
  class Network *_net;
  class Client *_client;
  class Graphics *_gfx;
  
  ClientState _state;

  bool _predict_local;
  double _time;
  
  TankRenderer _tankrenderer;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
