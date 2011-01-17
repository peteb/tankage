#include <game/cactus.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <utils/rect.h>
#include <utils/vec.h>

Cactus::Cactus(const vec2 &pos, class Texture *tex)
  : Item(pos, 20.0f)
  , tex(tex)
  , health(20.0f)
{
}

void Cactus::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  vec2 roundedPos;
  roundedPos.x = round(pos.x);
  roundedPos.y = round(pos.y);

  gfx->drawQuad(rect(roundedPos, 64, 64));
}

bool Cactus::update(double dt) {
  if (health <= 0.0f)
    return false;
  
  pos += vec2(0.0f, -200.0f) * dt;

  return (pos.y + 64.0f >= 0.0f);
}

bool Cactus::takeDamage(const vec2 &pos, float damage) {
  if (health <= 0.0f)
    return false;
  
  health -= damage;
  return true;
}
