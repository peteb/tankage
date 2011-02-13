#ifndef GAME_COMMON_REPLICATED_SYSTEM_H
#define GAME_COMMON_REPLICATED_SYSTEM_H

#include <game/common/net_protocol.h>
#include <vector>

typedef std::vector<class PacketWriter *> ChannelList;

class ReplicatedSystem {
public:
  virtual ~ReplicatedSystem() {}

  /**
   * Called when a client has identified.
   * @param channels the system can write stuff into these
   *        the system on the other end will get a call to onReceive.
   */
//  virtual void onIdent(class Client *client) {};

  /**
   * Called each server tick.
   * @param channels the system can write stuff into these
   */
  virtual void onTick(class Client *client) =0;

  /**
   * The system has received an update.
   * @param channelId the channel the data was received on
   * @param reader for accessing the data
   */
  virtual void onReceive(NetPacketType type, class Packet *packet) =0;
};

///////////////////////////////////////
// Fixme: NetError(CODE, "blaha blaha")

#endif // !GAME_COMMON_REPLICATED_SYSTEM_H
