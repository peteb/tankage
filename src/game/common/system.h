#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H


class SystemContext {
public:
  SystemContext();

  enum SystemId {
    SYSTEM_ACTORS = 0,
    SYSTEM_BACKGROUND,
    SYSTEM_CONTROL,
    SYSTEM_TEXTURE_LOADER,
    SYSTEM_PROJECTILES,            // dependent on texture loader
    SYSTEM_PARTICLES,
    SYSTEM_GAMESERVER,
    SYSTEM_GAMECLIENT,
    SYSTEM_PLAYERS,
    SYSTEM_CONFIG,
    SYSTEM_MAX
  };
  
  void set(unsigned id, class System *system);

  bool isServer() const {return systems[SYSTEM_GAMESERVER]; }
  
  template<typename T>
  T *system(unsigned id) const {
    return reinterpret_cast<T *>(resolveSystem(id));
  }
  
  template<typename T>
  T *registerSystem() {
    T *sys = new T;
    set(T::id(), sys);
    return sys;
  }
  
  void init(class Portal &modules);
  void start();
  
  class Projectiles *projectiles() const {
    return system<Projectiles>(SystemContext::SYSTEM_PROJECTILES);
  }

  class Actors *actors() const {
    return system<Actors>(SystemContext::SYSTEM_ACTORS);
  }

  class Control *control() const {
    return system<Control>(SystemContext::SYSTEM_CONTROL);
  }

  class Particles *particles() const {
    return system<Particles>(SystemContext::SYSTEM_PARTICLES);
  }

  class TextureLoader *textureLoader() const {
    return system<TextureLoader>(SystemContext::SYSTEM_TEXTURE_LOADER);
  }

  class Players *players() const {
    return system<Players>(SystemContext::SYSTEM_PLAYERS);
  }

  class GameServer *gameserver() const {
    return system<GameServer>(SystemContext::SYSTEM_GAMESERVER);
  }

  class GameClient *gameclient() const {
    return system<GameClient>(SystemContext::SYSTEM_GAMECLIENT);
  }
  
private:
  class System *resolveSystem(unsigned id) const;
  
  class System *systems[SYSTEM_MAX];
  
  bool ready;
};

class System {
public:
  virtual ~System() {}

  virtual void init(const class Portal &modules) =0;
  virtual void start() {}
  
  friend class SystemContext;
protected:
  const SystemContext *context;
};

#endif // !GAME_SYSTEM_H
