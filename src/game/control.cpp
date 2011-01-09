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
  // FIXME: trigger state start, end.
  // FIXME: in input, add state support.
  /*
    keyPressed -> keyIsPressed
    
    if (input->keyWasPressed(bla)) { // read state + mark as handled
    enterState();
    }
    if (input->keyWasReleased()) { // read state + mark as handled
    leaveState();
    }

    ...

    use KeyState {wasPressed, wasReleased}    ... keyState(bla)

   */
  if (input->keyPressed(keyUp)) {
    Snail *target = context->snails()->snail(Snails::SNAIL_LEFT);
    target->startState(Snail::STATE_MOVE_UP);
  }

  if (input->keyPressed(keyDown)) {
    
  }

  if (input->keyPressed(keyShoot)) {

  }
}

