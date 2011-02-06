#include <game/client/gameclient.h>
#include <game/common/net_protocol.h>
#include <engine/portal.h>
#include <engine/network.h>
#include <iostream>

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
  msg.client_version = 100;
  msg.net_version = netVersion;

  _client->send(&msg, sizeof(msg), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}

void GameClient::onDisconnect() {
  std::cout << "disconnected" << std::endl;
}

void GameClient::onReceive(Packet *packet) {
  std::cout << "receive" << std::endl;
}
