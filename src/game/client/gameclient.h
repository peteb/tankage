#ifndef GAME_CLIENT_GAMECLIENT_H
#define GAME_CLIENT_GAMECLIENT_H

#include <game/client/tank_renderer.h>
#include <game/client/text_renderer.h>
#include <game/client/bullet_renderer.h>
#include <game/client/resource_renderer.h>
#include <game/client/map.h>
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
  double sinceSnap() const;
  double tickDuration() const;
  bool lerpRemote() const;
  int localPlayer() const;
  void setFocus(const vec2 &pos);
  
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
  double _last_ping;
  vec2 _view;
  int _local_player;
  
  Control::Input _sent_input;
  TextureLoader _texloader;
  TextRenderer _textrenderer;
  BulletRenderer _bulletrenderer;
  TankRenderer _tankrenderer;
  ResourceRenderer _resrenderer;
  Control _control;
  ClientMap _map;
  
  std::string _error;
  
  std::map<int, class TankInfo *> _tanks;
};


#endif // !GAME_CLIENT_GAMECLIENT_H
