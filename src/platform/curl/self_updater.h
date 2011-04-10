#ifndef PLATFORM_CURL_SELF_UPDATER_H
#define PLATFORM_CURL_SELF_UPDATER_H

#include <platform/self_updater.h>
#include <utils/singleton.h>

namespace Curl {
class SelfUpdater : public ::SelfUpdater, public Singleton<SelfUpdater>  {
public:
  UpdateResult *requestUpdate(const std::string &url);
};

}
#endif // !CURL_SELF_UPDATER_H

