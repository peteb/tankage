#include <game/common/control.h>
#include <game/common/actors.h>
#include <game/common/tank.h>

#include <engine/input.h>
#include <engine/portal.h>
#include <utils/vec.h>

void Control::init(const class Portal &interfaces) {
  input = interfaces.requestInterface<Input>();
  keyUp = input->keycode("W");
  keyDown = input->keycode("S");
  keyShoot = input->keycode("mouse1");
  keyRight = input->keycode("D");
  keyLeft = input->keycode("A");
}

void Control::update() {
  triggerState(keyUp, Tank::STATE_MOVE_UP);
  triggerState(keyDown, Tank::STATE_MOVE_DOWN);
  triggerState(keyRight, Tank::STATE_TURN_RIGHT);
  triggerState(keyLeft, Tank::STATE_TURN_LEFT);
  triggerState(keyShoot, Tank::STATE_SHOOT);


/*  Tank *target = context->snails()->snail(Snails::SNAIL_LEFT);

  int x, y;
  input->mousePos(x, y);
  target->setCursor(vec2(x, y));*/
  
}


void Control::triggerState(int keycode, Tank::State state) {
  /*Snail *target = context->snails()->snail(Snails::SNAIL_LEFT);

  if (input->keyWasPressed(keycode)) {
    target->startState(state);
  }
  if (input->keyWasReleased(keycode)) {
    target->stopState(state);
    }*/

}
