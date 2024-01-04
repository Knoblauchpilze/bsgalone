
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  DOCK,
  HANGAR,
  JUMP,
  LOGIN,
  LOOT,
  SCANNED,
  SLOT,
  VELOCITY
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 8>;

} // namespace bsgo
