
#include "MessageType.hh"

namespace bsgalone::core {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::LOGIN:
      return "login";
    case MessageType::LOGIN_REQUEST:
      return "login_request";
    case MessageType::LOGOUT:
      return "logout";
    case MessageType::LOGOUT_REQUEST:
      return "logout_request";
    case MessageType::SIGNUP:
      return "signup";
    case MessageType::SIGNUP_REQUEST:
      return "signup_request";
    case MessageType::UNDOCK:
      return "undock";
    case MessageType::UNDOCK_REQUEST:
      return "undock_request";
    default:
      return "unknown";
  }
}

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>
{
  return std::unordered_set<MessageType>{
    MessageType::LOGIN,
    MessageType::LOGIN_REQUEST,
    MessageType::LOGOUT,
    MessageType::LOGOUT_REQUEST,
    MessageType::SIGNUP,
    MessageType::SIGNUP_REQUEST,
    MessageType::UNDOCK,
    MessageType::UNDOCK_REQUEST,
  };
}

} // namespace bsgalone::core
