
#pragma once

#include <array>
#include <string>

namespace bsgo {

enum class MessageType
{
  SCANNED,
  LOOT,
  JUMP
};

auto str(const MessageType &type) -> std::string;
auto allMessageTypes() -> std::array<MessageType, 3>;

} // namespace bsgo
