#include <game/client/gameclient.h>
#include <game/common/net_protocol.h>
#include <game/common/replicated_system.h>
#include <game/common/config.h>

#include <engine/portal.h>
#include <engine/network.h>
#include <engine/packet.h>

#include <utils/log.h>
#include <utils/packer.h>

#include <cassert>
#include <iostream>

#include <game/client/snapshot.h>
#include <game/server/tank.h>

Variable<std::string> client_host("iostream.cc:12345");
Variable<bool> client_predict(true);

GameClient::GameClient() : _client(0), _tankrenderer(this) {
  _time = 0.0f;
  
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
  }
}

GameClient::~GameClient() {
  if (_client) {
    _client->disconnect();
    delete _client;
  }
  
}

void GameClient::init(const Portal &interfaces) {
  _net = interfaces.requestInterface<Network>();

  Config *config = context->system<Config>();
  config->registerVariable("client", "host", &client_host);
  config->registerVariable("client", "predict", &client_predict);
  
  _tankrenderer.init(interfaces, context);
}

void GameClient::start() {
  _state = GameClient::STATE_DISCONNECTED;
  Log(INFO) << "connecting to host " << *client_host << "...";
  _client = _net->connect(*client_host, 2);  
}

void GameClient::update() {
  if (!_client) 
    return;

  updateNet();
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

void GameClient::disconnectGently() {
  if (!_client) {
    return;
  }

  _state = STATE_DISCONNECTED;
  
  // let the client down gently
  _client->disconnect();
  while (_client->isConnected()) {
    _client->update();
  }
  
  delete _client;
  _client = NULL;
}

double GameClient::localTime() const {
  return _time;
}

bool GameClient::predictLocal() const {
  return *client_predict;
}

void GameClient::onConnect() {
  Log(INFO) << "connected!"; 

  _time = 0.0f;

  // send the identification packet
  /*NetIdentifyMsg msg;
  msg.type = NET_IDENTIFY;
  msg.client_version = htons(100);
  msg.net_version = htons(NET_VERSION);
  _client->send(&msg, sizeof(msg), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);*/
}

void GameClient::onDisconnect() {
  Log(INFO) << "disconnected";
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
   // _prevtick = _lasttick;
   // _prevsnap = _lastsnap;
    
    //_lasttick = msg.readInt();
    // _lastsnap.clear();
    unsigned short snaptype;
    
    // FIXME: client, update as fast as possible, output the ticks etc
    //        then render!
    
    do {
      snaptype = msg.readShort();
      
     /* if (snaptype == 1) {
        std::cout << "got snaptype tank" << std::endl;
        TankState state;
        state.actor = msg.readInt();
        state.pos.x = static_cast<float>(msg.readShort()) / 10.0f;
        state.pos.y = static_cast<float>(msg.readShort()) / 10.0f;
        state.base_dir = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;
        state.turret_dir = static_cast<float>(msg.readShort()) / 65536.0f * 360.0f;
        state.shooting = msg.readShort();
        _lastsnap.push_back(state);
      }*/
    } while (snaptype != 0);
  }
  
  
  /*size_t size = packet->size();
  const void *data = packet->data();
  assert(size >= sizeof(NetPacketType) && "received a too small packet");

  const NetPacketType *type = static_cast<const NetPacketType *>(data);
  switch (*type) {
  case NET_ERROR:
    assert(size >= sizeof(NetErrorMsg) && "packet too small for error");
    onError(static_cast<const NetErrorMsg *>(data), packet);
    break;
  }

  for (size_t i = 0; i < _systems.size(); ++i) {
    if (_systems[i]->flags & ReplicatedSystem::CLIENT_RECEIVE) {
      _systems[i]->onReceive(*type, *packet);
    }
  }*/
}

void GameClient::onError(const NetErrorMsg *error, Packet *packet) {
  Log(DEBUG) << "received error from server: " 
             << error->desc << ", code: " 
             << error->error_code;
  
  disconnectGently();
}

