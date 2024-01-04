
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  DOCK,
  HANGAR,
  JUMP,
  LOOT,
  SCANNED,
  SLOT,
  VELOCITY
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 7>;

} // namespace bsgo
