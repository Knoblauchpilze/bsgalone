
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  DOCK,
  JUMP,
  LOOT,
  SCANNED,
  SLOT,
  VELOCITY
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 6>;

} // namespace bsgo
