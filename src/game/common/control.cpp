#include <game/common/control.h>
#include <game/common/actors.h>
#include <game/common/tank.h>
#include <game/common/players.h>

#include <engine/input.h>
#include <engine/portal.h>
#include <engine/window_manager.h>

#include <utils/vec.h>
#include <iostream>

Control::Control()
  : ReplicatedSystem(CLIENT_TICK|SERVER_RECEIVE)
{
}

void Control::init(const class Portal &interfaces) {
  input = interfaces.requestInterface<Input>();
  wm = interfaces.requestInterface<WindowManager>();
  lastTick = wm->timeSeconds();
  
  keyUp = input->keycode("W");
  keyDown = input->keycode("S");
  keyShoot = input->keycode("mouse1");
  keyRight = input->keycode("D");
  keyLeft = input->keycode("A");
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
        // TODO: replicate to server if >= 1/25 since last
      }
      else {
        std::cout << "no tank :(" << std::endl;
      }
    }
  }    
    
}

#include <iostream>
void Control::onTick(Client *client) {
  std::cout << "CONTROL TICK" << std::endl;
}

void Control::triggerState(int keycode, Tank::State state, Tank *target) {
  if (input->keyWasPressed(keycode)) {
    target->startState(state);
  }
  if (input->keyWasReleased(keycode)) {
    target->stopState(state);
  }
}
