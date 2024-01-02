
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  SCANNED,
  LOOT,
  JUMP,
  DOCK
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 4>;

} // namespace bsgo
