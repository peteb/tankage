#include <netinet/in.h>
#include <game/common/control.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/common/players.h>

#include <game/server/client_session.h>
#include <game/server/gameserver.h>

#include <engine/input.h>
#include <engine/cfg.h>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/network.h>

#include <utils/vec.h>

#include <arpa/inet.h>
#include <iostream>

Control::Control()
  : ReplicatedSystem(CLIENT_TICK|SERVER_RECEIVE)
  , moves(300)
{
  state.buttons = -1;
  state.aim_x = 0;
  state.aim_y = 0;
}

void Control::init(const class Portal &interfaces) {
  input = interfaces.requestInterface<Input>();
  config = interfaces.requestInterface<Config>();
  wm = interfaces.requestInterface<WindowManager>();
  lastTick = wm->timeSeconds();
  keyUp = input->keycode(config->property("control", "keyUp", "W"));
  keyDown = input->keycode(config->property("control", "keyDown", "S"));
  keyShoot = input->keycode(config->property("control", "keyShoot", "mouse1"));
  keyRight = input->keycode(config->property("control", "keyRight", "D"));
  keyLeft = input->keycode(config->property("control", "keyLeft", "A"));
}

void Control::update() {
  // Only run update on the client side
  ActorId actor = context->players()->localActor();
  if (actor == 0) {
    return;
  }
  
  Tank *target = context->actors()->tank(actor);
  if (!target) {
    return;
  }

  PlayerInput newState = currentState();
  if (state.buttons != newState.buttons || state.aim_x != newState.aim_x || state.aim_y != newState.aim_y) {
    // Input changed

    newState.time = wm->timeSeconds();
    state = newState;
    states[actor] = newState;

    Move stateMove;
    stateMove.delta = newState;
    stateMove.absolute = target->state();
    target->resetCount();
    moves.push_back(stateMove);
    toSend.push_back(newState);
  }
}

PlayerInput Control::currentState() const {
  PlayerInput ret;
  ret.buttons = 0;
  ret.aim_x = 0;
  ret.aim_y = 0;
  
  struct {PlayerInput::Buttons state; int keycode; } stateMap[] = {
    {PlayerInput::STATE_MOVE_UP, keyUp},
    {PlayerInput::STATE_MOVE_DOWN, keyDown},
    {PlayerInput::STATE_TURN_RIGHT, keyRight},
    {PlayerInput::STATE_TURN_LEFT, keyLeft},
    {PlayerInput::STATE_SHOOT, keyShoot}};

  /*
    SHOOTING is not really a move. It should not have an impact on
    prediction.
    Also, don't create projectiles on client. Only work on server data.
    
   */
  
  // Map keycodes to bits
  for (size_t i = 0; i < 5; ++i) {
    const int keycode = stateMap[i].keycode;
    const PlayerInput::Buttons state = stateMap[i].state;
      
    if (input->keyPressed(keycode)) {
      ret.buttons |= state;
    }
  }
    
  input->mousePos(ret.aim_x, ret.aim_y);

  return ret;
}

void Control::onTick(Client *client) {
  // Only tick on the client side
  ActorId actor = context->players()->localActor();
  if (actor == 0) {
    return;
  }
  
  Tank *target = context->actors()->tank(actor);
  if (!target) {
    return;
  }

  // FIXME: move NetPlayerInput -> Tank::Input to Input class

  for (size_t i = 0; i < toSend.size(); ++i) {
    NetPlayerInput msg;
    msg.type = NET_PLAYER_INPUT;
    msg.state = state.buttons;
    msg.target_x = htons(state.aim_x + 32768);
    msg.target_y = htons(state.aim_y + 32768);
    msg.time = state.time;
    
    client->send(&msg, sizeof(NetPlayerInput), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
  }

  toSend.clear();
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
