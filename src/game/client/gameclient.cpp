#include <game/client/gameclient.h>
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

#include <game/client/snapshot.h>
#include <game/server/tank.h>

Variable<std::string> client_host("iostream.cc:12345");
Variable<bool> client_predict(true);

void client_RegisterVariables(Config &config) {
  config.registerVariable("client", "host", &client_host);
  config.registerVariable("client", "predict", &client_predict);
}

GameClient::GameClient(class Portal &services) 
  : _tankrenderer(this, services)
  , _control(services)
{
  _net = services.requestInterface<Network>();
  _gfx = services.requestInterface<Graphics>();
  _wm = services.requestInterface<WindowManager>();
  
  _last_update = _wm->timeSeconds();
  _input_time = 0.0;
  _since_snap = 0.0;
  
  _state = GameClient::STATE_DISCONNECTED;
  Log(INFO) << "connecting to host " << *client_host << "...";
  _client = _net->connect(*client_host, 2);  
  
  /*
   test code...
   
   char buffer[1024];
  Packer packer(buffer, buffer + 1024);
  
  Tank::State state;
  state.id = 1337;
  state.pos = vec2(12.0f, 33.0f);
  state.base_dir = 90.0f;
  state.write(packer);
  
  Tank::State new_state;
  Unpacker msg(buffer, buffer + 1024);
  new_state.read(msg);
  std::cout << "id: " << new_state.id << " pos: " << std::string(new_state.pos) << " dir: " << state.base_dir << std::endl;
  
  Snapshot<Tank::State> snapshot(100);
  Unpacker msg2(buffer, buffer + 1024);
  snapshot.push_back(msg2);
  
  Snapshot<Tank::State>::const_iterator it = snapshot.find(137);
  if (it != snapshot.end()) {
    std::cout << "Found! " << std::string(it->pos) << std::endl;
  }*/
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
  _input_time -= dt;
  
  _since_snap += dt;
  
  if (_input_time <= 0.0) {
    sendInput();
    _input_time = 1.0/10.0;
  }
  
  updateNet();
  
  const color4 desertColor(0.957f, 0.917f, 0.682f, 1.0f);
  _gfx->clear(desertColor);
  _tankrenderer.render();
}

void GameClient::updateNet() {
  _client->update();
  
  const bool connected = _client->isConnected();
  if (_state == STATE_DISCONNECTED && connected) {
    _state = STATE_CONNECTED;
    onConnect();
  }
  else if (_state >= STATE_CONNECTED && !connected) {
    _state = STATE_DISCONNECTED;
    onDisconnect();
  }
  
  if (Packet *packet = _client->pendingPacket()) {
    onReceive(packet);
    delete packet;
  }
  
}

void GameClient::sendInput() {
  const size_t BUFSZ = 256;
  char buffer[BUFSZ];
  
  Packer msg(buffer, buffer + BUFSZ);
  msg.writeShort(NET_PLAYER_INPUT);
  _control.currentInput().write(msg);
  _client->send(buffer, msg.size(), 0, NET_CHANNEL_STATE);
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
}

void GameClient::onDisconnect() {
  Log(INFO) << "disconnected";
}

double GameClient::deltaTime() const {
  return _since_snap / (1.0 / 20.0);
}

void GameClient::onReceive(Packet *packet) {
  // Fixme: this code looks suspiciously similar to gameserver::onReceive..
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
    } while (snaptype != 0);
    
    _tankrenderer.addSnapshot(tanks_snapshot);
    _since_snap = 0.0;
  }
  
}

void GameClient::onError(const NetErrorMsg *error, Packet *packet) {
  Log(DEBUG) << "received error from server: " 
             << error->desc << ", code: " 
             << error->error_code;
  
  disconnectGently();
}

