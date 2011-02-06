#ifndef GAME_COMMON_NET_PROTOCOL_H
#define GAME_COMMON_NET_PROTOCOL_H

#include <stdint.h> // hey...

/**
 * A packet is made up like this:
 * [8 bit identifier (HeaderTypeT)]
 * [Header-specific data (depending on the identifier)]
 */


typedef uint8_t NetPacketType;
typedef uint16_t NetVersion;

const NetVersion netVersion = 1;

/**
 * Enumeration over the different packet types
 */
enum {
  NET_IDENTIFY = 1
};

enum NetChannels {
  NET_CHANNEL_STATE = 0,      //< for relative data; ie, login, events, file
                              //  downloads, etc. usually reliable.
  NET_CHANNEL_ABS = 1         //< absolute data, snapshots.
};

/**
 * Message sent when the connection has been established.
 */
struct NetIdentifyMsg {
  NetPacketType type;
  NetVersion client_version;
  NetVersion net_version;    //< the netcode id; different versions
                             //  will break compatibility
  
  
  
};

#endif // !GAME_COMMON_NET_PROTOCOL_H
