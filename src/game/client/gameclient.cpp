#include <game/client/gameclient.h>
#include <game/common/net_protocol.h>
#include <game/common/replicated_system.h>
#include <game/common/actors.h>

#include <engine/portal.h>
#include <engine/network.h>
#include <engine/packet.h>
#include <engine/logging.h>
#include <engine/cfg.h>

#include <iostream>
#include <cassert>
#include <algorithm>
#include <arpa/inet.h> // Fixme: this might not be possible on windows. utils
                       // for endian conversion?

/*
 * Messages are sent to the client and server, invoking onReceive on all
 * registered ReplicatedSystems. The systems will check the type of the packet,
 * then act accordingly (either handle the message or ignore it).
 *
 * Snapshots will be sent to the server (input), but mostly to the client (item
 * position, speed, etc.)
 * Each subsystem implements onTick, which should create the snapshot packet.
 * onReceive will be called when snapshot data is received.
 * The onTick function will be invoked at a certain interval (25 tps).
 */
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
  _state = GameClient::STATE_DISCONNECTED;
 
  _config = interfaces.requestInterface<Config>(); 
  _net = interfaces.requestInterface<Network>();
  _log = interfaces.requestInterface<Logging>();

  _log->write(Logging::DEBUG, "Connecting to host: %s", 
    _config->property("client", "host", "iostream.cc:12345").c_str());
  _client = _net->connect(_config->property("client", "host", "iostream.cc:12345"), 2);
}

void GameClient::update() {
  if (!_client) {
    return;
  }
  
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
  if (_state == STATE_CONNECTED) {
    _time += dt;
  }
  
  for (size_t i = 0; i < _systems.size(); ++i) {
    if (_systems[i]->flags & ReplicatedSystem::CLIENT_TICK) {
      _systems[i]->onTick(_client);
    }
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

void GameClient::registerSystem(class ReplicatedSystem *system) {
  _systems.push_back(system);
}

float GameClient::localTime() const {
  return _time;
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

