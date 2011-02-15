#include <game/common/players.h>
#include <engine/network.h>
#include <engine/portal.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

void Players::init(const Portal &interfaces) {

}


void Players::onTick(Client *client) {

}

void Players::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_ARENA_PARTICIPANTS) {
    std::cout << "received participants" << std::endl;

    const NetArenaParticipantsMsg *pcips = static_cast<const NetArenaParticipantsMsg *>(
      packet.data());

    std::cout << "   client id: " << int(pcips->client_id) << std::endl;

    for (size_t i = 0; i < pcips->num_particips; ++i) {
      std::cout << "   pcip " << int(pcips->pcips[i].id) << ": '" << pcips->pcips[i].name << "'" << std::endl;
    }
  }
}

void Players::onIdent(Client *client) {
  size_t packetSize = sizeof(NetArenaParticipantsMsg) +
    sizeof(NetArenaParticipant) * 1;
  
  void *data = malloc(packetSize);
  
  NetArenaParticipantsMsg *pcips = static_cast<NetArenaParticipantsMsg *>(data);
  pcips->type = NET_ARENA_PARTICIPANTS;
  pcips->client_id = 123;
  pcips->num_particips = 1;


  NetArenaParticipant *pant = pcips->pcips;
  pant->id = 123;
  pant->flags = 0;
  strcpy(pant->name, "peter");

  client->send(data, packetSize, Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
  free(data);
}
