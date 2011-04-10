#ifndef PLATFORM_SELF_UPDATER_H
#define PLATFORM_SELF_UPDATER_H

#include <platform/interface.h>
#include <string>

class SelfUpdater : public Interface {
public:  
  static std::string id() {return "self_updater"; }
  
  virtual void requestUpdate(const std::string &file,
                             const std::string &url) =0;
};

#endif // !PLATFORM_SELF_UPDATER_H
