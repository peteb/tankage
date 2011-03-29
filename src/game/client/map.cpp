#include "map.h"
#include <utils/packer.h>
#include <utils/log.h>

void ClientMap::addChunk(Unpacker &msg) {
  short tiles = msg.readShort();
  Log(DEBUG) << "got map chunk containing " << tiles << " tiles";
}
