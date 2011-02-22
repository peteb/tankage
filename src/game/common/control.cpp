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
{
  state = 0;
}

void Control::init(const class Portal &interfaces) {
  input = interfaces.requestInterface<Input>();
  config = interfaces.requestInterface<Config>();
  wm = interfaces.requestInterface<WindowManager>();
  lastTick = wm->timeSeconds();
  keyUp = input->keycode(config->property("control", "keyUp"));
  keyDown = input->keycode(config->property("control", "keyDown"));
  keyShoot = input->keycode(config->property("control", "keyShoot"));
  keyRight = input->keycode(config->property("control", "keyRight"));
  keyLeft = input->keycode(config->property("control", "keyLeft"));
}

void Control::update() {
  // PlayerId -> Tank instance.. lots of indirection here
  PlayerId localPlayer = context->players()->localPlayer();
  if (localPlayer != 0) {
    Player *player = context->players()->player(localPlayer);
    if (player) {
      Tank *target = context->actors()->tank(player->actor());

      if (target) {
        triggerState(keyUp, Tank::STATE_MOVE_UP, target);
        triggerState(keyDown, Tank::STATE_MOVE_DOWN, target);
        triggerState(keyRight, Tank::STATE_TURN_RIGHT, target);
        triggerState(keyLeft, Tank::STATE_TURN_LEFT, target);
        triggerState(keyShoot, Tank::STATE_SHOOT, target);

        int x, y;
        input->mousePos(x, y);
        target->setCursor(vec2(x, y));
        cursorPos = vec2(x, y);
      }
      else {
        std::cout << "no tank :(" << std::endl;
      }
    }
  }    
    
}

void Control::onTick(Client *client) {
  NetPlayerInput msg;
  msg.type = NET_PLAYER_INPUT;
  msg.state = state;
  msg.target_x = htons(cursorPos.x + 32768);
  msg.target_y = htons(cursorPos.y + 32768);

  client->send(&msg, sizeof(NetPlayerInput), Client::PACKET_UNSEQUENCED, NET_CHANNEL_ABS);
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

    for (unsigned i = 0; i < Tank::STATE_MAX; ++i) {
      // TODO: just work on deltas here
      if (msg->state & (1 << i)) {
        tank->startState((Tank::State)i);
      }
      else {
        tank->stopState((Tank::State)i);
      }
    }
    
    vec2 cursor(ntohs(msg->target_x) - 32768,
                ntohs(msg->target_y) - 32768);

    tank->setCursor(cursor);
  }
}

void Control::triggerState(int keycode, Tank::State state, Tank *target) {
  if (input->keyWasPressed(keycode)) {
    target->startState(state);
    this->state |= (1 << state);
  }
  if (input->keyWasReleased(keycode)) {
    target->stopState(state);
    this->state &= ~(1 << state);
  }
}
