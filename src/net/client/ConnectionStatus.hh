
#pragma once

#include <string>

namespace net::details {

enum class ConnectionStatus
{
  DISCONNECTED,
  CONNECTING,
  CONNECTED
};

auto str(const ConnectionStatus status) -> std::string;

} // namespace net::details
