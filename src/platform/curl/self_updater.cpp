#include "self_updater.h"
#include <curl/curl.h>

namespace Curl {
class UpdateResult : public ::UpdateResult {
public:
  bool gotUpdate() const {
    return false;
  }
};
}

Curl::SelfUpdater::SelfUpdater() {
    
}

Curl::SelfUpdater::~SelfUpdater() {
  
}


::UpdateResult *Curl::SelfUpdater::requestUpdate(const std::string &file, 
                                                 const std::string &url) {
  
  CURL *handle = curl_easy_init();
  
  return new Curl::UpdateResult;
}