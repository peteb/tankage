#include <game/server/gameserver.h>
#include <game/server/client_session.h>
#include <game/server/tank.h>

#include <game/common/net_protocol.h>
#include <game/common/net_error.h>
#include <game/common/replicated_system.h>
#include <game/common/config.h>

#include <engine/packet.h>
#include <engine/network.h>
#include <engine/portal.h>
#include <engine/window_manager.h>

#include <utils/packer.h>
#include <utils/log.h>

#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>

Variable<std::string> server_host("0.0.0.0:12345");
Variable<double> server_tickrate(20.0);

GameServer::GameServer() 
  : _host(0)
  , _log(0) 
{  
  _tick = 0;
}

GameServer::~GameServer() {
  delete _host;
}

void GameServer::init(const class Portal &interfaces) {
  _net = interfaces.requestInterface<Network>();
  _wm = interfaces.requestInterface<WindowManager>();
  
  Config *config = context->system<Config>(SystemContext::SYSTEM_CONFIG);
  config->registerVariable("server", "host", &server_host);
  config->registerVariable("server", "tickrate", &server_tickrate);
}

void GameServer::start() {
  Log(INFO) << "starting server at " << *server_host;
  _host = _net->startHost(*server_host, 32, 2);  
}

void GameServer::run() {
  double lasttick = _wm->timeSeconds();
  
  while (true) {
    double dt = _wm->timeSeconds() - lasttick;
    double interval = tickSpeed();
    int timeout = static_cast<int>(std::max(interval - dt, 0.0) * 1000.0);
    
    updateNet(timeout);
    
    if (dt >= interval) {
      onTick();
      lasttick += interval;
      _tick++;
    }

  }

}

void GameServer::onTick() {
  SessionMap::iterator it = _sessions.begin();
  SessionMap::iterator it_end = _sessions.end();

  char buffer[1024];
  Packer msg(buffer, buffer + 1024);
  msg.writeShort(NET_SNAPSHOT);
  msg.writeInt(gameTick());
  
  for (; it != it_end; ++it) {
    // collect data from entities
    for (size_t i = 0; i < _entities.size(); ++i) {
      _entities[i]->snap(msg, it->second);
    }

    msg.writeShort(0);
    it->second->client->send(buffer, std::max(msg.size(), msg.size()), 0, NET_CHANNEL_ABS);    
  }  
}

void GameServer::updateNet(int timeout) {
  _host->update(timeout);
  
  // get all the action
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

unsigned int GameServer::gameTick() const {
  return _tick;
}

double GameServer::tickSpeed() const {
  return 1.0 / *server_tickrate;
}

void GameServer::onConnect(Client *client) {
  Log(INFO) << "got connection from " << client->address();
  
  if (_sessions.find(client) != _sessions.end()) {
    // FIXME:  disconnect the connection
    return;
  }
  
  ClientSession *session = new ClientSession(client);
  _sessions.insert(std::make_pair(client, session));
  spawnTank();
}

void GameServer::onDisconnect(Client *client) {
  Log(INFO) << "disconnected: " << client->address();

  // Remove any client connection metadata
  SessionMap::iterator iter = _sessions.find(client);
  if (iter != _sessions.end()) {
    delete iter->second;
    _sessions.erase(iter);
  }
}

void GameServer::onReceive(Packet *packet) {
  Unpacker msg(packet->data(), (const unsigned char *)packet->data() + packet->size());
  short type = msg.readShort();
  
  if (type == 1) { // INPUT
                   //context->control()->onRecvInput(msg);
  }
  
  /*size_t size = packet->size();
  const void *data = packet->data();

  if (size < sizeof(NetPacketType)) {
    return;
  }

  Client *client = packet->sender();
  ClientSession *clientSession = session(client);
  
  try {
    const NetPacketType *type = static_cast<const NetPacketType *>(data);
    if (*type == NET_IDENTIFY) {
      assert(size >= sizeof(NetIdentifyMsg) && "packet too small for ident");
      onIdent(static_cast<const NetIdentifyMsg *>(data), packet);
    }
    else {
      // Some sanity checks first
      if (!clientSession) {
        // kinda strange, it should already be connected here
        throw NetError(NET_NOT_CONNECTED, "please connect first");
      }
      
      if (clientSession->state != ClientSession::STATE_IDENTIFIED) {
        throw NetError(NET_ALREADY_IDENTIFIED, "please identify first");
      }
      
      // Then call onReceive on the subsystems
      for (size_t i = 0; i < _systems.size(); ++i) {
        if (_systems[i]->flags & ReplicatedSystem::SERVER_RECEIVE) {
          _systems[i]->onReceive(*type, *packet);
        }
      }
    }
    
  }
  catch (const NetError &netError) {
    // Any failures will terminate the session
    NetErrorMsg msg;
    netError.fill(msg);
    client->send(&msg, sizeof(NetErrorMsg),
                 Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
      
    client->disconnect();
  }
  */
}


void GameServer::onIdent(const NetIdentifyMsg *data, Packet *packet) {
  NetIdentifyMsg ident;
  ident.type = data->type;
  ident.client_version = ntohs(data->client_version);
  ident.net_version = ntohs(data->net_version);
  
  //_log->write(Logging::DEBUG, "net: received ident for net %u", 
    //ident.net_version);

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
  
  // Forward the ident request to all subsystems
  /*for (size_t i = 0; i < _systems.size(); ++i) {
    if (_systems[i]->flags & ReplicatedSystem::SERVER_RECEIVE) {
      _systems[i]->onReceive(ident.type, *packet);
    }
  }*/
  //context->actors()->onReceive(ident.type, *packet);
  
  clientSession->state = ClientSession::STATE_IDENTIFIED;

  
  // FIXME: ******************
  //Tank *tank = context->actors()->createActor();
  //Player *player = context->players()->createPlayer(tank->id());
  //clientSession->player = player->id();
  // FIXME: ******************

  
  // Broadcast onIdent to all subsystems
  /*for (size_t i = 0; i < _systems.size(); ++i) {
    _systems[i]->onIdent(client);
  }*/
}


ClientSession *GameServer::session(Client *client) const {
  SessionMap::const_iterator iter = _sessions.find(client);
  if (iter == _sessions.end()) {
    return NULL;
  }

  return iter->second;
}

Tank *GameServer::spawnTank() {
  Tank *newTank = new Tank;
  _entities.push_back(newTank);
  return newTank;
}