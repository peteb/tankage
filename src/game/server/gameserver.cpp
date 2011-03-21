#include <game/server/gameserver.h>
#include <game/server/client_session.h>

#include <game/common/net_protocol.h>
#include <game/common/net_error.h>
#include <game/common/replicated_system.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/common/config.h>

#include <engine/packet.h>
#include <engine/network.h>
#include <engine/portal.h>
#include <engine/logging.h>
#include <engine/window_manager.h>

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
  _time = 0.0;
  _lasttick = 0.0;
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
  
  _lasttick = _wm->timeSeconds();
}

void GameServer::start() {
  std::cout << "starting server at " << *server_host << std::endl;
  _host = _net->startHost(*server_host, 32, 2);  
}

void GameServer::update() {
  _host->update(); // Fixme: timeout should maybe be the time until next update
  
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
  
  double now = _wm->timeSeconds();
  if (now - _lasttick >= 1.0 / *server_tickrate) {
    tick(now - _lasttick);
    _lasttick = now;
  }
}

void GameServer::tick(double dt) {

  for (size_t i = 0; i < _systems.size(); ++i) {
    for (SessionMap::iterator it = _sessions.begin(), e = _sessions.end();
         it != e; ++it) {

      if (_systems[i]->flags & ReplicatedSystem::SERVER_TICK) {
        _systems[i]->onTick(it->second->client);
      }
    }
  }

  _time += dt;
}


void GameServer::onConnect(Client *client) {
  //_log->write(Logging::DEBUG, "new client: %s ! :DD", 
    //client->address().c_str());    
  if (_sessions.find(client) != _sessions.end()) {
    // The client already exists, weird..
    //_log->write(Logging::DEBUG, "client already exists!");
    // Fixme: throw exception, disconnect the connection
    return;
  }
  
  ClientSession *session = new ClientSession(client);
  _sessions.insert(std::make_pair(client, session));
}

void GameServer::onDisconnect(Client *client) {
  //_log->write(Logging::DEBUG, "client disconnected :(");

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
  for (size_t i = 0; i < _systems.size(); ++i) {
    if (_systems[i]->flags & ReplicatedSystem::SERVER_RECEIVE) {
      _systems[i]->onReceive(ident.type, *packet);
    }
  }
  
  clientSession->state = ClientSession::STATE_IDENTIFIED;

  Tank *tank = context->actors()->createActor();
  Player *player = context->players()->createPlayer(tank->id());
  clientSession->player = player->id();
  
  // Broadcast onIdent to all subsystems
  for (size_t i = 0; i < _systems.size(); ++i) {
    _systems[i]->onIdent(client);
  }
}

double GameServer::localTime() const {
  return _time;
}

ClientSession *GameServer::session(Client *client) const {
  SessionMap::const_iterator iter = _sessions.find(client);
  if (iter == _sessions.end()) {
    return NULL;
  }

  return iter->second;
}
