#ifndef GAME_GAMESERVER_H
#define GAME_GAMESERVER_H

#include <game/server/entity.h>
#include <game/server/events.h>
#include <game/server/map.h>

#include <vector>
#include <map>

#include <Box2D.h>

void server_RegisterVariables(class Config &config);

class GameServer {
public:
  GameServer(const class Portal &services);
  ~GameServer();
  
  void run();

  class ClientSession *session(class Client *client) const;
  unsigned int gameTick() const;
  double tickDuration() const;
  Entity *entity(int eid) const;
  void destroyEntity(int eid);
  Entity *spawnBullet(const vec2 &pos, const vec2 &vel, double dir, int shooter);
  class Tank *intersectingTank(const vec2 &start, const vec2 &end, float radius, int ignore);
  Events &events();
  Map &map();
  
private:
  typedef std::map<class Client *, class ClientSession *> SessionMap;
  
  void onConnect(class Client *client);
  void onDisconnect(class Client *client);
  void onReceive(class Packet *packet);
  void updateNet(int timeout = 0);
  void onTick();
  class Tank *spawnTank();
  b2Body *createTankBody();
  void destroyZombies();
  void sendServerInfo(class Client *receiver);
  void sendPlayers(class Client *client);
  void sendError(class Client *client, int code, const std::string &str);
  void tickPhysics();

  std::vector<class Tank *> _tanks;
  std::vector<class Bullet *> _bullets;
  std::vector<int> _zombie_entities;
  
  b2World _world;
  SessionMap _sessions;
  Events _events;
  Map _map;
  
  class Host *_host;
  class Network *_net;

  unsigned int _tick;
  unsigned int _last_entity;
};


#endif // !GAME_GAMESERVER_H
