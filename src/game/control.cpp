#include <game/control.h>
#include <engine/input.h>
#include <engine/portal.h>
#include <game/snails.h>

Control::Control(const class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{
  input = interfaces.requestInterface<Input>();
  keyUp = input->keycode("up");
  keyDown = input->keycode("down");
  keyShoot = input->keycode("space");
}

void Control::update() {
  Snail *target = context->snails()->snail(Snails::SNAIL_LEFT);

  if (input->keyWasPressed(keyUp)) {
    target->startState(Snail::STATE_MOVE_UP);
  }
  if (input->keyWasReleased(keyUp)) {
    target->stopState(Snail::STATE_MOVE_UP);
  }

  if (input->keyWasPressed(keyDown)) {
    target->startState(Snail::STATE_MOVE_DOWN);
  }
  if (input->keyWasReleased(keyDown)) {
    target->stopState(Snail::STATE_MOVE_DOWN);
  }

}

