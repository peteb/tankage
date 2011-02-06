#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/common/system.h>

class GameClient : public System {
public:
  ~GameClient();

  void init(const class Portal &interfaces);
  void update();
  void disconnectGently();
  
private:
  void onConnect();
  void onDisconnect();
  void onReceive(class Packet *packet);
  
  class Network *_net;
  class Client *_client;
  bool _connected;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
