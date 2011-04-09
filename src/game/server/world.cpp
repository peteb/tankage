#include "world.h"
#include <game/server/map.h>
#include <game/server/gameserver.h>

World::World(class GameServer *server) 
  : _server(server)
  , _phys_world(b2Vec2(0.0f, 0.0f), false)
{
  Map &map = server->map();
  
  b2BodyDef static_body_def;
  b2Body *static_body = _phys_world.CreateBody(&static_body_def);
  b2PolygonShape tile_shape;
  
  for (int y = -16; y < 16; ++y) {
    for (int x = -16; x < 16; ++x) {
      char tile_type = map.at(x, y);
      if (tile_type == 0) {
        tile_shape.SetAsBox(0.5f, 0.5f, b2Vec2(x - 0.5f, y - 0.5f), 0.0f);
        
        /*b2FixtureDef fixture_def;
        fixture_def.shape = &tile_shape;
        fixture_def.density = 1.0f;
        fixture_def.friction = 0.3f;*/
        static_body->CreateFixture(&tile_shape, 1.0f);
      }
      
    }
  }
}

b2Body *World::createTankBody() {
  b2BodyDef def;
  def.type = b2_dynamicBody;
  b2Body *body = _phys_world.CreateBody(&def);
  
  b2PolygonShape box;
  box.SetAsBox(27.0f/64.0f, 25.0f/64.0f);
  

  body->CreateFixture(&box, 1.0f);
  
  return body;
}

void World::tick() {
  for (int i = 0; i < 6; ++i) {
    _phys_world.Step(1.0f/60.0f, 6, 2);
    _phys_world.ClearForces();
    
  }
}
