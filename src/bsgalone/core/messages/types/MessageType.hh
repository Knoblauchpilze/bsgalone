
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::core {

enum class MessageType
{
  HANGAR_DATA,
  LOCKER_DATA,
  LOGIN,
  LOGIN_REQUEST,
  LOGOUT,
  LOGOUT_REQUEST,
  SHOP_DATA,
  SIGNUP,
  SIGNUP_REQUEST,
  SYSTEM_DATA,
  UNDOCK,
  UNDOCK_REQUEST
};

auto str(const MessageType &type) -> std::string;

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>;

} // namespace bsgalone::core
