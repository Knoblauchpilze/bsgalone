
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
  SLOT
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 5>;

} // namespace bsgo
