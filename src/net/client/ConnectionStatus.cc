
#include "ConnectionStatus.hh"

namespace net::details {

auto str(const ConnectionStatus status) -> std::string
{
  switch (status)
  {
    case ConnectionStatus::DISCONNECTED:
      return "disconnected";
    case ConnectionStatus::DISCONNECTING:
      return "disconnecting";
    case ConnectionStatus::CONNECTING:
      return "connecting";
    case ConnectionStatus::CONNECTED:
      return "connected";
    default:
      return "unknown";
  }
}

} // namespace net::details
