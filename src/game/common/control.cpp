#include <netinet/in.h>
#include <game/common/control.h>
#include <game/common/players.h>
#include <game/common/config.h>

#include <game/server/client_session.h>
#include <game/server/gameserver.h>
#include <game/client/gameclient.h>

#include <engine/input.h>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/network.h>

#include <utils/vec.h>
#include <utils/packer.h>

#include <arpa/inet.h>
#include <iostream>

Variable<std::string> control_keyUp("W");
Variable<std::string> control_keyDown("S");
Variable<std::string> control_keyLeft("A");
Variable<std::string> control_keyRight("D");
Variable<std::string> control_keyShoot("mouse1");

void control_RegisterVariables(class Config &config) {
  config.registerVariable("control", "key_up", &control_keyUp);
  config.registerVariable("control", "key_down", &control_keyDown);
  config.registerVariable("control", "key_left", &control_keyLeft);
  config.registerVariable("control", "key_right", &control_keyRight);
  config.registerVariable("control", "key_shoot", &control_keyShoot);    
}

Control::Control(const Portal &interfaces) {
  _input = interfaces.requestInterface< ::Input>();
  _wm = interfaces.requestInterface<WindowManager>();

  reloadKeycodes();
}

void Control::reloadKeycodes() {
  keyUp = _input->keycode(*control_keyUp);
  keyDown = _input->keycode(*control_keyDown);
  keyLeft = _input->keycode(*control_keyLeft);
  keyRight = _input->keycode(*control_keyRight);
  keyShoot = _input->keycode(*control_keyShoot);
}

/* <--- begin input state ---> */
Control::Input &Control::Input::write(class Packer &msg) {
  msg.writeShort(buttons); // fixme: writeByte
  return *this;
}

Control::Input &Control::Input::read(class Unpacker &msg) {
  buttons = msg.readShort();
  return *this;
}
/* <--- end input state ---> */


Control::Input Control::currentInput() const {
  Input ret;
  ret.buttons = 0;
/*  ret.aim_x = 0;
  ret.aim_y = 0;
  */
  struct {Input::Commands state; int keycode; } stateMap[] = {
    {Input::FORWARD, keyUp}/*,
    {Input::STATE_MOVE_DOWN, keyDown},
    {Input::STATE_TURN_RIGHT, keyRight},
    {Input::STATE_TURN_LEFT, keyLeft},
    {Input::STATE_SHOOTING, keyShoot}*/};

  /*
    SHOOTING is not really a move. It should not have an impact on
    prediction.
    Also, don't create projectiles on client. Only work on server data.
    
   */
  
  // Map keycodes to bits
  for (size_t i = 0; i < 5; ++i) {
    const int keycode = stateMap[i].keycode;
    const Input::Commands state = stateMap[i].state;
      
    if (_input->keyPressed(keycode)) {
      ret.buttons |= state;
    }
  }
    
  //input->mousePos(ret.aim_x, ret.aim_y);

  return ret;
}

/*void Control::onTick() {
#if 0
  double thisTick = context->gameclient()->localTime();
  if (thisTick - lastTick < 1.0/10.0) {
    return;
  }
  
  lastTick = thisTick;
  // Only run update on the client side
  ActorId actor = context->players()->localActor();
  if (actor == 0) {
    return;
  }
  
  Tank *target = context->actors()->tank(actor);
  if (!target) {
    return;
  }
  
  PlayerInput state = currentState();    
  state.time = thisTick;
  states[actor] = state;
    
  Move move;
  move.delta = state;
  move.absolute = target->state();
  target->resetCount();
  moves.push_back(move);

  // FIXME: move NetPlayerInput -> Tank::Input to Input class

  
  NetPlayerInput msg;
  msg.type = NET_PLAYER_INPUT;
  msg.state = state.buttons;
  msg.target_x = htons(state.aim_x + 32768);
  msg.target_y = htons(state.aim_y + 32768);
  msg.time = state.time;
    
  std::cout << "sending" << std::endl;
  client->send(&msg, sizeof(NetPlayerInput), 0, NET_CHANNEL_STATE);
#endif
}

void Control::onReceive(NetPacketType type, const Packet &packet) {
  if (type == NET_PLAYER_INPUT) {
    if (packet.size() < sizeof(NetPlayerInput)) {
      return;
    }
    
    const NetPlayerInput *msg = static_cast<const NetPlayerInput *>(packet.data());
    Client *client = packet.sender();
    if (!client) {
      return;
    }
    
    ClientSession *session = context->gameserver()->session(client);
    if (!session) {
      // No session? Huh.
      return;
    }

    Player *player = context->players()->player(session->player);
    if (!player) {
      return;
    }

    Tank *tank = context->actors()->tank(player->actor());
    if (!tank) {
      return;
    }

    const PlayerInput *prev = lastInput(player->actor());
    if (prev) {
      if (msg->time <= prev->time) {
        return;
      }
    }
    
    PlayerInput state;
    state.time = msg->time;
    state.rxtime = 0.0; //context->gameserver()->localTime();
    state.buttons = msg->state;
    state.aim_x = ntohs(msg->target_x) - 32768;
    state.aim_y = ntohs(msg->target_y) - 32768;
    states[player->actor()] = state;
    tank->resetCount();
  }
}

const PlayerInput *Control::lastInput(ActorId actor) const {
  std::map<ActorId, PlayerInput>::const_iterator iter = states.find(actor);
  if (iter == states.end())
    return NULL;

  return &iter->second;
}

Control::MoveRange Control::history(float time) {
  MoveBuffer::reverse_iterator iter = moves.rbegin(), last = iter;
  for (; iter != moves.rend(); ++iter) {
    if (iter->delta.time < time) 
      break;
    
    last = iter;
  }

  // FIXME: better name
  MoveBuffer::iterator baurgh = MoveBuffer::iterator(iter.base());
  if (iter == moves.rend())
    ++baurgh;
  
  return Control::MoveRange(baurgh, moves.end());
}

void Control::removeHistory(const MoveBuffer::iterator &first) {
  moves.erase(moves.begin(), first);
}
*/