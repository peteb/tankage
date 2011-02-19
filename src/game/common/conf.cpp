#include <game/common/conf.h>
#include <ptrcfg/propertytreeparser.h>

#include <sstream>
#include <fstream>
#include <iostream>

Conf::Conf(const std::string &path) {
  std::ifstream file(path.c_str(), std::ifstream::in); 

  if (!file.is_open() || !file.good()) 
    throw std::runtime_error("failed to open conf file: " + path);   

  std::stringstream buffer;
  buffer << file.rdbuf();
  PropertyTreeParser parser;
  _node = new PropertyNode(parser.parse(buffer.str()));

  file.close();
} // Conf

Conf::~Conf() {
  delete _node;
} // ~Conf

std::string Conf::property(const std::string &system, const std::string &name) {
  return _node->getNode(system).getProperty(name);  
} // property

void Conf::updateProperty(const std::string &system, const std::string &name, 
                          const std::string &value) {
  _node->getNode(system).addProperty(Property(name, value));

  std::multimap<std::string, ConfConsumer*>::iterator it;
  for (it = _consumers.begin(); it != _consumers.end(); ++it) {
    if (it->first == system) {
      it->second->updateConf(name, value);
    }
  } // for 
} 

void Conf::registerConsumer(const std::string &system, ConfConsumer* consumer) {
  _consumers.insert(std::pair<std::string,ConfConsumer*>(system, consumer));
}

