
#include "MessageType.hh"

namespace bsgalone::core {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::HANGAR_DATA:
      return "hangar_data";
    case MessageType::LOCKER_DATA:
      return "locker_data";
    case MessageType::LOGIN:
      return "login";
    case MessageType::LOGIN_REQUEST:
      return "login_request";
    case MessageType::LOGOUT:
      return "logout";
    case MessageType::LOGOUT_REQUEST:
      return "logout_request";
    case MessageType::SHOP_DATA:
      return "shop_data";
    case MessageType::SIGNUP:
      return "signup";
    case MessageType::SIGNUP_REQUEST:
      return "signup_request";
    case MessageType::SYSTEM_DATA:
      return "system_data";
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
    MessageType::HANGAR_DATA,
    MessageType::LOCKER_DATA,
    MessageType::LOGIN,
    MessageType::LOGIN_REQUEST,
    MessageType::LOGOUT,
    MessageType::LOGOUT_REQUEST,
    MessageType::SHOP_DATA,
    MessageType::SIGNUP,
    MessageType::SIGNUP_REQUEST,
    MessageType::SYSTEM_DATA,
    MessageType::UNDOCK,
    MessageType::UNDOCK_REQUEST,
  };
}

} // namespace bsgalone::core
