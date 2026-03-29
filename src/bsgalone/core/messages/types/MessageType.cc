
#include "MessageType.hh"

namespace bsgalone::core {

auto str(const MessageType &type) -> std::string
{
  switch (type)
  {
    case MessageType::LOGIN_REQUEST:
      return "login_request";
    case MessageType::SIGNUP_REQUEST:
      return "signup_request";
    default:
      return "unknown";
  }
}

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>
{
  return std::unordered_set<MessageType>{
    MessageType::LOGIN_REQUEST,
    MessageType::SIGNUP_REQUEST,
  };
}

} // namespace bsgalone::core
