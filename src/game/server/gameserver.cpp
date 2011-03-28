#include <game/server/gameserver.h>
#include <game/server/client_session.h>
#include <game/server/tank.h>

#include <game/common/net_protocol.h>
#include <game/common/net_error.h>
#include <game/common/config.h>

#include <engine/packet.h>
#include <engine/network.h>
#include <engine/portal.h>
#include <engine/window_manager.h>

#include <utils/packer.h>
#include <utils/log.h>

#include <iostream>
#include <cassert>
#include <cstring>
#include <algorithm>

Variable<std::string> server_host("0.0.0.0:12345");
Variable<double> server_tickrate(10.0);

void server_RegisterVariables(class Config &config) {
  config.registerVariable("server", "host", &server_host);
  config.registerVariable("server", "tickrate", &server_tickrate);  
}

GameServer::GameServer(const Portal &services) {  
  _net = services.requestInterface<Network>();
  _wm = services.requestInterface<WindowManager>();
  
  _tick = 0;
  _last_entity = 0;
  
  Log(INFO) << "starting server at " << *server_host;
  _host = _net->startHost(*server_host, 32, 2);  
}

GameServer::~GameServer() {
  delete _host;
}

void GameServer::run() {
  double lasttick = _wm->timeSeconds();
  
  while (true) {
    double dt = _wm->timeSeconds() - lasttick;
    double interval = tickDuration();
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
  char buffer[4096];

  destroyZombies(); // updateNet might call onDisconnect which might destroyEntity

  for (size_t i = 0; i < _entities.size(); ++i) {
    _entities[i]->tick();
  }
  
  destroyZombies(); // ticks might destroyEntity
  
  SessionMap::iterator it = _sessions.begin(), it_e = _sessions.end();
  for (; it != it_e; ++it) {
    Packer msg(buffer, buffer + 4096);
    msg.writeShort(NET_SNAPSHOT);
    msg.writeInt(gameTick());

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

double GameServer::tickDuration() const {
  return 1.0 / *server_tickrate;
}

void GameServer::onConnect(Client *client) {
  Log(INFO) << "got connection from " << client->address();
  
  if (_sessions.find(client) != _sessions.end()) {
    // FIXME:  disconnect the connection
    return;
  }
  
  sendServerInfo(client);
  
  ClientSession *session = new ClientSession(client);
  _sessions.insert(std::make_pair(client, session));
  Tank *tank = spawnTank();
  session->tankid = tank->id();
}

void GameServer::onDisconnect(Client *client) {
  Log(INFO) << "disconnected: " << client->address();

  // Remove any client connection metadata
  SessionMap::iterator iter = _sessions.find(client);
  if (iter != _sessions.end()) {
    destroyEntity(iter->second->tankid);
    delete iter->second;
    _sessions.erase(iter);
  }
}

void GameServer::onReceive(Packet *packet) {
  Unpacker msg(packet->data(), (const unsigned char *)packet->data() + packet->size());
  short type = msg.readShort();
  
  if (type == NET_PLAYER_INPUT) { // INPUT
    ClientSession *sess = session(packet->sender());
    if (!sess)
      return;
    
    Tank *tank = static_cast<Tank *>(entity(sess->tankid));
    if (tank) {
      Control::Input input;
      input.read(msg);
      tank->recvInput(input);
    }
  }
}

ClientSession *GameServer::session(Client *client) const {
  SessionMap::const_iterator iter = _sessions.find(client);
  if (iter == _sessions.end()) {
    return NULL;
  }

  return iter->second;
}

Tank *GameServer::spawnTank() {
  Tank *tank = new Tank(this);
  Tank::State initial;
  initial.id = ++_last_entity;
  initial.pos = vec2(400.0f, 300.0f);
  initial.base_dir = 0.0f;
  initial.lin_vel = vec2::Zero();
  initial.rot_vel = 0.0f;
  
  tank->assign(initial);
  _entities.push_back(tank);
  return tank;
}

Entity *GameServer::entity(int eid) const {
  for (size_t i = 0; i < _entities.size(); ++i) {
    if (_entities[i]->id() == eid)
      return _entities[i];
  }
  
  return NULL;
}

void GameServer::destroyEntity(int eid) {
  _zombie_entities.push_back(eid);
}

void GameServer::destroyZombies() {
  if (_zombie_entities.empty())
    return;
  
  // FIXME: optimize this!
  
  std::vector<Entity *>::iterator iter = _entities.begin();
  while (iter != _entities.end()) {
    if (std::find(_zombie_entities.begin(), _zombie_entities.end(), (*iter)->id())
        != _zombie_entities.end())
      iter = _entities.erase(iter);
    else
      ++iter;
  }
}

void GameServer::sendServerInfo(Client *receiver) {
  char buffer[1024];
  Packer msg(buffer, buffer + 1024);
  msg.writeShort(NET_SERVER_INFO);
  msg.writeShort(*server_tickrate * 10.0);
  
  receiver->send(buffer, 1024, Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}
