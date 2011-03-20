#include <game/client/gameclient.h>
#include <game/common/net_protocol.h>
#include <game/common/replicated_system.h>
#include <game/common/actors.h>
#include <game/common/config.h>

#include <engine/portal.h>
#include <engine/network.h>
#include <engine/packet.h>
#include <engine/logging.h>

#include <iostream>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <arpa/inet.h>

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
  _log = interfaces.requestInterface<Logging>();

  Config *config = context->system<Config>(SystemContext::SYSTEM_CONFIG);
  config->registerVariable("client", "host", &client_host);
  config->registerVariable("client", "predict", &client_predict);
}

void GameClient::start() {
  _state = GameClient::STATE_DISCONNECTED;
  _log->write(Logging::DEBUG, "Connecting to host: %s", client_host->c_str());
  _client = _net->connect(*client_host, 2);  
}

void GameClient::update() {
  if (!_client) return;

  _client->update();

  const bool connectedNow = _client->isConnected();
  if (_state == STATE_DISCONNECTED && connectedNow) {
    _state = STATE_CONNECTED;
    onConnect();
  }
  else if (_state >= STATE_CONNECTED && !connectedNow) {
    _state = STATE_DISCONNECTED;
    onDisconnect();
  }

  if (Packet *packet = _client->pendingPacket()) {
    onReceive(packet);
    delete packet;
  }

}

void GameClient::tick(double dt) {
  
  for (size_t i = 0; i < _systems.size(); ++i) {
    if (_systems[i]->flags & ReplicatedSystem::CLIENT_TICK) {
      _systems[i]->onTick(_client);
    }
  }

  if (_state == STATE_CONNECTED) {
    _time += dt;
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
  _time = 0.0f;
  _log->write(Logging::DEBUG, "*** connected!");

  // send the identification packet
  NetIdentifyMsg msg;
  msg.type = NET_IDENTIFY;
  msg.client_version = htons(100);
  msg.net_version = htons(NET_VERSION);
  _client->send(&msg, sizeof(msg), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}

void GameClient::onDisconnect() {
  _log->write(Logging::DEBUG, "disconnected");
}

void GameClient::onReceive(Packet *packet) {
  // Fixme: this code looks suspiciously similar to gameserver::onReceive..
  static int packetCount = 0;
  if (packetCount++ > 10) {
    packetCount = 0;
    _log->write(Logging::DEBUG, "rtt: %u", 
      packet->sender()->stats(Client::STAT_RTT));
  }
  
  size_t size = packet->size();
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
  }
}

void GameClient::onError(const NetErrorMsg *error, Packet *packet) {
  _log->write(Logging::DEBUG, "Received error from server: %s, code: %u", 
    error->desc, error->error_code);
  disconnectGently();
}

