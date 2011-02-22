#include <engine/cfg/cfg.h>
#include <ptrcfg/propertytreeparser.h>

#include <sstream>
#include <fstream>
#include <iostream>

Engine::Config::Config(const std::string &path) {
  std::ifstream file(path.c_str(), std::ifstream::in); 

  if (!file.is_open() || !file.good()) 
    throw std::runtime_error("failed to open cfg file: " + path);   

  std::stringstream buffer;
  buffer << file.rdbuf();
  PropertyTreeParser parser;
  _node = new PropertyNode(parser.parse(buffer.str()));

  file.close();
} // Config

Engine::Config::~Config() {
  delete _node;
} // ~Config

std::string Engine::Config::property(const std::string &system,
                                     const std::string &name,
                                     const std::string &defaultValue) {
  return _node->getNode(system).getProperty(name, defaultValue);  
} // property

void Engine::Config::updateProperty(const std::string &system, 
										const std::string &name, 
									    const std::string &value) {
  _node->getNode(system).addProperty(Property(name, value));

  std::multimap<std::string, ConfigConsumer*>::iterator it;
  for (it = _consumers.begin(); it != _consumers.end(); ++it) {
    if (it->first == system) {
      it->second->updateConfig(name, value);
    }
  } // for 
} // updateProperty 


void Engine::Config::updateProperties(int argc, char **argv) {
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

void Engine::Config::registerConsumer(const std::string &system, 
										  ConfigConsumer* consumer) {
  _consumers.insert(std::pair<std::string,ConfigConsumer*>(system, consumer));
} // registerConsumer

