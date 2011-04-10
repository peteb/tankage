#include "self_updater.h"

namespace Curl {
class UpdateResult : public ::UpdateResult {
public:
  bool gotUpdate() const {
    return false;
  }
};
}

::UpdateResult *Curl::SelfUpdater::requestUpdate(const std::string &file, 
                                                 const std::string &url) {
  return new Curl::UpdateResult;
}