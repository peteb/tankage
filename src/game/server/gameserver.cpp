#include <game/server/gameserver.h>
#include <game/common/net_protocol.h>

#include <engine/packet.h>
#include <engine/network.h>
#include <engine/portal.h>

#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <cstring>

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

GameServer::~GameServer() {
  delete _host;
}

void GameServer::init(const class Portal &interfaces) {
  Network *net = interfaces.requestInterface<Network>();

  _host = net->startHost("0.0.0.0:12345", 32, 2);
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

  PacketData systemMapping;
  systemMapping.reserve(sizeof(NetPacketType));
  systemMapping.resize(sizeof(NetPacketType));

  NetPacketType *type =
    static_cast<NetPacketType *>(&systemMapping[0]);
  *type = NET_SYSTEM_MAP;

  // send the system map
  Client *client = packet->sender();
  assert(client && "no sender");
  
  PacketWriter writer(systemMapping);
  writer.writeU8(123);

  std::cout << "sending " << systemMapping.size() << std::endl;
  
  client->send(&systemMapping[0], systemMapping.size(),
               Client::PACKET_RELIABLE, NET_CHANNEL_STATE);

}
