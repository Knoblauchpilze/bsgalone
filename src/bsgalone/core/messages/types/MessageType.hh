
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::core {

enum class MessageType
{
  LOGIN,
  LOGIN_REQUEST,
  SIGNUP_REQUEST
};

auto str(const MessageType &type) -> std::string;

auto allMessageTypesAsSet() -> std::unordered_set<MessageType>;

} // namespace bsgalone::core
