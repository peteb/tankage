#include <game/server/gameserver.h>
#include <game/server/client_session.h>
#include <game/server/tank.h>
#include <game/server/bullet.h>
#include <game/server/resource.h>

#include <game/common/net_protocol.h>
#include <game/common/config.h>

#include <platform/packet.h>
#include <platform/network.h>
#include <platform/portal.h>

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
  : _world(b2Vec2(0.0f, 0.0f), false)
  , _map(this, &_world)
{  
  _net = services.requestInterface<Network>();
  
  _tick = 0;
  _last_entity = 0;
  
  Log(INFO) << "starting server at " << *server_host;
  _host = _net->startHost(*server_host, 32, 2);  
}

GameServer::~GameServer() {
  delete _host;
}

void GameServer::run() {
  double last_tick = _net->time() / 1000.0;
  
  while (true) {
    double this_tick = _net->time() / 1000.0;
    int timeout = clamp(last_tick + tickDuration() - this_tick, tickDuration() * 0.1, 0.0) * 1000.0;
    
    updateNet(timeout);
    
    this_tick = _net->time() / 1000.0;
    if (this_tick - last_tick >= tickDuration()) {
      onTick();

      last_tick = this_tick;
      _tick++;
    }
    _host->flush();
  }

}

void GameServer::onTick() {
  static std::vector<unsigned char> buffer;
  buffer.clear();
  buffer.reserve(5000);
  
  destroyZombies(); // updateNet might call onDisconnect which might destroyEntity

  for (size_t i = 0; i < _tanks.size(); ++i)
    _tanks[i]->tick();
  for (size_t i = 0; i < _bullets.size(); ++i)
    _bullets[i]->tick();
  for (size_t i = 0; i < _resources.size(); ++i)
    _resources[i]->tick();
  
  tickPhysics();
  _map.tick();
  
  for (size_t i = 0; i < _tanks.size(); ++i)
    _tanks[i]->postTick();
  
  SessionMap::iterator it = _sessions.begin(), it_e = _sessions.end();
  for (; it != it_e; ++it) {
    Packer msg(buffer);
    msg.writeShort(NET_SNAPSHOT);
    msg.writeInt(gameTick());

    // collect data from entities
    for (size_t i = 0; i < _tanks.size(); ++i)
      _tanks[i]->snap(msg, it->second);    
    for (size_t i = 0; i < _bullets.size(); ++i)
      _bullets[i]->snap(msg, it->second);
    for (size_t i = 0; i < _resources.size(); ++i)
      _resources[i]->snap(msg, it->second);

    // and add unreliable events to snapshot packet
    _events.snap(msg, it->second);
    msg.writeShort(0);
    
    it->second->client->send(&buffer[0], buffer.size(), 0, NET_CHANNEL_ABS);  
    
    // send reliable events
    buffer.clear();
    _events.writeReliable(msg, it->second);

    if (buffer.size() > 0) {
      msg.writeShort(0);
      it->second->client->send(&buffer[0], buffer.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);      
    }
    
    // send map updates in own packet
    buffer.clear();
    _map.snap(msg, it->second);

    if (buffer.size() > 0) {
      it->second->client->send(&buffer[0], buffer.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);      
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
  static std::vector<unsigned char> buffer;
  buffer.assign((const char *)packet->data(), 
                (const char *)packet->data() + packet->size());
  
  Unpacker msg(buffer);
  short type = msg.readShort();
  Client *client = packet->sender();

  // FIXME: if sending error and doing disconnect, wait with disconnect so it can send it first.
  
  // we only allow CLIENT_INFO to be received without a session.  
  if (type == NET_CLIENT_INFO) {
    std::string name = msg.readString();
    int cl_version = msg.readInt();
    
    if (cl_version != NET_VERSION) {
      Log(DEBUG) << "connecting client has wrong version";
      sendError(client, 10, "Wrong version!");
      client->disconnect();
      return;
    }

    if (_sessions.find(client) != _sessions.end()) {
      Log(DEBUG) << "client already has a session...?";
      sendError(client, 11, "You're already here.");
      client->disconnect();
      return;
    }
    
    
    ClientSession *new_s = new ClientSession(client);
    _sessions.insert(std::make_pair(client, new_s));
    Tank *tank = spawnTank();
    new_s->tankid = tank->id();

    sendServerInfo(client);

    Log(INFO) << "client " << new_s->tankid << " joined as '" << name << "'";
    new_s->name = name;
    _events.createPlayerJoined(new_s->tankid, name); // FIXME: CLIENT_INFO should probably be an update. join is a special case of INFO when a client hasn't received an INFO before
    
    sendPlayers(client);
  }
  else {
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
  }
}

void GameServer::sendPlayers(class Client *client) {
  static std::vector<unsigned char> buffer;
  buffer.clear();
  
  SessionMap::iterator it = _sessions.begin(), it_e = _sessions.end();
  for (; it != it_e; ++it) {
    if (it->second->client == client)
      continue;
    
    Packer reply_msg(buffer);
    reply_msg.writeShort(NET_PLAYER_INFO);
    reply_msg.writeInt(it->second->tankid);
    reply_msg.writeString(it->second->name);
    // FIXME: this is one "packet" per player.. maybe we should batch
    client->send(&buffer[0], buffer.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);     
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
  static std::vector<unsigned char> buffer;
  buffer.clear();
  
  Packer msg(buffer);
  msg.writeShort(NET_SERVER_INFO);
  msg.writeShort(*server_tickrate * 10.0);
  msg.writeInt(session(receiver)->tankid);
  
  receiver->send(&buffer[0], buffer.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
}

void GameServer::sendError(class Client *client, int code, 
                           const std::string &str) {
  static std::vector<unsigned char> buffer;
  buffer.clear();
  
  Packer msg(buffer);
  msg.writeShort(NET_ERROR);
  msg.writeInt(code);
  msg.writeString(str);
  client->send(&buffer[0], buffer.size(), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
  client->flush();
}

Tank *GameServer::spawnTank() {
  b2Body *tank_body = createTankBody();
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

Entity *GameServer::spawnResource(const vec2 &pos, char type) {
  Log(DEBUG) << "spawning resource " << int(type) << " at " << std::string(pos);
  
  Resource *resource = new Resource(this);
  Resource::State initial;
  initial.id = ++_last_entity;
  initial.type = type;
  initial.pos = pos;
  initial.start_tick = gameTick();
  resource->assign(initial);
  _resources.push_back(resource);
  
  return resource;  
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

  for (size_t i = 0; i < _resources.size(); ++i) {
    if (_resources[i]->id() == eid)
      return _resources[i];
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

  {
    std::vector<Resource *>::iterator iter = _resources.begin();
    while (iter != _resources.end()) {
      if (std::find(_zombie_entities.begin(), _zombie_entities.end(), (*iter)->id())
          != _zombie_entities.end())
        iter = _resources.erase(iter);
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

b2Body *GameServer::createTankBody() {
  b2BodyDef def;
  def.type = b2_dynamicBody;
  b2Body *body = _world.CreateBody(&def);
  
  b2PolygonShape box;
  box.SetAsBox(27.0f/64.0f, 25.0f/64.0f);
    
  body->CreateFixture(&box, 1.0f);
  
  return body;  
}

void GameServer::tickPhysics() {
  for (int i = 0; i < 6; ++i) {
    _world.Step(1.0f/60.0f, 6, 2);
    _world.ClearForces();
  }    
}

