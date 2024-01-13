
#pragma once

#include "MessageType.hh"

namespace net {

inline constexpr auto messageTypeSize() -> std::size_t
{
  return sizeof(std::underlying_type<MessageType>::type);
}

} // namespace net
