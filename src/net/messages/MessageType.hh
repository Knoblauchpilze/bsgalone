
#pragma once

#include <cstdint>
#include <string>

namespace net {

enum class MessageType : int32_t
{
  LOGIN_REQUEST
};

constexpr auto messageTypeSize() -> std::size_t;
auto str(const MessageType type) -> std::string;

} // namespace net

#include "MessageType.hxx"
