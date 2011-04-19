
#include "log.h"

#include <iostream>
#include <cstdlib>
#include <fstream>


std::vector<Log::Consumer> Log::_consumers;

Log::Log(Severity severity, const char* file, const char* function, int line) 
  : _severity(severity) {
  std::string severity_name;
  switch (_severity) {
//  case SEVERITY_ERROR:
//    severity_name = "error";
//    break;
//  case SEVERITY_WARNING:
//    severity_name = "warning";
//    break;
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
}

void Log::clearConsumers() {
  _consumers.clear();
}

Log::DefaultLogConsumer::DefaultLogConsumer() : _level(SEVERITY_DEBUG) {
    char *level = getenv("TANKAGE_LOGGING_LEVEL");
    if (level) {
      _level = static_cast<Severity>(std::atoi(level)); 
    }
}

void Log::DefaultLogConsumer::operator()(Log::Severity severity, 
  const std::string &line) {
  if (_level >= severity) {
    std::cout << line << std::endl;
  }
}

void Log::ToFileConsumer::operator()(Log::Severity severity, 
  const std::string &line) {
    std::ofstream file;
    file.open(_path.c_str(), std::ios::app);
    if (!_path.empty() && file.is_open()) {
      file << line << std::endl;
      file.close();
    } else {
      // fall-back on the standard out
      std::cout << line << std::endl;
    } 
}

