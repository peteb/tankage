#include <engine/cfg/cfg.h>
#include <ptrcfg/propertytreeparser.h>

#include <sstream>
#include <fstream>
#include <iostream>

Configuration::Cfg::Cfg(const std::string &path) {
  std::ifstream file(path.c_str(), std::ifstream::in); 

  if (!file.is_open() || !file.good()) 
    throw std::runtime_error("failed to open cfg file: " + path);   

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


void Configuration::Cfg::updateProperties(int argc, char **argv) {
  for (int i(1); i != argc; ++i) {
    std::string arg = argv[i];
    size_t dot = arg.find(".");
    if (dot == std::string::npos) 
      throw std::runtime_error("invalid argument: missing ."); 
    
    size_t equals = arg.find("=");
    if (equals == std::string::npos || dot > equals)
      throw std::runtime_error("invalid argument: missing =");

    std::string system = arg.substr(0, dot); 
    std::string name = arg.substr(dot+1, equals-(dot+1));
    std::string value = arg.substr(equals+1);
    if (system.empty() || name.empty() || value.empty())
      throw std::runtime_error("invalid argument: " + arg);    

    updateProperty(system, name, value);
  } // for
} // updateProperties

void Configuration::Cfg::registerConsumer(const std::string &system, 
										  CfgConsumer* consumer) {
  _consumers.insert(std::pair<std::string,CfgConsumer*>(system, consumer));
} // registerConsumer

