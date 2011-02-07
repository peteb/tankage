#include <game/client/gameclient.h>
#include <game/common/net_protocol.h>
#include <engine/portal.h>
#include <engine/network.h>
#include <iostream>
#include <cassert>
#include <arpa/inet.h> // Fixme: this might not be possible on windows. utils
                       // for endian conversion?

GameClient::~GameClient() {
  if (_client) {
    _client->disconnect();
    delete _client;
  }
  
}

void GameClient::init(const Portal &interfaces) {
  _state = GameClient::STATE_DISCONNECTED;
  
  _net = interfaces.requestInterface<Network>();
  _client = _net->connect("127.0.0.1:12345", 2);
  
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

void GameClient::onConnect() {
  std::cout << "connected" << std::endl;

  // send the identification packet
  NetIdentifyMsg msg;
  msg.type = NET_IDENTIFY;
  msg.client_version = htons(100);
  msg.net_version = htons(NET_VERSION);

  _client->send(&msg, sizeof(msg), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}

void GameClient::onDisconnect() {
  std::cout << "disconnected" << std::endl;
}

void GameClient::onReceive(Packet *packet) {
  std::cout << "receive" << std::endl;

  // Fixme: this code looks suspiciously similar to gameserver::onReceive..
  
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
}

void GameClient::onError(const NetErrorMsg *error, Packet *packet) {
  std::cout << "Received error from server: " << error->desc << std::endl;
  disconnectGently();
}
