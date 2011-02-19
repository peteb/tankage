#ifndef GAME_CONF_H
#define GAME_CONF_H

#include <ptrcfg/propertynode.h>

#include <string>
#include <map>

class ConfConsumer {
public: 
  virtual void updateConf(const std::string &name, const std::string &value) =0;
};

class Conf {
public:
  Conf(const std::string &path);
  ~Conf();
		
  std::string property(const std::string &system, const std::string &name);
  void updateProperty(const std::string &system, const std::string &name,
                      const std::string &value); 
  void registerConsumer(const std::string &system, ConfConsumer* consumer);

private:
  PropertyNode *_node;
  std::multimap<std::string,ConfConsumer*> _consumers;
};

#endif // !GAME_CONF_H

