#ifndef GAME_COMMON_REPLICATED_SYSTEM_H
#define GAME_COMMON_REPLICATED_SYSTEM_H

#include <game/common/net_protocol.h>
#include <game/common/system.h>

class ReplicatedSystem : public System {
public:
  virtual ~ReplicatedSystem() {}

  /**
   * Called when a client has identified.
   */
  virtual void onIdent(class Client *client) {};

  /**
   * Called each server tick.
   */
  virtual void onTick(class Client *client) =0;

  /**
   * A message has been received, broadcasted to all registered subsystems
   */
  virtual void onReceive(NetPacketType type, const class Packet &packet) =0;
};

#endif // !GAME_COMMON_REPLICATED_SYSTEM_H
