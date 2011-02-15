#ifndef GAME_CONFIGURATION_H
#define GAME_CONFIGURATION_H

#include <string>
#include <map>

class Configuration {
public:
  Configuration();
		
  std::string attribute(const std::string &system, const std::string &name);	
  void registerConsumer(const std::string &system, 
                        void (*callback)(const std::string &name)); 

private:
  std::map<std::string,std::string> _attributes;
};

#endif // !GAME_CONFIGURATION_H

