#ifndef ENGINE_CFG_CFG_H
#define ENGINE_CFG_CFG_H

#include <engine/cfg.h>
#include <engine/config.h>
#include <utils/singleton.h>
#include <ptrcfg/propertynode.h>

#include <string>
#include <map>

namespace Configuration {
class Cfg : public ::Cfg, public Singleton<Cfg> { 
public:
  Cfg(const std::string &path = std::string(RESOURCE_PATH) + "/snail-wail.cfg");
  ~Cfg();

  std::string property(const std::string &system, const std::string &name);
  void updateProperty(const std::string &system, const std::string &name,
                      const std::string &value); 
  void registerConsumer(const std::string &system, CfgConsumer* consumer);

private:
  PropertyNode *_node;
  std::multimap<std::string,CfgConsumer*> _consumers;  
};
}

#endif // !ENGINE_CFG_CFG_H

