
#pragma once

#include <cstdint>
#include <string>

namespace net {

enum class MessageType : int32_t
{
  LOGIN_REQUEST
};

auto str(const MessageType type) -> std::string;

} // namespace net
