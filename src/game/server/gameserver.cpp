#include <game/server/gameserver.h>
#include <game/common/net_protocol.h>
#include <game/common/replicated_system.h>
#include <game/common/snails.h>

#include <engine/packet.h>
#include <engine/network.h>
#include <engine/portal.h>

#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>

namespace {
void SendError(Client *client, uint8_t code, const std::string &desc) {
  NetErrorMsg msg;
  msg.type = NET_ERROR;
  msg.error_code = code;
  strncpy(msg.desc, desc.c_str(), MAX_ERRDESC);
  msg.desc[MAX_ERRDESC-1] = '\0';
  client->send(&msg, sizeof(NetErrorMsg),
               Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}
}

GameServer::GameServer() {
  std::fill(_systems, _systems + NET_SYSTEM_MAX,
            static_cast<ReplicatedSystem *>(0));
}

GameServer::~GameServer() {
  delete _host;
}

void GameServer::init(const class Portal &interfaces) {
  Network *net = interfaces.requestInterface<Network>();

  _host = net->startHost("0.0.0.0:12345", 32, 2);
  _systems[NET_SYSTEM_SNAILS] = context->snails();
  
}

void GameServer::update() {
  _host->update(); // Fixme: timeout should be the time until next update
  
  // Fixme: connectingClient -> clientConnecting?
  
  while (Client *client = _host->connectingClient()) {
    onConnect(client);
  }
    
  while (Client *client = _host->disconnectingClient()) {
    onDisconnect(client);
    delete client;
  }
  
  while (Packet *packet = _host->pendingPacket()) {
    onReceive(packet);
    delete packet;
  }
  
  // Packet: data() size() resize()
  // PacketBuilder(packet).write(123)
  // PacketReader
  // PacketWriter, PacketReader
  
  // Is it OK to have a bitfield in the beginning of each packet,
  // saying which subsystems have added stuff in it?
  // The packet is the transport of data between instances of the same
  // subsystem.
  
}

void GameServer::onConnect(Client *client) {
  std::cout << "new client: " << client->address() << "! :DD" << std::endl;
}

void GameServer::onDisconnect(Client *client) {
  std::cout << "client disconnected :(" << std::endl;
}

void GameServer::onReceive(Packet *packet) {
  size_t size = packet->size();
  const void *data = packet->data();
  assert(size >= sizeof(NetPacketType) && "received a too small packet");

  const NetPacketType *type = static_cast<const NetPacketType *>(data);
  switch (*type) {
  case NET_IDENTIFY:
    assert(size >= sizeof(NetIdentifyMsg) && "packet too small for ident");
    onIdent(static_cast<const NetIdentifyMsg *>(data), packet);
    break;

  }
  
}

void GameServer::onIdent(const NetIdentifyMsg *data, Packet *packet) {
  NetIdentifyMsg ident;
  ident.type = data->type;
  ident.client_version = ntohs(data->client_version);
  ident.net_version = ntohs(data->net_version);
  
  std::cout << "net: received ident for net " << ident.net_version << std::endl;

  if (ident.net_version != NET_VERSION) {
    SendError(packet->sender(), NET_IDENT_WRONG_VERSION,
              "wrong network version");
    return;
  }

  PacketData fullState;
  fullState.resize(sizeof(NetSystemMsg));
  
  PacketWriter writer(fullState);
  uint32_t systems = 0;
  
  for (size_t i = 0; i < NET_SYSTEM_MAX && _systems[i]; ++i) {
    size_t sizeBefore = fullState.size();
    _systems[i]->writeFull(writer);
    
    if (fullState.size() != sizeBefore) {
      // data has been written to the packet, mark the system as updated
      systems |= (1 << i);
    }
  }

  NetSystemMsg *header = reinterpret_cast<NetSystemMsg *>(&fullState[0]);
  header->type = NET_SYSTEM;
  header->systems = systems;
  
  std::cout << "full state size: " << fullState.size()
            << " systems: " << header->systems << std::endl;

  packet->sender()->send(&fullState[0], fullState.size(),
                         Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}
