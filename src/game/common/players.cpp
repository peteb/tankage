#include <game/common/players.h>
#include <engine/network.h>
#include <engine/portal.h>
#include <iostream>
#include <cassert>

void Players::init(const Portal &interfaces) {
  _localPlayer = 0;
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

    std::cout << "   client id: " << int(pcips->client_id) << std::endl;
    _localPlayer = pcips->client_id;
    
    for (size_t i = 0; i < pcips->num_particips; ++i) {
      std::cout << "   pcip " << int(pcips->pcips[i].id) << ": '" << pcips->pcips[i].name << "'" << std::endl;
    }
  }
}

void Players::onIdent(Client *client) {
  assert(_players.size() < 255 && "too many players");
  
  size_t packetSize = sizeof(NetArenaParticipantsMsg) +
    sizeof(NetArenaParticipant) * _players.size();
  
  NetArenaParticipantsMsg *pcips = static_cast<NetArenaParticipantsMsg *>(
    malloc(packetSize));
  pcips->type = NET_ARENA_PARTICIPANTS;
  pcips->client_id = htons(123);
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

PlayerId Player::id() const {
  return _id;
}

ActorId Player::actor() const {
  return _actor;
}

void Player::participant(NetArenaParticipant &pant) const {
  pant.id = htons(123);
  pant.flags = 0;
  strcpy(pant.name, "peter");
}


