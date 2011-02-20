#include <game/common/players.h>
#include <game/server/gameserver.h>
#include <game/server/client_session.h>
#include <engine/network.h>
#include <engine/portal.h>

#include <iostream>
#include <cassert>

Players::Players() {
  _localPlayer = 0;
  _lastId = 0;
}

void Players::init(const Portal &interfaces) {
}


void Players::onTick(Client *client) {

}

void Players::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_ARENA_PARTICIPANTS) {
    // FIXME: there should probably be some kind of check here; the server
    //        shouldn't take updates from the clients!
    //        Similar in other replicated systems
    
    std::cout << "received participants" << std::endl;

    const NetArenaParticipantsMsg *pcips = static_cast<const NetArenaParticipantsMsg *>(
      packet.data());

    std::cout << "   client id: " << int(ntohs(pcips->client_id)) << std::endl;
    _localPlayer = ntohs(pcips->client_id);
    
    for (size_t i = 0; i < pcips->num_particips; ++i) {
      updatePlayer(pcips->pcips[i]);
      std::cout << "   pcip " << int(ntohs(pcips->pcips[i].id)) << ": '" << pcips->pcips[i].name << "'" << std::endl;
    }
  }
}

void Players::onIdent(Client *client) {
  assert(_players.size() < 255 && "too many players");

  PlayerId playerId = context->gameserver()->session(client)->player;
  
  size_t packetSize = sizeof(NetArenaParticipantsMsg) +
    sizeof(NetArenaParticipant) * _players.size();
  
  NetArenaParticipantsMsg *pcips = static_cast<NetArenaParticipantsMsg *>(
    malloc(packetSize));
  pcips->type = NET_ARENA_PARTICIPANTS;
  pcips->client_id = htons(playerId);
  pcips->num_particips = _players.size();

  for (size_t i = 0, e = _players.size(); i != e; ++i) {
    _players[i]->participant(pcips->pcips[i]);
  }

  client->send(pcips, packetSize, Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
  free(pcips);
}

Player *Players::player(PlayerId id) const {
  for (PlayerVector::const_iterator i = _players.begin(), e = _players.end();
       i != e; ++i) {
    if ((*i)->id() == id) {
      return *i;
    }
  }

  return static_cast<Player *>(0);
}

PlayerId Players::localPlayer() const {
  return _localPlayer;
}

Player *Players::createPlayer(ActorId actor) {
  Player *newPlayer = new Player(++_lastId, actor);
  _players.push_back(newPlayer);
  return newPlayer;
}


void Players::updatePlayer(const NetArenaParticipant &net_pant) {
  const PlayerId playerId = ntohs(net_pant.id);

  Player *playerEntry = player(playerId);
  if (!playerEntry) {
    std::cout << "NEW PLAYER: " << playerId << std::endl;
    const ActorId actorId = ntohs(net_pant.actor);
    playerEntry = new Player(playerId, actorId);
    _players.push_back(playerEntry);
  }
  else {
    playerEntry->update(net_pant);
  }
}



Player::Player(PlayerId id, ActorId actor)
  : _id(id)
  , _actor(actor)
{
}

PlayerId Player::id() const {
  return _id;
}

ActorId Player::actor() const {
  return _actor;
}

void Player::participant(NetArenaParticipant &pant) const {
  pant.id = htons(_id);
  pant.actor = htons(_actor);
  pant.flags = 0;
  strcpy(pant.name, "peter");
}

void Player::update(const NetArenaParticipant &pant) {
  _actor = ntohs(pant.actor);
}


