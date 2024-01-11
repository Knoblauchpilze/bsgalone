
#include "MessageType.hh"

namespace net {

auto str(const MessageType type) -> std::string
{
  switch (type)
  {
    case MessageType::LOGIN_REQUEST:
      return "login_request";
    default:
      return "unknown";
  }
}

} // namespace net
