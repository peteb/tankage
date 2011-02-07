#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/common/system.h>

class GameClient : public System {
public:
  enum ClientState {
    STATE_DISCONNECTED = 0,
    STATE_CONNECTED,
    STATE_IDENTIFIED
  };
  
  ~GameClient();

  void init(const class Portal &interfaces);
  void update();
  void disconnectGently();
  
private:
  void onConnect();
  void onDisconnect();
  void onReceive(class Packet *packet);

  // net protocol
  void onError(const struct NetErrorMsg *error, class Packet *packet);
  void onSystemUpdate(const struct NetSystemMsg *msg, class Packet *packet);
  
  class Network *_net;
  class Client *_client;
  ClientState _state;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
