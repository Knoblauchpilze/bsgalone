
#include "ConnectionType.hh"

namespace net {

auto str(const ConnectionType &type) -> std::string
{
  switch (type)
  {
    case ConnectionType::SERVER:
      return "server";
    case ConnectionType::CLIENT:
      return "client";
    default:
      return "unknown";
  }
}

} // namespace net
