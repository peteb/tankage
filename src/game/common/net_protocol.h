#ifndef GAME_COMMON_NET_PROTOCOL_H
#define GAME_COMMON_NET_PROTOCOL_H

const int NET_VERSION = 10; // step this when data format changes

/**
 * Enumeration over the different packet types
 */
enum {  
  NET_PLAYER_INPUT = 1,
  NET_SERVER_INFO = 2,
  NET_CLIENT_INFO = 3,
  NET_PLAYER_INFO = 4,
  NET_TANK_HIT = 5,
  NET_SNAPSHOT = 6,
  NET_MAPCHUNK = 7,
  NET_TILE_UPDATE = 8,
  NET_ERROR = 9
};

enum NetChannels {
  NET_CHANNEL_STATE = 0,      //< for relative data; ie, login, events, file
                              //  downloads, etc. usually reliable.
  NET_CHANNEL_ABS = 1,        //< absolute data, snapshots.
  NET_CHANNEL_MAX
};

#endif // !GAME_COMMON_NET_PROTOCOL_H
