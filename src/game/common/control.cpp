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
  inputBegan = 0.0;
  state.buttons = 0;
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

  Tank::Input newState = currentState();
  if (state.buttons != newState.buttons || state.aim_x != newState.aim_x || state.aim_y != newState.aim_y) {
    // Input changed

    const double timeNow = wm->timeSeconds();
    const double stateTime = timeNow - inputBegan;
    // FIXME: store state + stateTime in input history

    state = newState;
    inputBegan = timeNow;
    state.time = timeNow;
    states[actor] = newState;
  }
}

Tank::Input Control::currentState() const {
  Tank::Input ret;
  ret.buttons = 0;
  ret.aim_x = 0;
  ret.aim_y = 0;
  
  struct {Tank::Buttons state; int keycode; } stateMap[] = {
    {Tank::STATE_MOVE_UP, keyUp},
    {Tank::STATE_MOVE_DOWN, keyDown},
    {Tank::STATE_TURN_RIGHT, keyRight},
    {Tank::STATE_TURN_LEFT, keyLeft},
    {Tank::STATE_SHOOT, keyShoot}};

  // Map keycodes to bits
  for (size_t i = 0; i < 5; ++i) {
    const int keycode = stateMap[i].keycode;
    const Tank::Buttons state = stateMap[i].state;
      
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

  static NetPlayerInput lastMsg;
  NetPlayerInput msg;
  msg.type = NET_PLAYER_INPUT;
  msg.state = state.buttons;
  msg.target_x = htons(state.aim_x + 32768);
  msg.target_y = htons(state.aim_y + 32768);
  msg.time = inputBegan;
  
  if (lastMsg.state != msg.state || lastMsg.target_x != msg.target_x || lastMsg.target_y != msg.target_y) {
    client->send(&msg, sizeof(NetPlayerInput), Client::PACKET_RELIABLE, NET_CHANNEL_STATE);
    lastMsg = msg;

    // Add the move to the buffer of previous commands
    Move lastMove;
    lastMove.time = wm->timeSeconds() - inputBegan;
    lastMove.delta = state;
    lastMove.absolute = target->snapshot();
    moves.push_back(lastMove);
  }
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

    Tank::Input state;
    state.time = msg->time;
    state.buttons = msg->state;
    state.aim_x = ntohs(msg->target_x) - 32768;
    state.aim_y = ntohs(msg->target_y) - 32768;
    states[player->actor()] = state;
  }
}

const Tank::Input *Control::lastInput(ActorId actor) const {
  std::map<ActorId, Tank::Input>::const_iterator iter = states.find(actor);
  if (iter == states.end())
    return NULL;

  return &iter->second;
}

Control::MoveRange Control::history(float time) {
  MoveRing::reverse_iterator iter = moves.rbegin(), e = moves.rend();
  for (; iter != e && iter->delta.time > time; ++iter);
  // FIXME: make sure there's no bug here if iter reaches moves.rend(),
  // what happens when we return moves.rend().base()?
  
  return Control::MoveRange(iter.base(), moves.end());
}

void Control::removeHistory(const MoveRing::iterator &first) {
  moves.pop_front(first);
}
