#ifndef GAME_CLIENT_RESOURCE_RENDERER_H
#define GAME_CLIENT_RESOURCE_RENDERER_H

#include <game/client/snapshot.h>
#include <game/server/resource.h>

class ResourceRenderer {
public:
  ResourceRenderer(class GameClient *client, class Portal &services);
  
  void render();
  void addSnapshot(const Snapshot<Resource::State> &snapshot);
  
private:
  typedef Snapshot<Resource::State> ResourceSnapshot;
  
  ResourceSnapshot _current;
  ResourceSnapshot _last;
  
  class GameClient *_client;
  class Graphics *_gfx;
  class Texture *_texture;
};

#endif // !GAME_CLIENT_RESOURCE_RENDERER_H
