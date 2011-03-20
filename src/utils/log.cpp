
#include "log.h"

#include <iostream>


std::vector<Log::Consumer> Log::_consumers;

Log::Log(Severity severity, const char* file, const char* function, int line) 
  : _severity(severity) {
  std::string severity_name;
  switch (_severity) {
  case SEVERITY_ERROR:
    severity_name = "error";
    break;
  case SEVERITY_WARNING:
    severity_name = "warning";
    break;
  case SEVERITY_INFO:
    severity_name = "info";
    break;
  case SEVERITY_DEBUG:
    severity_name = "debug";
    break;
  default:
    severity_name = "??";
  }
  // FIXME kaspars: Fix this for Windows later
  std::string file_name = file ? file : "";
  size_t pos = file_name.find_last_of("/"); 
  file_name = file_name.substr(pos+1);
  _stream << "[" << severity_name << " " << file_name << ":" << line << " " 
    << function << "] "; 
} // log

Log::~Log() {
  for (std::vector<Consumer>::iterator it = _consumers.begin(); 
    it != _consumers.end(); ++it) {
    (*it)(_severity, _stream.str());
  }
} // ~log

void Log::registerConsumer(const Consumer &consumer) {
  _consumers.push_back(consumer); 
} // register_consumer 

