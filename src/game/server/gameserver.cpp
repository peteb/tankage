#include <game/server/gameserver.h>
#include <game/server/client_session.h>
#include <game/server/tank.h>
#include <game/server/bullet.h>

#include <game/common/net_protocol.h>
#include <game/common/config.h>

#include <engine/packet.h>
#include <engine/network.h>
#include <engine/portal.h>
#include <engine/window_manager.h>

#include <utils/packer.h>
#include <utils/log.h>

#include <cassert>
#include <cstring>
#include <algorithm>

Variable<std::string> server_host("0.0.0.0:12345");
Variable<double> server_tickrate(10.0);

void server_RegisterVariables(class Config &config) {
  config.registerVariable("server", "host", &server_host);
  config.registerVariable("server", "tickrate", &server_tickrate);  
}

GameServer::GameServer(const Portal &services) 
  : _map(this)
  , _world(this)
{  
  _net = services.requestInterface<Network>();
  //_wm = services.requestInterface<WindowManager>();
  
  _tick = 0;
  _last_entity = 0;
  
  Log(INFO) << "starting server at " << *server_host;
  _host = _net->startHost(*server_host, 32, 2);  
}

GameServer::~GameServer() {
  delete _host;
}

void GameServer::run() {
  
  while (true) {
    
    updateNet(1);
   	usleep(100 * 1000); 
      onTick();
	_tick++;
  }

}

void GameServer::onTick() {
  char buffer[4096];
  destroyZombies(); // updateNet might call onDisconnect which might destroyEntity

  for (size_t i = 0; i < _tanks.size(); ++i)
    _tanks[i]->tick();
  for (size_t i = 0; i < _bullets.size(); ++i)
    _bullets[i]->tick();
  
  _world.tick();
  
  for (size_t i = 0; i < _tanks.size(); ++i)
    _tanks[i]->postTick();
  
  SessionMap::iterator it = _sessions.begin(), it_e = _sessions.end();
  for (; it != it_e; ++it) {
    Packer msg(buffer, buffer + 4096);
    msg.writeShort(NET_SNAPSHOT);
    msg.writeInt(gameTick());

    // collect data from entities
    for (size_t i = 0; i < _tanks.size(); ++i)
      _tanks[i]->snap(msg, it->second);    
    for (size_t i = 0; i < _bullets.size(); ++i)
      _bullets[i]->snap(msg, it->second);

    // and add unreliable events to snapshot packet
    _events.snap(msg, it->second);
    msg.writeShort(0);
    
    it->second->client->send(buffer, msg.size(), 0, NET_CHANNEL_ABS);  
    
    // send reliable events
    msg.reset();
    _events.writeReliable(msg, it->second);

    if (msg.size() > 0) {
      msg.writeShort(0);
      it->second->client->send(buffer, msg.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);      
    }
    
    // send map updates in own packet
    msg.reset();
    _map.snap(msg, it->second);

    if (msg.size() > 0) {
      it->second->client->send(buffer, msg.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);      
    }    
  }  
  
  destroyZombies(); // ticks might destroyEntity
  
  _events.removeSnapped();
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
  
  
  ClientSession *session = new ClientSession(client);
  _sessions.insert(std::make_pair(client, session));
  Tank *tank = spawnTank();
  session->tankid = tank->id();

  sendServerInfo(client);
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

  ClientSession *sess = session(packet->sender());
  if (!sess)
    return;
  
  // TODO sometime: send snapshots at different intervals. players at 15, bullets at 5, etc.
  
  if (type == NET_PLAYER_INPUT) {
    Tank *tank = static_cast<Tank *>(entity(sess->tankid));
    if (tank) {
      Control::Input input;
      input.read(msg);
      tank->recvInput(input);
    }
  }
  else if (type == NET_CLIENT_INFO) { // should we use this for updating name??
    std::string name = msg.readString();
    Log(INFO) << "client " << sess->tankid << " joined as '" << name << "'";
    sess->name = name;
    _events.createPlayerJoined(sess->tankid, name); // FIXME: CLIENT_INFO should probably be an update. join is a special case of INFO when a client hasn't received an INFO before
    
    const int BUFSZ = 256;
    char buffer[BUFSZ];
    
    SessionMap::iterator it = _sessions.begin(), it_e = _sessions.end();
    for (; it != it_e; ++it) {
      if (it->second->client == packet->sender())
        continue;
      
      Packer reply_msg(buffer, buffer + BUFSZ);
      reply_msg.writeShort(NET_PLAYER_INFO);
      reply_msg.writeInt(it->second->tankid);
      reply_msg.writeString(it->second->name);
      // FIXME: this is one "packet" per player.. maybe we should batch
      packet->sender()->send(buffer, reply_msg.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);     
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

void GameServer::sendServerInfo(Client *receiver) {
  char buffer[1024];
  Packer msg(buffer, buffer + 1024);
  msg.writeShort(NET_SERVER_INFO);
  msg.writeShort(*server_tickrate * 10.0);
  msg.writeInt(session(receiver)->tankid);
  
  receiver->send(buffer, 1024, Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}


Tank *GameServer::spawnTank() {
  b2Body *tank_body = _world.createTankBody();
  Tank *tank = new Tank(this, tank_body);
  Tank::State initial;
  initial.id = ++_last_entity;
  initial.pos = vec2::FromDegrees(float(_last_entity * 90)) * 40.0f;
  initial.base_dir = 0.0f;
  initial.lin_vel = vec2::Zero();
  initial.rot_vel = 0.0f;
  initial.turret_dir = 0.0f;
  
  tank->assign(initial);
  _tanks.push_back(tank);
  return tank;
}

Entity *GameServer::spawnBullet(const vec2 &pos, const vec2 &vel, double dir, int shooter) {
  Bullet *bullet = new Bullet(this, shooter);
  Bullet::State initial;
  initial.id = ++_last_entity;
  initial.start_pos = pos;
  initial.start_vel = vel;
  initial.start_tick = gameTick();
  initial.dir = dir;
  initial.max_lerp = 10.0f;
  bullet->assign(initial);
  _bullets.push_back(bullet);
  
  return bullet;
}

Entity *GameServer::entity(int eid) const {
  for (size_t i = 0; i < _tanks.size(); ++i) {
    if (_tanks[i]->id() == eid)
      return _tanks[i];
  }

  for (size_t i = 0; i < _bullets.size(); ++i) {
    if (_bullets[i]->id() == eid)
      return _bullets[i];
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

  {
    std::vector<Tank *>::iterator iter = _tanks.begin();
    while (iter != _tanks.end()) {
      if (std::find(_zombie_entities.begin(), _zombie_entities.end(), (*iter)->id())
          != _zombie_entities.end())
        iter = _tanks.erase(iter);
      else
        ++iter;
    }
  }

  {
    std::vector<Bullet *>::iterator iter = _bullets.begin();
    while (iter != _bullets.end()) {
      if (std::find(_zombie_entities.begin(), _zombie_entities.end(), (*iter)->id())
          != _zombie_entities.end())
        iter = _bullets.erase(iter);
      else
        ++iter;
    }
  }
  
}

Tank *GameServer::intersectingTank(const vec2 &start, const vec2 &end, float radius, int ignore) {
  for (size_t i = 0; i < _tanks.size(); ++i) {
    Tank *tank = _tanks[i];
    if (tank->id() != ignore) {
      vec2 closest = closest_point(start, end, tank->state().pos);
      if (length(tank->state().pos - closest) <= (tank->radius() + radius)) {
        return tank;
      }
    }
  }
  
  return NULL;
}

Events &GameServer::events() {
  return _events;
}

Map &GameServer::map() {
  return _map;
}
