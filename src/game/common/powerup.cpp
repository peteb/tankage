#include <game/common/powerup.h>
#include <game/common/snails.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <utils/rect.h>

Powerup::Powerup(const vec2 &pos, class Texture *tex,
                 const std::string &type, int amount, const class SystemContext *ctx)
  : Item(pos, 16.0f)
  , tex(tex)
  , type(type)
  , amount(amount)
  , vel(vec2(0.0f, -200.0f))
  , ctx(ctx)
{
  radius = 16.0f;
}

void Powerup::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  vec2 roundedPos;
  roundedPos.x = round(pos.x);
  roundedPos.y = round(pos.y);

  gfx->drawQuad(rect(roundedPos, 8, 8));
}

bool Powerup::update(double dt) {
  const vec2 prevPos = pos;
  pos += vel * dt;
  
  vec2 hitPos;
  Snail *hit = ctx->snails()->intersectingSnails(prevPos, pos, radius, NULL, hitPos);
  if (hit) {
    if (hit->takeItem(type, amount))
      return false; // remove the powerup if the snail took it
  }
  
  // return false if we're outside the screen
  return (pos.y + 16.0f >= 0.0f) || (pos.x < -16.0f) || (pos.x > 816.0f);
}

bool Powerup::takeDamage(const vec2 &pos, float damage, Snail *shooter) {
  // See in which direction we should start flying, then change the velocity
  // accordingly
  vec2 velfactor = vec2::Identity();
  
  if (shooter->position().x < this->pos.x)
    velfactor = vec2(-1.0f, 0.0f);
  else
    velfactor = vec2(1.0f, 0.0f);

  vel = vec2(400.0f, 0.0f) * velfactor;

  return true;
}
