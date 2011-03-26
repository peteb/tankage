#include <game/client/gameclient.h>
#include <game/client/tank_info.h>
#include <game/client/snapshot.h>
#include <game/server/tank.h> // needed for tank::state
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

void client_RegisterVariables(Config &config) {
  config.registerVariable("client", "host", &client_host);
  config.registerVariable("client", "name", &client_name);
  config.registerVariable("client", "predict", &client_predict);
  config.registerVariable("client", "lerp_remote", &client_lerpRemote);
}

GameClient::GameClient(class Portal &services) 
  : _texloader(services)
  , _textrenderer(services, _texloader)
  , _tankrenderer(this, services)
  , _control(services)
{
  _net = services.requestInterface<Network>();
  _gfx = services.requestInterface<Graphics>();
  _wm = services.requestInterface<WindowManager>();
  
  _last_update = _wm->timeSeconds();
  _input_time = 0.0;
  _since_snap = 0.0;
  _net_tickrate = 10.0; // default, but should be updated by server_info
  
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

  sendInput();
  updateNet();
  
  const color4 desertColor(0.957f, 0.917f, 0.682f, 1.0f);
  _gfx->clear(desertColor);
  _tankrenderer.render();
}

void GameClient::updateNet() {
  _client->update();
  
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
    new_input.write(msg);
    _client->send(buffer, msg.size(), 0, NET_CHANNEL_ABS);
    
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
    _client->update();
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
  return _since_snap / (1.0 / _net_tickrate);
}

bool GameClient::lerpRemote() const {
  return *client_lerpRemote;
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
    Snapshot<Tank::State> tanks_snapshot(snap_tick);
    
    unsigned short snaptype;
    
    do {
      snaptype = msg.readShort();
      
      if (snaptype == 1) {
        tanks_snapshot.push(msg);
      }
      else if (snaptype != 0) {
        onEvent(snaptype, msg);
      }
    } while (snaptype != 0);
    
    _tankrenderer.addSnapshot(tanks_snapshot);
    _since_snap = 0.0;
  }
  else {
    onEvent(msgtype, msg);
  }
  
}

void GameClient::onEvent(short event, class Unpacker &msg) {
  Log(INFO) << "received event " << event;

  if (event == NET_SERVER_INFO) {
    _net_tickrate = static_cast<double>(msg.readShort()) / 10.0;
    Log(INFO) << "server_info tickrate: " << _net_tickrate;
  }
  else if (event == NET_PLAYER_JOINED) {
    int tankid = msg.readInt();
    std::string name = msg.readString();
    Log(INFO) << "player '" << name << "' joined as " << tankid;
    
    TankInfo *info = tankInfo(tankid);
    if (!info) {
      info = new TankInfo;
      _tanks[tankid] = info;
    }
    
    info->name = name;
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
