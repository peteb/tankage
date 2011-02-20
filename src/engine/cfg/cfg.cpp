#include <engine/cfg/cfg.h>
#include <ptrcfg/propertytreeparser.h>

#include <sstream>
#include <fstream>
#include <iostream>

Configuration::Cfg::Cfg(const std::string &path) {
  std::ifstream file(path.c_str(), std::ifstream::in); 

  if (!file.is_open() || !file.good()) 
    throw std::runtime_error("failed to open conf file: " + path);   

  std::stringstream buffer;
  buffer << file.rdbuf();
  PropertyTreeParser parser;
  _node = new PropertyNode(parser.parse(buffer.str()));

  file.close();
} // Cfg

Configuration::Cfg::~Cfg() {
  delete _node;
} // ~Cfg

std::string Configuration::Cfg::property(const std::string &system,
                                         const std::string &name) {
  return _node->getNode(system).getProperty(name);  
} // property

void Configuration::Cfg::updateProperty(const std::string &system, 
										const std::string &name, 
									    const std::string &value) {
  _node->getNode(system).addProperty(Property(name, value));

  std::multimap<std::string, CfgConsumer*>::iterator it;
  for (it = _consumers.begin(); it != _consumers.end(); ++it) {
    if (it->first == system) {
      it->second->updateCfg(name, value);
    }
  } // for 
} // updateProperty 

void Configuration::Cfg::registerConsumer(const std::string &system, 
										  CfgConsumer* consumer) {
  _consumers.insert(std::pair<std::string,CfgConsumer*>(system, consumer));
} // registerConsumer

