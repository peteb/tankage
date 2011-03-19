
#include "log.h"
#include <iostream>

using namespace tankage;

std::vector<log::log_consumer_t> log::_consumers;

log::log(severity_t severity, const char* file, const char* function, int line) 
  : _severity(severity) {
  std::string severity_name;
  switch (_severity) {
  case severity_error:
    severity_name = "error";
    break;
  case severity_warning:
    severity_name = "warning";
    break;
  case severity_info:
    severity_name = "info";
    break;
  case severity_debug:
    severity_name = "debug";
    break;
  default:
    severity_name = "??";
  }
  // FIXME kaspars: need to strip file name from it's full path
  _stream << "(" << severity_name << " " << function << ":" << line << ")> "; 
} // log

log::~log() {
  for (std::vector<log_consumer_t>::iterator it = _consumers.begin(); 
    it != _consumers.end(); ++it) {
    (*it)(_severity, _stream.str());
  }
} // ~log

void log::register_consumer(const log_consumer_t &consumer) {
  _consumers.push_back(consumer); 
} // register_consumer 

