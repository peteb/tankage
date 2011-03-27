#ifndef GAME_CLIENT_BULLET_RENDERER_H
#define GAME_CLIENT_BULLET_RENDERER_H

#include <game/client/snapshot.h>
#include <game/server/bullet.h>

class BulletRenderer {
public:
  BulletRenderer(class GameClient *client, class Portal &services);
  
  void render();
  void addSnapshot(const Snapshot<Bullet::State> &snapshot);

private:
  typedef Snapshot<Bullet::State> BulletSnapshot;
  
  BulletSnapshot _current;
  
  class GameClient *_client;
  class Graphics *_gfx;
  class Texture *_bullet_texture;
};

#endif // !GAME_CLIENT_BULLET_RENDERER_H
