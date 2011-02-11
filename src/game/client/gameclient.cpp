#include <game/client/gameclient.h>
#include <game/common/net_protocol.h>
#include <game/common/replicated_system.h>
#include <game/common/snails.h>

#include <engine/portal.h>
#include <engine/network.h>
#include <engine/packet.h>

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
GameClient::GameClient() {
  std::fill(_systems, _systems + NET_SYSTEM_MAX,
            static_cast<ReplicatedSystem *>(0));
}

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
  _systems[NET_SYSTEM_SNAILS] = context->snails();
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

  case NET_SYSTEM:
    assert(size >= sizeof(NetSystemMsg) && "packet too small for error");
    onSystemUpdate(static_cast<const NetSystemMsg *>(data), packet);
    break;
  }
}

void GameClient::onError(const NetErrorMsg *error, Packet *packet) {
  std::cout << "Received error from server: " << error->desc << std::endl;
  disconnectGently();
}

void GameClient::onSystemUpdate(const NetSystemMsg *msg, Packet *packet) {
  std::cout << "received system update " << msg->systems << std::endl;
  if (msg->systems != 0) {
    PacketReader reader((const char *)packet->data() + sizeof(NetSystemMsg),
                        packet->size() - sizeof(NetSystemMsg));
    
    for (size_t i = 0; i < NET_SYSTEM_MAX; ++i) {
      if (msg->systems & (1 << i) && _systems[i]) {
        std::cout << "System updated" << std::endl;
        _systems[i]->readFull(reader);
      }
    }
  }
}
