#include <game/server/gameserver.h>
#include <game/server/client_session.h>

#include <game/common/net_protocol.h>
#include <game/common/net_error.h>
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

GameServer::GameServer() {
  
}

GameServer::~GameServer() {
  delete _host;
}

void GameServer::init(const class Portal &interfaces) {
  Network *net = interfaces.requestInterface<Network>();

  _host = net->startHost("0.0.0.0:12345", 32, 2);
}

void GameServer::update() {
  _host->update(); // Fixme: timeout should maybe be the time until next update
  
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
}

void GameServer::tick(double dt) {
  std::cout << dt << std::endl;
}


void GameServer::registerSystem(class ReplicatedSystem *system) {
  _systems.push_back(system);
}

void GameServer::onConnect(Client *client) {
  std::cout << "new client: " << client->address() << "! :DD" << std::endl;
  if (_sessions.find(client) != _sessions.end()) {
    // The client already exists, weird..
    std::cout << "client already exists!" << std::endl;
    // Fixme: throw exception, disconnect the connection
    return;
  }
  
  ClientSession *session = new ClientSession(client);
  _sessions.insert(std::make_pair(client, session));
}

void GameServer::onDisconnect(Client *client) {
  std::cout << "client disconnected :(" << std::endl;

  // Remove any client connection metadata
  SessionMap::iterator iter = _sessions.find(client);
  if (iter != _sessions.end()) {
    delete iter->second;
    _sessions.erase(iter);
  }
}

void GameServer::onReceive(Packet *packet) {
  size_t size = packet->size();
  const void *data = packet->data();

  if (size < sizeof(NetPacketType)) {
    return;
  }

  Client *client = packet->sender();
  ClientSession *clientSession = session(client);
  
  try {
    
    const NetPacketType *type = static_cast<const NetPacketType *>(data);
    switch (*type) {
    case NET_IDENTIFY:
      assert(size >= sizeof(NetIdentifyMsg) && "packet too small for ident");
      onIdent(static_cast<const NetIdentifyMsg *>(data), packet);
      break;
    
    default:
      // Some sanity checks first
      if (!clientSession) {
        // kinda strange, it should already be connected here
        throw NetError(NET_NOT_CONNECTED, "please connect before issuing ident");
      }
      
      if (clientSession->state != ClientSession::STATE_IDENTIFIED) {
        throw NetError(NET_ALREADY_IDENTIFIED, "please identify first");
      }
      
      // Then call onReceive on the subsystems
      for (size_t i = 0; i < _systems.size(); ++i) {
        _systems[i]->onReceive(*type, *packet);
      }
      
    }
  }
  catch (const NetError &netError) {
    // Any failures during the ident progress will terminate the session
    
    NetErrorMsg msg;
    netError.fill(msg);
    client->send(&msg, sizeof(NetErrorMsg),
                 Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
      
    client->disconnect();
  }
  
}


void GameServer::onIdent(const NetIdentifyMsg *data, Packet *packet) {
  NetIdentifyMsg ident;
  ident.type = data->type;
  ident.client_version = ntohs(data->client_version);
  ident.net_version = ntohs(data->net_version);
  
  std::cout << "net: received ident for net " << ident.net_version << std::endl;
  Client *client = packet->sender();
  ClientSession *clientSession = session(client);

  // Some sanity checks first
  if (!clientSession) {
    // kinda strange, it should already be connected here
    throw NetError(NET_NOT_CONNECTED, "please connect before issuing ident");
  }
  
  if (clientSession->state != 0) {
    throw NetError(NET_ALREADY_IDENTIFIED, "you are already identified");
  }
  
  if (ident.net_version != NET_VERSION) {
    throw NetError(NET_IDENT_WRONG_VERSION, "wrong network version");
  }
  
  
  for (size_t i = 0; i < _systems.size(); ++i) {
    _systems[i]->onReceive(ident.type, *packet);
  }
  
  clientSession->state = ClientSession::STATE_IDENTIFIED;
  
  // Broadcast onIdent to all subsystems
  for (size_t i = 0; i < _systems.size(); ++i) {
    _systems[i]->onIdent(client);
  }      
}

ClientSession *GameServer::session(Client *client) const {
  SessionMap::const_iterator iter = _sessions.find(client);
  if (iter == _sessions.end()) {
    return NULL;
  }

  return iter->second;
}
