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
const int MAX_PARTICIPANT_NAME = 32;

/**
 * Enumeration over the different packet types
 */
enum {  
  NET_IDENTIFY = 1,      //< NetIdentifyMsg
  NET_ERROR = 2,         //< NetErrorMsg
  NET_ARENA_PARTICIPANTS = 3,
  NET_TANKS_UPDATE = 4,
  NET_PROJECTILES_UPDATE = 5,
  NET_PLAYER_INPUT = 6,
  NET_SERVER_INFO = 7,
  NET_CLIENT_INFO = 8,
  NET_PLAYER_INFO = 9,
  NET_TANK_HIT = 10,
  NET_SNAPSHOT,
  NET_MAPCHUNK,
};

enum NetChannels {
  NET_CHANNEL_STATE = 0,      //< for relative data; ie, login, events, file
                              //  downloads, etc. usually reliable.
  NET_CHANNEL_ABS = 1,        //< absolute data, snapshots.
  NET_CHANNEL_MAX
};

enum NetErrorCode {
  NET_IDENT_WRONG_VERSION = 1,     //< the net version doesn't match the server
  NET_NOT_CONNECTED = 2,
  NET_NOT_IDENTIFIED = 3,
  NET_ALREADY_IDENTIFIED = 4,
};

/**
 * The generic packet for error messages/exceptions.
 */
struct NetErrorMsg {
  NetPacketType type;
  uint8_t error_code;
  char desc[MAX_ERRDESC];   //< c-string describing the problem
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


struct NetArenaParticipant {
  uint16_t id;
  uint16_t actor;
  uint8_t flags;
  char name[MAX_PARTICIPANT_NAME];
};


struct NetArenaParticipantsMsg {
  NetPacketType type;
  uint16_t client_id;
  uint8_t num_particips;
  NetArenaParticipant pcips[0];
};

struct NetTankSnapshot {
  uint16_t id;
  uint16_t x;
  uint16_t y;
  uint16_t base_dir;
  uint16_t turret_dir;
};

struct NetProjectileSnapshot {
  uint16_t id;
  uint16_t x;
  uint16_t y;
  uint16_t dir;
};

struct NetTanksSnapMsg {
  NetPacketType type;
  float last_input;
  uint8_t num_snapshots;
  NetTankSnapshot snaps[0];
};

struct NetProjectilesSnapMsg {
  NetPacketType type;
  uint8_t num_snapshots;
  NetProjectileSnapshot snaps[0];
};

struct NetPlayerInput {
  NetPacketType type;
  float time;
  uint8_t state; // forward, back, left, right, shoot
  uint16_t target_x;
  uint16_t target_y;
};

#endif // !GAME_COMMON_NET_PROTOCOL_H
