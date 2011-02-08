#ifndef GAME_COMMON_REPLICATED_SYSTEM_H
#define GAME_COMMON_REPLICATED_SYSTEM_H

#include <vector>


class ReplicatedSystem {
public:
  virtual ~ReplicatedSystem() {}
  
  virtual void writeFull(class PacketWriter &packet) =0;
  virtual void readFull(class PacketReader &packet) =0;
};

#endif // !GAME_COMMON_REPLICATED_SYSTEM_H
