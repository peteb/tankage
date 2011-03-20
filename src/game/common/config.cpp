#include <game/common/config.h>
#include <ptrcfg/propertytreeparser.h>
#include <ptrcfg/propertytreeprinter.h>

#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

void Config::init(const class Portal &modules) {
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

  _node = PropertyNode::MergeTrees(_node, parser.parse(buffer.str()));
}

void Config::start() {
  
}

Config::Config(const std::string &path) 
  : _path(path)
  , _node("root")
{
} // Config

Config::~Config() {
  // FIXME: before we quit, loop through all consumers and get their values
  
  std::stringstream buffer;
  PropertyTreePrinter printer(buffer);
  printer.print(_node); 
  // write config file
  std::ofstream file(_path.c_str(), std::ios::out); 
  if (file.is_open()) {
    file << buffer.str();
    file.close();
  } else {
    // don't bother, probably no permissions, keep using defaults
    std::cout << "DE failed to write config file: " << _path << std::endl;
  }
} // ~Config


void Config::registerVariable(const char *subtree,
                              const char *name,
                              VariableBase *variable) {
  try {
    // read value from vartree if it exists
    variable->assign(_node.getNode(subtree).getProperty(name));
  }
  catch (...) {
  }
  
  std::string varid = std::string(subtree) + "." + std::string(name);
  if (_consumers.find(subtree) != _consumers.end()) {
    throw std::runtime_error(std::string("variable '") + varid + "' already set");
  }
  
  _consumers[varid] = variable;
}


std::string Config::property(const std::string &system,
                                      const std::string &name,
                                      const std::string &defaultValue) {
  try {
    _node.getNode(system); 
  } catch (...) {
    _node.addNode(PropertyNode(system));
  }    
  try {
    _node.getNode(system).getProperty(name);
  } catch (...) {
    _node.getNode(system).addProperty(Property(name, defaultValue));
  }
  return _node.getNode(system).getProperty(name);  
} // property

void Config::updateProperty(const std::string &system, 
                            const std::string &name, 
                            const std::string &value) {

  
  ConsumerMap::iterator iter = _consumers.find(system + "." + name);
  if (iter == _consumers.end()) {
    throw std::runtime_error("unknown variable: '" + system + "." + name + "'");
  }
  
  iter->second->assign(value);
} // updateProperty 


void Config::parse(const std::vector<char *> &args) {
  for (size_t i(1); i < args.size(); ++i) {
    const std::string &arg = args[i];
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


