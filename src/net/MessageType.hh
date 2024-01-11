
#pragma once

#include <string>

namespace net {

enum class MessageType
{
  LOGIN_REQUEST
};

auto str(const MessageType type) -> std::string;

} // namespace net
