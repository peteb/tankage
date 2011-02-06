#include <game/server/gameserver.h>
#include <engine/network.h>
#include <engine/portal.h>
#include <iostream>

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
//   std::cout << "received a packet!" << std::endl;
//   std::cout << (const char *)packet->data() << std::endl;
//   std::cout << packet->sender()->stats(Client::STAT_RTT) << std::endl;
//   packet->sender()->send("HEJSAN!!", 9, Client::PACKET_RELIABLE, 0);
}

