#ifndef ENGINE_CFG_CFG_H
#define ENGINE_CFG_CFG_H

#include <engine/cfg.h>
#include <engine/config.h>
#include <utils/singleton.h>
#include <ptrcfg/propertynode.h>

#include <string>
#include <map>

namespace Engine {
class Config : public ::Config, public Singleton<Config> { 
public:
  Config(const std::string &path = "");
  ~Config();

  std::string property(const std::string &system, const std::string &name,
					   const std::string &defaultValue);
  void updateProperty(const std::string &system, const std::string &name,
                      const std::string &value); 
  void updateProperties(int argc, char **argv);
  void registerConsumer(const std::string &system, ConfigConsumer* consumer);

private:
  std::string _path;
  PropertyNode *_node;
  std::multimap<std::string,ConfigConsumer*> _consumers;  
};
}

#endif // !ENGINE_CFG_CFG_H

