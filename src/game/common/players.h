#ifndef GAME_COMMON_PLAYERS_H
#define GAME_COMMON_PLAYERS_H

#include <game/common/replicated_system.h>
#include <game/common/actors.h>
#include <game/common/net_protocol.h>
#include <vector>

typedef int PlayerId;

class Player {
public:
  Player(PlayerId id, ActorId actor);
  
  PlayerId id() const;
  ActorId actor() const;

  void participant(NetArenaParticipant &pant) const;
  void update(const NetArenaParticipant &pant);
  
private:
  PlayerId _id;
  ActorId _actor;
};

class Players : public ReplicatedSystem {
public:
  static SystemContext::SystemId id() {
    return SystemContext::SYSTEM_PLAYERS;
  }

  Players();
  void init(const class Portal &interfaces);

  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  void onIdent(class Client *client);
  Player *player(PlayerId id) const;
  PlayerId localPlayer() const;
  Player *createPlayer(ActorId actor);
  ActorId localActor() const;
  
private:
  void updatePlayer(const NetArenaParticipant &participant);
  
  typedef std::vector<Player *> PlayerVector;
  PlayerVector _players;
  PlayerId _localPlayer;
  PlayerId _lastId;
};

#endif // !GAME_COMMON_PLAYERS_H
