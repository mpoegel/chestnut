#pragma once

#include <string>

namespace chestnut {

class EventCallback {
  public:
    ~EventCallback() {}

    virtual void pushEvent(const std::string &name, const std::string &value) = 0;
};

} // namespace chestnut
