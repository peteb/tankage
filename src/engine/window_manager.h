#ifndef ENGINE_WINDOW_MANAGER_H
#define ENGINE_WINDOW_MANAGER_H

#include <engine/interface.h>
#include <utils/rect.h>

class WindowManager : public Interface {
public:
  static const std::string &id() {
    static const std::string _id = "window_manager";
    return _id;
  }
  
  virtual rect size() const =0;
};

#endif // !ENGINE_WINDOW_MANAGER_H
