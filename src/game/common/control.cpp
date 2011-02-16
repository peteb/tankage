#include <game/common/control.h>
#include <game/common/snails.h>
#include <engine/input.h>
#include <engine/portal.h>
#include <utils/vec.h>

void Control::init(const class Portal &interfaces) {
  input = interfaces.requestInterface<Input>();
  keyUp = input->keycode("up");
  keyDown = input->keycode("down");
  keyShoot = input->keycode("space");
  keyRight = input->keycode("right");
  keyLeft = input->keycode("left");
}

void Control::update() {
  triggerState(keyUp, Snail::STATE_MOVE_UP);
  triggerState(keyDown, Snail::STATE_MOVE_DOWN);
  triggerState(keyRight, Snail::STATE_TURN_RIGHT);
  triggerState(keyLeft, Snail::STATE_TURN_LEFT);
  triggerState(keyShoot, Snail::STATE_SHOOT);


  Snail *target = context->snails()->snail(Snails::SNAIL_LEFT);

  int x, y;
  input->mousePos(x, y);
  target->setCursor(vec2(x, y));
}


void Control::triggerState(int keycode, Snail::SnailState state) {
  Snail *target = context->snails()->snail(Snails::SNAIL_LEFT);

  if (input->keyWasPressed(keycode)) {
    target->startState(state);
  }
  if (input->keyWasReleased(keycode)) {
    target->stopState(state);
  }

}
