#include <game/snails.h>

Snails::Snails(const class Portal &interfaces, SystemContext *ctx)
  : System(ctx)
{

}

void Snails::render() {

}

Snail *Snails::snail(int id) const {
  return new Snail;
}

void Snail::startState(SnailState state) {
  
}
