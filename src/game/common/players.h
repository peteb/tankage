#ifndef GAME_COMMON_PLAYERS_H
#define GAME_COMMON_PLAYERS_H

#include <game/common/replicated_system.h>
#include <game/common/actors.h>
#include <game/common/net_protocol.h>
#include <vector>

typedef int PlayerId;

class Player {
public:
  PlayerId id() const;
  ActorId actor() const;

  void participant(NetArenaParticipant &pant) const;
  
private:
  PlayerId _id;
  ActorId _actor;
};

class Players : public ReplicatedSystem {
public:
  void init(const class Portal &interfaces);

  void onTick(class Client *client);
  void onReceive(NetPacketType type, const class Packet &packet);
  void onIdent(class Client *client);
  Player *player(PlayerId id) const;
  PlayerId localPlayer() const;

private:
  typedef std::vector<Player *> PlayerVector;
  PlayerVector _players;
  PlayerId _localPlayer;
};

#endif // !GAME_COMMON_PLAYERS_H
