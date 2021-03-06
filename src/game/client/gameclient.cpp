#include <game/client/gameclient.h>
#include <game/client/tank_info.h>
#include <game/client/snapshot.h>
#include <game/server/tank.h> // needed for tank::state
#include <game/server/bullet.h>
#include <game/server/resource.h>
#include <game/common/net_protocol.h>
#include <game/common/config.h>

#include <platform/portal.h>
#include <platform/network.h>
#include <platform/packet.h>
#include <platform/graphics.h>
#include <platform/window_manager.h>
#include <platform/self_updater.h>
#include <platform/config.h>

#include <utils/log.h>
#include <utils/packer.h>

#include <cassert>
#include <iostream>
#include <fstream>

namespace {
std::string Env(const std::string &name) {
  char *var = getenv(name.c_str());
  return (var ? var : "");
}
}


Variable<std::string> client_host("tankage.iostream.cc:12345");
Variable<std::string> client_name("Master");
Variable<bool> client_predict(true);
Variable<bool> client_lerpRemote(true);
Variable<bool> client_centerCam(true);

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
  , _resrenderer(this, services)
  , _control(services)
  , _map(services, this)
{
  _net = services.requestInterface<Network>();
  _gfx = services.requestInterface<Graphics>();
  _wm = services.requestInterface<WindowManager>();

  #if defined(REMOTE_BINARY) && !defined(DEV)
  if (Env("SKIP_UPDATE") != "true") {
    Log(INFO) << "checking for updates...";
    SelfUpdater *updater = services.requestInterface<SelfUpdater>();
    #ifdef _WIN32
    updater->requestUpdate("tankage.exe", 
                           "chikera-makera.id.lv/drop-box/tankage/tankage.exe");
    #else

    updater->requestUpdate("tankage", 
                           "http://iostream.cc/~peter/binaries/" REMOTE_BINARY);
    #endif
  }
  else {
    Log(INFO) << "skipping update";
    
    #ifdef _WIN32
    // FIXME: this should not be here. it should be in the SelfUpdate service.
    std::ifstream file("_tankage.exe", std::ifstream::in);
    if (file.is_open()) {
      file.close();
      Log(INFO) << "removing _tankage.exe";
      if(remove("_tankage.exe")) {
        Log(INFO) << "failed to remove _tankage.exe";
      }
    }
    #endif
  }
  #else
  Log(INFO) << "can't update; binary not built with a remote url";
  #endif
  
  _last_update = _wm->timeSeconds();
  _input_time = 0.0;
  _since_snap = 0.0;
  _net_tickrate = 10.0; // default, but should be updated by server_info
  _view = vec2(0.0f, 0.0f);
  _local_player = -1;
  _first_tick = -1;
  
  _state = GameClient::STATE_DISCONNECTED;
  Log(INFO) << "connecting to host " << *client_host << "...";
  _error = "Connecting...";
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

  // FIXME: get the local tank here, and set the transform based on that!
  //        also remove setFocus
  if (*client_centerCam)
    _gfx->setTransform(-_view);
  else
    _gfx->setTransform(vec2(0.0f, 0.0f));
  
  sendInput();
  updateNet();
  
  
  const color4 desertColor(0.957f, 0.917f, 0.682f, 1.0f);
  _gfx->clear(desertColor);
  _map.render();
  _resrenderer.render();
  _tankrenderer.render();
  _bulletrenderer.render();
  
  if (_state != GameClient::STATE_CONNECTED)
    _textrenderer.renderText(_error, vec2(0.0f, 0.0f), 2.0f);
  
}

void GameClient::updateNet() {
  _client->update(10); // FIXME: can we steal ms here just like this?
  
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
  static std::vector<unsigned char> buffer;
  buffer.clear();
  
  double now = _wm->timeSeconds();
  Control::Input new_input = _control.currentInput();
  
  if (new_input.buttons != _sent_input.buttons || now - _input_time >= 1.0/10.0) {
    Packer msg(buffer);
    msg.writeShort(NET_PLAYER_INPUT);
    new_input.aim_x += _view.x;
    new_input.aim_y += _view.y;
    new_input.write(msg);
    _client->send(&buffer[0], buffer.size(), 0, NET_CHANNEL_ABS);
    _client->flush();
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
  
  static std::vector<unsigned char> buffer;
  buffer.clear();

  Packer msg(buffer);
  msg.writeShort(NET_CLIENT_INFO);
  msg.writeString(*client_name);
  msg.writeInt(NET_VERSION);
  _client->send(&buffer[0], buffer.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}

void GameClient::onDisconnect() {
  Log(INFO) << "disconnected";
}

double GameClient::deltaTime() const {
  // 0.0 = at time of snapshot
  // 1.0 = when next snapshot should be here
  
  return _since_snap / (1.0 / _net_tickrate);
}

// 0.1.9

double GameClient::sinceSnap() const {
  return _since_snap;
}

bool GameClient::lerpRemote() const {
  return *client_lerpRemote;
}

double GameClient::tickDuration() const {
  return 1.0/_net_tickrate;
}

double GameClient::localTime() const {
  return (snapTick() - _first_tick) * tickDuration() + sinceSnap();
}

int GameClient::snapTick() const {
  return _snap_tick;
}

void GameClient::onReceive(Packet *packet) {
  static std::vector<unsigned char> buffer;  
  static int packetCount = 0;
  
  if (packetCount++ > 10) {
    packetCount = 0;
    Log(DEBUG) << "rtt: " << packet->sender()->stats(Client::STAT_RTT);
  }
  
  buffer.assign((const char *)packet->data(),
                (const char *)packet->data() + packet->size());
  Unpacker msg(buffer);
  short msgtype = msg.readShort();
  
  if (msgtype == NET_SNAPSHOT) {
    int snap_tick = msg.readInt();
    Log(DEBUG) << "rx snap " << snap_tick << ", since last: " << sinceSnap();
    
    if (_first_tick < 0)
      _first_tick = snap_tick;
    
    _snap_tick = snap_tick;
    
    Snapshot<Tank::State> tanks_snapshot(snap_tick);
    Snapshot<Bullet::State> bullets_snapshot(snap_tick);
    Snapshot<Resource::State> res_snapshot(snap_tick);
    
    while (short snaptype = msg.readShort()) {
      switch (snaptype) {
      case 1: // FIXME: remove magical numbers
        tanks_snapshot.push(msg);
        break;
          
      case 2:
        bullets_snapshot.push(msg);
        break;
        
      case 3:
        res_snapshot.push(msg);
        break;

      default:
        onEvent(snaptype, msg);
      }
    }
    
    _tankrenderer.addSnapshot(tanks_snapshot);
    _bulletrenderer.addSnapshot(bullets_snapshot);
    _resrenderer.addSnapshot(res_snapshot);
    _since_snap = 0.0;
  }
  else if (msgtype == NET_ERROR) {
    int code = msg.readInt();
    std::string str = msg.readString();
    Log(INFO) << "> " << str << " (" << code << ")";
    _error = str;
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
  else if (event == NET_ITEM_PICKUP) {
    int entity = msg.readInt();
    char item = msg.readByte();
    char amount = msg.readByte();
    Log(DEBUG) << "tank " << entity << " picked up " << 
                  int(item) << " amount: " << int(amount);
  }
  else if (event == NET_INVENTORY_UPDATE) {
    int entity = msg.readInt();
    char items = msg.readByte();
    
    // We still have to eat the data, even if we're not the target..
    if (entity == localPlayer()) {
      Log(INFO) << int(items) << "# of items in tank inventory:";
      Log(INFO) << "---------------------------------";
    }
    else {
      Log(INFO) << "not local player";
    }
      
    for (int i = 0; i < items; ++i) {
      char type = msg.readByte();
      char amount = msg.readByte();
        
      if (entity == localPlayer()) {
        Log(INFO) << "type: " << int(type) << " amount: " << int(amount);
      }
    }
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
