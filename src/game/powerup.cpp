#include <game/powerup.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <utils/rect.h>

Powerup::Powerup(const vec2 &pos, class Texture *tex, const std::string &type, int amount)
  : Item(pos, 16.0f)
  , tex(tex)
  , type(type)
  , amount(amount)
  , taken(0)
{
}

void Powerup::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  vec2 roundedPos;
  roundedPos.x = round(pos.x);
  roundedPos.y = round(pos.y);

  gfx->drawQuad(rect(roundedPos, 16, 16));
}

bool Powerup::update(double dt) {
  if (taken == 0)
    pos += vec2(0.0f, -200.0f) * dt;
  else
    pos += vec2(700.0f, 0.0f) * dt * static_cast<float>(taken);
  
  return (pos.y + 16.0f >= 0.0f) || (pos.x < -16.0f) || (pos.x > 816.0f);
}

bool Powerup::takeDamage(const vec2 &pos, float damage) {
  if (pos.x < this->pos.x)
    taken = -1;
  else
    taken = 1;
  
  return true;
}
