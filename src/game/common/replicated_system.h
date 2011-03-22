#ifndef GAME_COMMON_REPLICATED_SYSTEM_H
#define GAME_COMMON_REPLICATED_SYSTEM_H

#include <game/common/net_protocol.h>
#include <game/common/system.h>

class ReplicatedSystem : public System {
public:
  enum {
    SERVER_TICK      = 0x0001,
    CLIENT_TICK      = 0x0002,
    SERVER_RECEIVE   = 0x0004,
    CLIENT_RECEIVE   = 0x0008
  };
  
  ReplicatedSystem(unsigned flags = 0xFFFFFFFF) : flags(flags) {}
  
  virtual ~ReplicatedSystem() {}

  /**
   * Called when a client has identified.
   */
  virtual void onIdent(class Client *client) {};

  /**
   * Called each server tick.
   */
  virtual void onTick() =0;

  /**
   * A message has been received, broadcasted to all registered subsystems
   */
  virtual void onReceive(NetPacketType type, const class Packet &packet) {};

  const unsigned flags;
};

#endif // !GAME_COMMON_REPLICATED_SYSTEM_H
