#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/client/tank_renderer.h>
#include <game/client/text_renderer.h>
#include <game/client/bullet_renderer.h>
#include <game/common/net_protocol.h>
#include <game/common/control.h>
#include <game/common/texture_loader.h>
#include <vector>
#include <map>

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
  double deltaTime() const;
  bool lerpRemote() const;
  
  class TankInfo *tankInfo(int eid);
  TextureLoader &textureLoader();
  TextRenderer &textRenderer();
  
private:
  void sendInput();
  
  void onConnect();
  void onDisconnect();
  void onReceive(class Packet *packet);
  void onEvent(short event, class Unpacker &msg);
  
  void updateNet();
  
  class Network *_net;
  class Client *_client;
  class Graphics *_gfx;
  class WindowManager *_wm;
  
  ClientState _state;

  double _last_update;
  double _input_time;
  double _since_snap;
  double _net_tickrate;
  
  Control::Input _sent_input;
  TextureLoader _texloader;
  TextRenderer _textrenderer;
  BulletRenderer _bulletrenderer;
  TankRenderer _tankrenderer;
  Control _control;
  
  std::map<int, class TankInfo *> _tanks;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
