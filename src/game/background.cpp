
#include <game/background.h>
#include <engine/portal.h>
#include <engine/graphics.h>

Background::Background(class Portal &interfaces) {
  graphics = interfaces.requestInterface<Graphics>();
}
  
void Background::render() {
  
}

