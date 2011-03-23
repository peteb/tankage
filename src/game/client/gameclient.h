#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/common/net_protocol.h>

#include <game/client/tank_renderer.h>

#include <vector>

void client_RegisterVariables(class Config &config);

class GameClient {
public:  
  enum ClientState {
    STATE_DISCONNECTED = 0,
    STATE_CONNECTED,
    STATE_IDENTIFIED
  };
  
  GameClient(class Portal &services);
  ~GameClient();

  void start();
  void update();
  void disconnectGently();
  
private:
  void onConnect();
  void onDisconnect();
  void onReceive(class Packet *packet);

  void updateNet();
  
  // net protocol
  void onError(const struct NetErrorMsg *error, class Packet *packet);
  void onSystemUpdate(const struct NetSystemMsg *msg, class Packet *packet);
  
  class Network *_net;
  class Client *_client;
  class Graphics *_gfx;
  
  ClientState _state;

  double _time;
  
  TankRenderer _tankrenderer;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
