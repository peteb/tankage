#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

class SystemContext {
public:
  SystemContext();
  
  void set(class Snails *snails);
  void set(class Background *background);
  void set(class Control *control);
  
  class Snails *snails() const;
  class Background *background() const;
  class Control *control() const;
  
  void init();
  
private:
  class Snails *_snails;
  class Background *_background;
  class Control *_control;
  
  bool ready;
};

class System {
public:
  System(SystemContext *ctx) : context(ctx) {}
  virtual ~System() {}

  virtual void init() {}
  
protected:
  const SystemContext *context;
};

#endif // !GAME_SYSTEM_H
