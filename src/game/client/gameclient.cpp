#include <game/client/gameclient.h>
#include <game/client/tank_info.h>
#include <game/client/snapshot.h>
#include <game/server/tank.h> // needed for tank::state
#include <game/server/bullet.h>
#include <game/common/net_protocol.h>
#include <game/common/config.h>

#include <engine/portal.h>
#include <engine/network.h>
#include <engine/packet.h>
#include <engine/graphics.h>
#include <engine/window_manager.h>

#include <utils/log.h>
#include <utils/packer.h>

#include <cassert>
#include <iostream>


Variable<std::string> client_host("iostream.cc:12345");
Variable<std::string> client_name("Some guy");
Variable<bool> client_predict(true);
Variable<bool> client_lerpRemote(true);
Variable<bool> client_centerCam(false);

void client_RegisterVariables(Config &config) {
  config.registerVariable("client", "host", &client_host);
  config.registerVariable("client", "name", &client_name);
  config.registerVariable("client", "predict", &client_predict);
  config.registerVariable("client", "lerp_remote", &client_lerpRemote);
  config.registerVariable("client", "center_cam", &client_centerCam);
}

GameClient::GameClient(class Portal &services) 
  : _texloader(services)
  , _textrenderer(services, _texloader) // NOTE: can probably be done similar to below, ->textureLoader()..
  , _bulletrenderer(this, services)
  , _tankrenderer(this, services)
  , _control(services)
  , _map(services)
{
  _net = services.requestInterface<Network>();
  _gfx = services.requestInterface<Graphics>();
  _wm = services.requestInterface<WindowManager>();
  
  _last_update = _wm->timeSeconds();
  _input_time = 0.0;
  _since_snap = 0.0;
  _net_tickrate = 10.0; // default, but should be updated by server_info
  _view = vec2(0.0f, 0.0f);
  _local_player = -1;
  
  _state = GameClient::STATE_DISCONNECTED;
  Log(INFO) << "connecting to host " << *client_host << "...";
  _client = _net->connect(*client_host, 2);  
}

GameClient::~GameClient() {
  if (_client) {
    _client->disconnect();
    delete _client;
  }
  
}

void GameClient::update() {
  if (!_client) 
    return;

  double now = _wm->timeSeconds();
  double dt = now - _last_update;
  _last_update = now;  
  _since_snap += dt;

  if (*client_centerCam)
    _gfx->setTransform(-_view);
  else
    _gfx->setTransform(vec2(0.0f, 0.0f));
  
  sendInput();
  updateNet();
  
  
  const color4 desertColor(0.957f, 0.917f, 0.682f, 1.0f);
  _gfx->clear(desertColor);
  _map.render();
  _tankrenderer.render();
  _bulletrenderer.render();
}

void GameClient::updateNet() {
  _client->update(10);
  
  const bool connected = _client->isConnected();

  if (Packet *packet = _client->pendingPacket()) {
    onReceive(packet);
    delete packet;
  }
  
  if (_state == STATE_DISCONNECTED && connected) {
    _state = STATE_CONNECTED;
    onConnect();
  }
  else if (_state >= STATE_CONNECTED && !connected) {
    _state = STATE_DISCONNECTED;
    onDisconnect();
  }
}

void GameClient::sendInput() {
  const size_t BUFSZ = 256;
  char buffer[BUFSZ];

  double now = _wm->timeSeconds();
  Control::Input new_input = _control.currentInput();
  
  if (new_input.buttons != _sent_input.buttons || now - _input_time >= 1.0/10.0) {
    Packer msg(buffer, buffer + BUFSZ);
    msg.writeShort(NET_PLAYER_INPUT);
    new_input.aim_x += _view.x;
    new_input.aim_y += _view.y;
    new_input.write(msg);
    _client->send(buffer, msg.size(), Client::PACKET_UNSEQUENCED, NET_CHANNEL_ABS);
    
    _sent_input = new_input;
    _input_time = now;
  }
  
}

void GameClient::disconnectGently() {
  if (!_client)
    return;
  
  _state = STATE_DISCONNECTED;
  
  // let the client down gently
  _client->disconnect();
  while (_client->isConnected()) { // a bit dangerous...
    _client->update(5);
  }
  
  delete _client;
  _client = NULL;
}

void GameClient::onConnect() {
  Log(INFO) << "connected!";
  
  char buffer[1024];
  Packer msg(buffer, buffer + 1024);
  msg.writeShort(NET_CLIENT_INFO);
  msg.writeString(*client_name);
  _client->send(buffer, msg.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}

void GameClient::onDisconnect() {
  Log(INFO) << "disconnected";
}

double GameClient::deltaTime() const {
  // 0.0 = at time of snapshot
  // 1.0 = when next snapshot should be here
  
  return _since_snap / (1.0 / _net_tickrate);
}

double GameClient::sinceSnap() const {
  return _since_snap;
}

bool GameClient::lerpRemote() const {
  return *client_lerpRemote;
}

double GameClient::tickDuration() const {
  return 1.0/_net_tickrate;
}

void GameClient::onReceive(Packet *packet) {
  static int packetCount = 0;
  if (packetCount++ > 10) {
    packetCount = 0;
    Log(DEBUG) << "rtt: " << packet->sender()->stats(Client::STAT_RTT);
  }
  
  Unpacker msg(packet->data(), (const char *)packet->data() + packet->size());
  short msgtype = msg.readShort();
  
  if (msgtype == NET_SNAPSHOT) {
    int snap_tick = msg.readInt();
    Log(DEBUG) << "rx snap " << snap_tick << ", since last: " << sinceSnap();
    
    Snapshot<Tank::State> tanks_snapshot(snap_tick);
    Snapshot<Bullet::State> bullets_snapshot(snap_tick);
    
    while (short snaptype = msg.readShort()) {
      switch (snaptype) {
      case 1: // FIXME: remove magical numbers
        tanks_snapshot.push(msg);
        break;
          
      case 2:
        bullets_snapshot.push(msg);
        break;
        
      default:
        onEvent(snaptype, msg);
      }
    }
    
    _tankrenderer.addSnapshot(tanks_snapshot);
    _bulletrenderer.addSnapshot(bullets_snapshot);
    _since_snap = 0.0;
  }
  else if (msgtype == NET_MAPCHUNK) {
    _map.addChunk(msg);
  }
  else {
    onEvent(msgtype, msg);
  }
  
}

void GameClient::onEvent(short event, class Unpacker &msg) {
  Log(INFO) << "received event " << event;

  if (event == NET_SERVER_INFO) {
    _net_tickrate = static_cast<double>(msg.readShort()) / 10.0;
    _local_player = msg.readInt();
    Log(INFO) << "server_info tickrate: " << _net_tickrate;
    Log(INFO) << "server_info player: " << _local_player;
    
  }
  else if (event == NET_PLAYER_INFO) {
    int tankid = msg.readInt();
    std::string name = msg.readString();
    Log(INFO) << "entity " << tankid << " is playing as '" << name << "'";
    
    TankInfo *info = tankInfo(tankid);
    if (!info) {
      info = new TankInfo;
      _tanks[tankid] = info;
    }
    
    info->name = name;
  }
  else if (event == NET_TANK_HIT) {
    vec2 pos;
    pos.x = msg.readShort();
    pos.y = msg.readShort();
    int tankid = msg.readInt();
    int shooter = msg.readInt();
    
    Log(DEBUG) << "tank " << tankid << " hit at " << std::string(pos) << " by " << shooter;

//    if (shooter == localPlayer())
//      system("afplay ../data/hit.wav &");
  }
  else if (event == NET_TILE_UPDATE) {
    int x = msg.readShort();
    int y = msg.readShort();
    char state = msg.readByte();
    _map.setTile(std::make_pair(x, y), state);
  }
}

TextureLoader &GameClient::textureLoader() {
  return _texloader;
}

TextRenderer &GameClient::textRenderer() {
  return _textrenderer;
}

TankInfo *GameClient::tankInfo(int eid) {
  std::map<int, TankInfo *>::iterator it = _tanks.find(eid);
  if (it == _tanks.end())
    return NULL;

  return it->second;
}

int GameClient::localPlayer() const {
  return _local_player;
}

void GameClient::setFocus(const vec2 &pos) {
  // FIXME: pull this out into Camera class when more functionality is added
  if (*client_centerCam)
    _view = pos;
}