#ifndef GAME_COMMON_NET_PROTOCOL_H
#define GAME_COMMON_NET_PROTOCOL_H

#include <stdint.h> // hey...

/**
 * A packet is made up like this:
 * [8 bit identifier (HeaderTypeT)]
 * [Header-specific data (depending on the identifier)]
 *
 * Changes in this file should result in an incrementation of the network
 * version.
 */


typedef uint8_t NetPacketType;
typedef uint16_t NetVersion;

const NetVersion NET_VERSION = 1;
const int MAX_ERRDESC = 64;

/**
 * Enumeration over the different packet types
 */
enum {
  NET_IDENTIFY = 1,      //< NetIdentifyMsg
  NET_ERROR = 2,         //< NetErrorMsg
  NET_SYSTEM = 3
};

enum NetChannels {
  NET_CHANNEL_STATE = 0,      //< for relative data; ie, login, events, file
                              //  downloads, etc. usually reliable.
  NET_CHANNEL_ABS = 1         //< absolute data, snapshots.
};

enum NetError {
  NET_IDENT_WRONG_VERSION = 1   //< the net version doesn't match the server
};

enum NetSystems {
  NET_SYSTEM_SNAILS = 0,
  NET_SYSTEM_MAX
};

/**
 * Message sent when the connection has been established.
 */
struct NetIdentifyMsg {
  NetPacketType type;
  NetVersion client_version;
  NetVersion net_version;    //< the netcode id; different versions
                             //  will break compatibility.
};

/**
 * The generic packet for error messages/exceptions.
 */
struct NetErrorMsg {
  NetPacketType type;
  uint8_t error_code;
  char desc[MAX_ERRDESC];   //< c-string describing the problem
};

struct NetSystemMsg {
  NetPacketType type;
  uint32_t systems;
};

#endif // !GAME_COMMON_NET_PROTOCOL_H
