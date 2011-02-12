#ifndef GAME_GAMESERVER_H
#define GAME_GAMESERVER_H

#include <game/common/system.h>
#include <game/common/net_protocol.h>
#include <vector>

/** FIXME kaspars: Convert this to Doxygen diagram 
 * client   server
 * 
 * -----> connect
 * <----- ok
 * 
 * -----> id 
 * <----- init (spectator or player, snail positions)
 * 
 * <----- receive stuff (sounds, positions)
 * 
 * -----> control
 */

class GameServer : public System {
public:
  GameServer();
  ~GameServer();
  
  void init(const class Portal &interfaces);
  void update();
  void registerSystem(class ReplicatedSystem *system);
  
private:
  void onConnect(class Client *client);
  void onDisconnect(class Client *client);
  void onReceive(class Packet *packet);

  // net protocol
  void onIdent(const struct NetIdentifyMsg *ident, class Packet *packet);
  
  std::vector<class ReplicatedSystem *> _systems;
  class Host *_host;
};


#endif // !GAME_GAMESERVER_H
