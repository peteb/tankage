#ifndef GAME_GAMESERVER_H
#define GAME_GAMESERVER_H

#include <game/common/system.h>

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
  ~GameServer();
  
  void init(const class Portal &interfaces);
  void update();
  
private:
  void onConnect(class Client *client);
  void onDisconnect(class Client *client);
  void onReceive(class Packet *packet);

  // net protocol
  void onIdent(const struct NetIdentifyMsg *ident, class Packet *packet);
  
  class Host *_host;
};


#endif // !GAME_GAMESERVER_H
