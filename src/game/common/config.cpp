#include <game/common/config.h>
#include <ptrcfg/propertytreeparser.h>
#include <ptrcfg/propertytreeprinter.h>

#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

Config::Config(const std::string &path) : _path(path) {
  if (_path.empty()) {
    char *home = getenv("HOME");
    if (home) {
      _path = std::string(home).append("/.tankage.conf");
    }
  } else {
    // FIXME kaspars: add Windows stuff
  }
  std::stringstream buffer;
  PropertyTreeParser parser;
  // read config file
  std::ifstream file(_path.c_str(), std::ifstream::in); 
  if (file.is_open()) {
    buffer << file.rdbuf();
    file.close();
  } else {
    // initial start-up, file is not created, yet
    std::cout << "DE failed to read config file: " << _path << std::endl;
  }
  _node = new PropertyNode(parser.parse(buffer.str()));
} // Config

Config::~Config() {
  std::stringstream buffer;
  PropertyTreePrinter printer(buffer);
  printer.print(*_node); 
  // write config file
  std::ofstream file(_path.c_str(), std::ios::out); 
  if (file.is_open()) {
    file << buffer.str();
    file.close();
  } else {
    // don't bather, probably no permissions, keep using defaults
    std::cout << "DE failed to write config file: " << _path << std::endl;
  }
  delete _node;
} // ~Config

std::string Config::property(const std::string &system,
                                     const std::string &name,
                                     const std::string &defaultValue) {
  try {
    _node->getNode(system); 
  } catch (...) {
    _node->addNode(PropertyNode(system));
  }    
  try {
    _node->getNode(system).getProperty(name);
  } catch (...) {
    _node->getNode(system).addProperty(Property(name, defaultValue));
  }
  return _node->getNode(system).getProperty(name);  
} // property

void Config::updateProperty(const std::string &system, 
										const std::string &name, 
									    const std::string &value) {
  // don't allow to update non-existing attributes
  _node->getNode(system).getProperty(name);
  _node->getNode(system).addProperty(Property(name, value));

  /*std::multimap<std::string, ConfigConsumer*>::iterator it;
  for (it = _consumers.begin(); it != _consumers.end(); ++it) {
    if (it->first == system) {
      it->second->updateConfig(name, value);
    }
  } */// for 
} // updateProperty 


void Config::updateProperties(int argc, char **argv) {
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

//void Config::registerConsumer(const std::string &system, 
//										  ConfigConsumer* consumer) {
//  _consumers.insert(std::pair<std::string,ConfigConsumer*>(system, consumer));
//} // registerConsumer
//
