#include <game/client/gameclient.h>
#include <game/common/net_protocol.h>
#include <game/common/replicated_system.h>
#include <game/common/actors.h>
#include <game/common/config.h>

#include <engine/portal.h>
#include <engine/network.h>
#include <engine/packet.h>

#include <utils/log.h>
#include <utils/packer.h>

#include <cassert>
#include <iostream>

Variable<std::string> client_host("iostream.cc:12345");
Variable<bool> client_predict(true);

GameClient::GameClient() : _client(0) {
  _time = 0.0f;
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
  std::cout << "DATA TYPE: " << msg.readShort() << std::endl;
  
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

