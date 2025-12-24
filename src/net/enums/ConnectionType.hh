
#pragma once

#include <string>

namespace net {

enum class ConnectionType
{
  SERVER,
  CLIENT
};

auto str(const ConnectionType type) -> std::string;

} // namespace net
